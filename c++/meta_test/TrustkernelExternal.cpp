#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/system_properties.h>
#include <unistd.h>

/* for STL */
#include <string>
#include <unordered_map>

#include <pl.h>
#include <kphproxy.h>
#include <kphhelper.h>

#include "TrustkernelExternalPrivate.h"
#include "TrustkernelExternalCommand.h"

#include "MetaPub.h"
#include "LogDefine.h"

#define str(x) #x
#define xstr(x) str(x)

#define MAJOR_VERSION   3
#define MIDDL_VERSION   2
#define MINOR_VERSION   0

typedef enum candid
{
	KPH_GENERATE = 0,
	KPH_VERIFY_CONFIG,
	KPH_VERIFY_KEY,
	KPH_VERIFY_DATA,
	KPH_UPDATE_PSTATE,
}ID_CANDID;

using std::string;
using std::unordered_map;

struct file_buffer {
    uint8_t *buf;
    size_t length;

    file_buffer(uint8_t *_buf, size_t _len):
        buf(_buf), length(_len) {}

    file_buffer():
        buf(NULL), length(0) {}
};

static unordered_map<uint64_t, uint32_t> cmd_resps;
static unordered_map<string, file_buffer> id_buf;

const char meta_version[] = "metaserver-" xstr(MAJOR_VERSION.MIDDL_VERSION.MINOR_VERSION);

typedef struct {
    uint8_t *buf;
    int length;
} Buffer;

#define COPY_DATA(dst, src, len) \
    do { \
        memcpy(dst, src, len); \
        src += len; \
    } while(0);

#define OUTPUT_PATH     "/sdcard/tee"

#define LOG_BUF_MAX_LENGTH      255

static void log_to_kmsg(const char *prefix,
                        const char *func,
                        int lineno,
                        const char *buf)
{
    int klog_fd;
    struct iovec iov[1];

    char klog_buf[1024];
    size_t size;

    klog_fd = open("/dev/kmsg", O_WRONLY | O_CLOEXEC);

    if (klog_fd == -1)
        return;

    size = snprintf(klog_buf, sizeof(klog_buf),
        "[%s]<%s:%d>: %s", prefix, func, lineno, buf);

    iov[0].iov_base = (void *) klog_buf;
    iov[0].iov_len = size < 1024 ? size : 1024;

    writev(klog_fd, iov, 1);
    close(klog_fd);
}

void __kpha_debugprint(const char *func, int lineno, const char *prefix, const char *fmt, ...)
{
    FILE *file;
    va_list va_args;

    char logbuf[LOG_BUF_MAX_LENGTH + 1] = { 0 };

    va_start(va_args, fmt);
    vsnprintf(logbuf, LOG_BUF_MAX_LENGTH + 1, fmt, va_args);
    va_end(va_args);

    META_LOG("[%s]<%s:%d>: %s", prefix, func, lineno, logbuf);
    log_to_kmsg(prefix, func, lineno, logbuf);
}

static string build_id_from_name(const char *filename)
{
    string id(OUTPUT_PATH);

    if (strncmp(filename, OUTPUT_PATH, strlen(OUTPUT_PATH)) == 0) {
        id = filename;
    } else {
        id += "/";
        id += filename;
    }

    return id;
}

static uint8_t *buffer_from_id(string id, size_t *length)
{
    unordered_map<string, file_buffer>::iterator it;

    it = id_buf.find(id);
    if (it == id_buf.end())
        return NULL;

    *length = it->second.length;
    return it->second.buf;
}

static bool id_exist(string id)
{
    unordered_map<string, file_buffer>::iterator it;

    it = id_buf.find(id);
    if (it != id_buf.end()) {
        kpha_err("Buffer with same id=%s already exists\n", id.c_str());
        return false;
    }

    return true;
}

static bool insert_id_buf(string id, file_buffer buf)
{
    if (id_exist(id) == false)
        return false;

    id_buf.insert(unordered_map<string, file_buffer>
            ::value_type(id, buf));
    return true;
}

static int handle_kph_verify_ta_data(const int ta_id)
{
    int ret = 0;
    kpha_log("enter");
	if(ta_id)
	{
		if ((ret = kph_verify_ta_data2(ta_id,"keybox")))
		{
			kpha_err("verify google keybox failed");
		}
	
	}else
	{
		if ((ret = kph_verify_ta_data(ta_id)))
		{
			kpha_err("verify tee key failed");
		}
	}
    return ret;
}

static int handle_kph_verify_ta_key(const int ta_id)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = kph_verify_ta_key(ta_id))) {
        kpha_err("verify_ta_key failed");
    }

    return ret;
}

static int handle_kph_verify_device_config(void)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = kph_verify_device_config())) {
        kpha_err("verify_device_config failed");
    }

    return ret;
}

static int handle_kph_generate_key_legacy(const int ta_id)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = kph_generate_key_legacy(ta_id))) {
        kpha_err("generate_key_legacy failed");
    }

    return ret;
}

static int invoke_kpha_api (const ID_CANDID id, int num)
{
	int ret = 0;
	switch(id)
	{
            case KPH_GENERATE:
                ret = handle_kph_generate_key_legacy(num);
                break;
            case KPH_VERIFY_CONFIG:
                ret = handle_kph_verify_device_config();
                break;

            case KPH_VERIFY_KEY:
                ret = handle_kph_verify_ta_key(num);
                break;

            case KPH_VERIFY_DATA:
                ret = handle_kph_verify_ta_data(num);
                break;

            default:
                kpha_err("Unkown id = %d",id);
        }

        return ret;
}


static char *parser_cmd_buf( const char *cmdbuf, int &typ, int &num)
{
	uint32_t i,x;
        char *out_cmd_head = NULL;
	const char *candidate_cmd_head[] = { "generate", "verify_config", "verify_key", "verify_data", "update_pstate" };// 5                                                                                                                                 
	const char *candidate_num_head[] = {"6", "0"};

        kpha_log("input buf = %s",cmdbuf);
	for (i = 0; i < sizeof(candidate_cmd_head) / sizeof(candidate_cmd_head[0]); i++) {
		if (strstr(cmdbuf, candidate_cmd_head[i]) != NULL)
		{
			typ = i;
			out_cmd_head = (char *)candidate_cmd_head[i];
			for(x = 0; x< sizeof(candidate_num_head) / sizeof(candidate_num_head[0]); x++)
			{
				if(strstr(cmdbuf,candidate_num_head[x]) != NULL)
				{
					num = atoi(candidate_num_head[x]);
				}else
				{
					kpha_err("num not available");	
				}
			}
                        return out_cmd_head;
		}
    }

    return NULL;
   kpha_err("cmd not int candidate");
}


/* raw means pass the raw output buffer back to client */
static int __runcmd(const char *p, char *meta_resp, size_t meta_resp_size, int *r, bool raw)
{
    FILE *fp;
    int size,ret,id,num = -1;
    char linebuf[LOG_BUF_MAX_LENGTH];

    std::string resp;
    if (!raw) {
        resp.assign(p);
        resp += ":\n";
    }

    char *s_resp;

    if (p == NULL || meta_resp == NULL || r == NULL) {
        kpha_err("NULL cmd buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (meta_resp == NULL) {
        kpha_err("NULL meta_resp buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (r == NULL) {
        kpha_err("NULL ret");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (meta_resp_size < sizeof(meta_buf_header_t)) {
        kpha_err("Expecting buffer to be larger/equal than %zuB, got %ZuB",
            sizeof(meta_buf_header_t));
        return KPHA_ERR_SHORT_BUFFER;
    }

 #if 1 

    char *obj =NULL;
    obj = parser_cmd_buf(p,id,num);
    if(obj !=NULL)
        kpha_log("parser obj char = %s",obj);

    ID_CANDID ucandid = (ID_CANDID)id;

    ret = invoke_kpha_api(ucandid,num);

    if(ret == -65528 || ret == -267386879){
        *r = 8;
    }else if(ret == -65533 || ret == 0){
        *r = 0;
    }

    kpha_log("YQ-- find id = %d,num =%d,invoke_kpha_api = %d *r = %d \n",ucandid,num,ret,*r);
#endif 

    if (raw || *r != 0) {
        uint64_t buf_ptr = 0ULL;
      	uint32_t buf_size = static_cast<uint32_t>(sizeof(ret));
        //int32_t buf_size = static_cast<uint32_t>(sizeof(resp.length() + 1));
        meta_buf_header_t *mbh = reinterpret_cast<meta_buf_header_t *>(meta_resp);
#if 0
        if (*r)
            *r = WEXITSTATUS(*r);
#endif
        kpha_log("kph_cmd=(%s) api ret = %d", p, *r);

        if ((s_resp = strdup(resp.c_str())) == NULL) {
            kpha_err("dup string length=%zu failed", resp.length());

            /* just throw away this buffer if we failed to dup it.
             * we cannot let a malloc failure to interrupt commands'
             * execution result
             */
            buf_ptr = 0;
            buf_size = 0;
        } else {
            buf_ptr = reinterpret_cast<uint64_t>(s_resp);

            cmd_resps.insert(std::unordered_map<uint64_t, uint32_t>
                ::value_type(buf_ptr, buf_size));
        }

        mbh->bufsize = buf_size;
        mbh->bufptr = buf_ptr;
    }

err:
    return 0;
}

static int runcmd(const char *p, char *meta_resp, size_t meta_resp_size, bool raw = false)
{
    int cmd_r, r;

    if (meta_resp_size < sizeof(meta_buf_header_t)) {
        kpha_err("Invalid meta response buffer size: %u", meta_resp_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    /* the code guarantees that meta_resp is always
     * larger than sizeof(meta_buf_header_t) */
    r = __runcmd(p, meta_resp, sizeof(meta_buf_header_t), &cmd_r, raw);

    if (r)
        return r;

    if (!raw)
        return cmd_r == 0 ? 0 : KPHA_ERR_EXEC(cmd_r);

    ((meta_buf_header_t *) meta_resp)->ret = cmd_r;
    return 0;
}

static void dropcmdresp(uint64_t bufptr)
{
    std::unordered_map<uint64_t, uint32_t>::const_iterator it;
    if ((it = cmd_resps.find(bufptr)) != cmd_resps.end()) {
        void *p = reinterpret_cast<void *>(bufptr);
        cmd_resps.erase(it);
        free(p);
    }
}

static int __readcmdresp(char *buf, uint64_t bufptr, uint32_t *size)
{
    std::unordered_map<uint64_t, uint32_t>::const_iterator it;
    uint32_t resp_size;
    void *p;

    if ((it = cmd_resps.find(bufptr)) == cmd_resps.end()) {
        kpha_err("Invalid bufptr (0x%lx, %u)", bufptr, *size);
        return KPHA_ERR_CUSTOM;
    }

    p = reinterpret_cast<void *>(bufptr);

    if ((resp_size = it->second) > *size) {
        kpha_err("Buf 0x%lx expecting size %u, get resp_size %u",
            bufptr, size, resp_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    memcpy(buf, p, resp_size);
    *size = resp_size;

    cmd_resps.erase(it);
    free(p);
    return 0;
}

/* Android should allow unaligned memory access */
#pragma pack(1)

typedef struct {
    uint64_t bufptr;
    uint32_t size;
} readcmdresp_param_t;

#pragma pack()

static int readcmdresp(char *buf, int *size, char **resp)
{
    int r;
    char *p;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (size == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (*size < static_cast<int>(sizeof(readcmdresp_param_t))) {
        kpha_err("Expecting buffer to be larger than/equal to %zuB, got %dB",
            sizeof(readcmdresp_param_t), *size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    readcmdresp_param_t *readcmdresp_param = reinterpret_cast<readcmdresp_param_t *>(buf);

    if ((p = (char *) malloc(readcmdresp_param->size)) == NULL) {
        kpha_err("Failed malloc %u B", readcmdresp_param->size);
        return KPHA_ERR_OUT_OF_MEMORY;
    }

    if ((r = __readcmdresp(p, readcmdresp_param->bufptr, &readcmdresp_param->size))) {
        free(p);
        return r;
    }

    *resp = p;
    *size = (int) readcmdresp_param->size;

    return 0;
}

static int write_file(char *filename, char *buf_in, int buf_size)
{
    int fd, size, ret = KPHA_ERR_SUCCESS;

    if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
        kpha_err("open %s failed with %s(%d)", filename, strerror(errno), errno);
        return KPHA_ERR_POSIX(errno);
    }

    if ((size = write(fd, buf_in, buf_size)) != buf_size) {
        if (size == -1) {
            kpha_err("write %s failed with %s(%d)", filename, strerror(errno), errno);
            ret = KPHA_ERR_POSIX(errno);
            goto exit;
        } else {
            kpha_err("Expecting to write %dB, wrote %ZdB", buf_size, size);
            ret = KPHA_ERR_BAD_WRITE;
            goto exit;
        }
    }

exit:
    close(fd);
    return ret;
}

static int read_file(char *filename, char *buf_out, int *buf_size)
{
    int fd = 0, size = 0, ret = KPHA_ERR_SUCCESS;

    if (filename == NULL || buf_out == NULL || buf_size == 0) {
        kpha_err("Bad filename %p buf_out %p pbuf_size %p",
                filename, buf_out, buf_size);
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (*buf_size == 0) {
        kpha_err("buf_size == 0!");
        return KPHA_ERR_SHORT_BUFFER;
    }

    if ((fd = open(filename, O_RDONLY)) < 0) {
        kpha_err("open %s failed with %s(%d)", filename, strerror(errno), errno);
        return KPHA_ERR_POSIX(errno);
    }

    size = read(fd, buf_out, *buf_size);
    if (size < 0 && (strstr(filename, "device.finish") == NULL)) {
        kpha_err("read %s failed with %s(%d)", filename, strerror(errno), errno);
        ret = KPHA_ERR_POSIX(errno);
        goto exit;
    }

    *buf_size = size;
exit:
    close(fd);
    return ret;
}

static uint8_t decimal2hex(uint8_t b)
{
    if (b <= 9) {
        return b + '0';
    } else if (b >= 10 && b <= 15) {
        return b - 10 + 'a';
    } else {
        return 0xff;
    }
}

static int convert_bytes_to_hex(Buffer *buffer, char **hex_result)
{
    uint32_t length = buffer->length * 2 + 1;
    int i = 0, j = 0;
    uint8_t b = 0;

    *hex_result = (char *) malloc(length);

    if (*hex_result == NULL) {
        return KPHA_ERR_OUT_OF_MEMORY;
    }

    for (i = 0, j = 0; i < buffer->length; ++i, j += 2) {
        b = buffer->buf[i];
        (*hex_result)[j] = decimal2hex(b >> 4);
        (*hex_result)[j + 1] = decimal2hex(b & 0xf);
    }

    (*hex_result)[length - 1] = '\0';

    return 0;
}

static int get_rid(char *buf, int capacity, int *used)
{
    char *hex_str = NULL;
    int ret;

    int fd = open("/proc/rid", O_RDONLY);
    if (fd < 0) {
        kpha_err("Open /proc/rid failed with %s(%d)", strerror(errno), errno);
        return KPHA_ERR_POSIX(errno);
    }

    *used = read(fd, buf, capacity);
    Buffer to_conv = {
        (uint8_t*) buf,
        *used,
    };

    close(fd);

    if ((ret = convert_bytes_to_hex(&to_conv, &hex_str))) {
        kpha_err("convert bytes to hex fail with %d", ret);
        return ret;
    }

    *used = strlen(hex_str);
    if (*used + 1 > capacity) {
        kpha_err("Expecting buffer to be larger than %dB, got %dB", *used, capacity);
        free(hex_str);
        return KPHA_ERR_SHORT_BUFFER;
    } else {
        strcpy(buf, hex_str);
        free(hex_str);
        return 0;
    }
}

/* used does not include trailing '\0' */
static int get_serialno(char *buf, int capacity, int *used)
{
    int r;
    char serialno[PROP_VALUE_MAX];

    if ((r =  __system_property_get("ro.serialno", serialno)) < 0) {
        kpha_err("getprop(ro.serialno) failed with %d", r);
        return KPHA_ERR_POSIX(r);
    }

    if (r == 0) {
        kpha_err("Cannot find ro.serialno. Use fake one.");
        /* TKCOREOS00000000 */
        strcpy(serialno, "7CC02E0500000000");
        r = strlen(serialno);
    }

    *used = r + 1;

    if (capacity <= *used) {
        kpha_err("Expecting %dB, got %dB", *used, capacity);
        return KPHA_ERR_SHORT_BUFFER;
    }

    strcpy(buf, serialno);
    return 0;
}

static int parse_buf_and_write_file(char *buf, int buf_size)
{
    int filename_len = 0;
    int value_len = 0;
    int ret = 0;
    char *filename = NULL;
    char *value = NULL;
    char *p = buf;
    char tmp_name[256];

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB", sizeof(int), buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_DATA(&filename_len, p, sizeof(int));

    if (filename_len + 1 <= 0) {
        kpha_err("Bad filename_len %zuB", filename_len + 1);
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if ((filename = (char *) malloc(filename_len + 1)) == NULL) {
        kpha_err("Bad malloc filename %dB", filename_len);
        ret = KPHA_ERR_OUT_OF_MEMORY;
        goto err;
    }
    memset(filename, 0, filename_len + 1);

    if (buf_size < (int) (sizeof(int) + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB", sizeof(int) + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(filename, p, filename_len);

    if (buf_size < (int) (sizeof(int) * 2 + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) * 2 + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }

    COPY_DATA(&value_len, p, sizeof(int));

    if ((value = (char *) malloc(value_len + 1)) == NULL) {
        kpha_err("Bad malloc for value %dB", value_len + 1);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }

    memset(value, 0, value_len + 1);

    if (buf_size < (int) (sizeof(int) * 2 + filename_len + value_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) * 2 + filename_len + value_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }

    COPY_DATA(value, p, value_len);

    if (!insert_id_buf(build_id_from_name(filename),
            file_buffer((uint8_t *)value, (size_t)value_len))) {
        kpha_err("Failed due to duplicated id\n");
        ret = KPHA_ERR_CUSTOM;
        goto err;
    }

    free(filename);
    return 0;

err:
    free(filename);
    return ret;
}

static int parse_buf_and_import_config(char *buf, int buf_size)
{
    int ret = 0;
    char *p = buf;

    char *filename = NULL;
    int filename_len = 0;

    uint8_t *config_buf, *new_config_buf = NULL;
    uint8_t *pbuf;
    size_t config_len;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB", sizeof(int), buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    COPY_DATA(&filename_len, p, sizeof(int));

    if ((filename = (char *) malloc(filename_len + 1)) == NULL) {
        kpha_err("Bad malloc filename %dB", filename_len);
        ret = KPHA_ERR_OUT_OF_MEMORY;
        goto err;
    }
    memset(filename, 0, filename_len + 1);

    if (buf_size < (int) (sizeof(int) + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(filename, p, filename_len);

    if ((config_buf = buffer_from_id(build_id_from_name(filename),
            &config_len)) == NULL) {
        kpha_err("config_buf with filename=%s not exist\n", filename);
        ret = KPHA_ERR_CUSTOM;
        goto err;
    }

    pbuf = &config_buf[config_len - 1];
    while (isspace(*pbuf) && pbuf > config_buf) {
        *pbuf = '\0';
        --pbuf; --config_len;
    }

    if (pbuf == config_buf) {
        kpha_err("Empty config buf\n");
        ret = KPHA_ERR_CUSTOM;
        goto err;
    }

    if ((size_t) (pbuf - config_buf + 1) == config_len) {
        new_config_buf = (uint8_t *) malloc(config_len + 1);
        if (new_config_buf == NULL) {
            kpha_err("Failed to realloc buffer\n");
            ret = KPHA_ERR_OUT_OF_MEMORY;
            goto err;
        }

        memcpy(new_config_buf, config_buf, config_len);
        new_config_buf[config_len] = 0;

        config_buf = new_config_buf;
    }

    ret = kph_import_config((char *) config_buf, NULL, NULL);
    if (ret != 0) {
        kpha_err("Import config failed with %d\n", ret);
    }

    if (new_config_buf != NULL) {
        free(new_config_buf);
    }

err:
    free(filename);
    return ret;
}

static int parse_buf_and_import_ta_data(char *buf, int buf_size)
{
    char *filename = NULL;
    int filename_len = 0;

    int n = 0;
    int n_len = 0;

    int ret = 0;
    char *p = buf;

    uint8_t *data_buf;
    size_t data_len;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB",
                sizeof(int), buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    COPY_DATA(&filename_len, p, sizeof(int));

    if ((filename = (char *) malloc(filename_len + 1)) == NULL) {
        kpha_err("Bad malloc filename %dB", filename_len);
        ret = KPHA_ERR_OUT_OF_MEMORY;
        goto err;
    }
    memset(filename, 0, filename_len + 1);

    if (buf_size < (int) (sizeof(int) + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(filename, p, filename_len);

    if (buf_size < (int) (sizeof(int) * 2 + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) * 2 + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(&n_len, p, sizeof(int));

    if (n_len != (int) sizeof(n)) {
        kpha_err("Invalid n_len = %d\n", n_len);
        ret = KPHA_ERR_BAD_PARAMETERS;
        goto err;
    }

    if (buf_size < (int) (sizeof(int) * 2 + filename_len + n_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) * 2 + filename_len + n_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }

    COPY_DATA(&n, p, n_len);

    if ((data_buf = buffer_from_id(build_id_from_name(filename),
                    &data_len)) == NULL) {
        kpha_err("data_buf with filename=%s not exist\n", filename);
        ret = KPHA_ERR_CUSTOM;
        goto err;
    }

    /* legacy mode */
    ret = kph_import_ta_data((uint32_t) n, NULL, NULL, 0, data_buf, data_len);
    if (ret) {
        kpha_err("import ta data failed with %d\n", ret);
        goto err;
    }

err:
    free(filename);
    return ret;
}

static int parse_buf_and_generate_key(char *buf, int buf_size)
{
    int number = -1;
    int number_len = 0;
    int ret = 0;
    const char *comm_prefix = "kph -c generate -n";
    char comm[128] = { 0 };

    char *p = buf;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB", sizeof(int), buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_DATA(&number_len, p, sizeof(int));

    if (buf_size < (int) (sizeof(int) + number_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB", sizeof(int) + number_len, buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_DATA(&number, p, number_len);

    /*FIXME check rv of sprintf */
    sprintf(comm, "%s %d", comm_prefix, number);
    return runcmd(comm, buf, buf_size);
}

static int parse_buf_and_import_ta_key(char *buf, int buf_size)
{
    int ret = 0;
    char *p = buf;

    char *filename = NULL;
    int filename_len = 0;

    int n = 0;
    int n_len = 0;

    uint8_t *key_buf;
    size_t key_len;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB",
                sizeof(int), buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_DATA(&filename_len, p, sizeof(int));

    if ((filename = (char *) malloc(filename_len + 1)) == NULL) {
        kpha_err("Bad malloc filename %dB", filename_len);
        ret = KPHA_ERR_OUT_OF_MEMORY;
        goto err;
    }
    memset(filename, 0, filename_len + 1);

    if (buf_size < (int) (sizeof(int) + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(filename, p, filename_len);

    if (buf_size < (int)(sizeof(int) * 2 + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) * 2 + filename_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(&n_len, p, sizeof(int));

    if (n_len != (int) sizeof(n)) {
        kpha_err("Invalid n_len = %d\n", n_len);
        ret = KPHA_ERR_BAD_PARAMETERS;
        goto err;
    }

    if (buf_size < (int)(sizeof(int) * 2 + filename_len + n_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) * 2 + filename_len + n_len, buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }

    COPY_DATA(&n, p, n_len);

    if ((key_buf = buffer_from_id(build_id_from_name(filename),
                    &key_len)) == NULL) {
        kpha_err("key_buf with filename=%s not exist\n", filename);
        ret = KPHA_ERR_CUSTOM;
        goto err;
    }

    ret = kph_import_key_legacy((uint32_t) n, key_buf, key_len);
    if (ret) {
        kpha_err("import ta key failed with %d\n", ret);
        goto err;
    }

err:
    free(filename);
    return ret;
}

static int parse_buf_and_export_key(char *buf, int buf_size)
{
    int ret = 0;

    char *p = buf;
    int n;
    int n_len;

    char key_name[256];

    file_buffer fbuf;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB",
                sizeof(int), buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_DATA(&n_len, p, sizeof(int));

    if (n_len != (int) sizeof(n)) {
        kpha_err("Invalid n_len = %d\n", n_len);
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size < (int)(sizeof(int) + n_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) + n_len, buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    COPY_DATA(&n, p, n_len);

    ret = kph_export_key_legacy((uint32_t) n, &(fbuf.buf), &(fbuf.length));
    if (ret != 0) {
        kpha_err("Export key failed with %d\n", ret);
        return ret;
    }

    ret = kphh_get_pubkey_filename((uint32_t) n, key_name, sizeof(key_name));
    if (ret != 0) {
        kpha_err("Get pubkey filename failed, the err code is %d\n", ret);
        return ret;
    }

    if (!insert_id_buf(build_id_from_name(key_name), fbuf)) {
        kpha_err("Failed due to duplicated id\n");
        return KPHA_ERR_CUSTOM;
    }

    return ret;
}

static int parse_buf_and_write_file_to_pc(char *buf, int *buf_size)
{
    int filename_len = 0;
    char *filename = NULL;
    int ret = 0;
    char *p = buf;

    uint8_t *data;
    size_t length;

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (*buf_size < (int) sizeof(int)) {
        kpha_err("Expecting buffer larger than %zuB, got %dB", sizeof(int), *buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_DATA(&filename_len, p, sizeof(int));

    if ((filename = (char *) malloc(filename_len + 1)) == NULL) {
        kpha_err("Bad malloc filename %dB", filename_len);
        ret = KPHA_ERR_OUT_OF_MEMORY;
        goto err;
    }
    memset(filename, 0, filename_len + 1);

    if (*buf_size < (int) (sizeof(int) + filename_len)) {
        kpha_err("Expecting buffer over buf_size %zuB, got %dB",
                sizeof(int) + filename_len, *buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }
    COPY_DATA(filename, p, filename_len);
    memset(buf, 0, *buf_size);

    data = buffer_from_id(build_id_from_name(filename),
            &length);

    if (data == NULL) {
        if (strstr(filename, "device.finish") != NULL) {
            buf = NULL;
            *buf_size = 0;
            goto err;
        }
        kpha_err("Data with id=%s not found", filename);
        ret = KPHA_ERR_CUSTOM;
        goto err;
    }

    if (*buf_size < (int) length) {
        kpha_err("Got data with buffer size=%dB with capacity=%zu",
               length, *buf_size);
        ret = KPHA_ERR_SHORT_BUFFER;
        goto err;
    }

    memcpy(buf, data, length);
    *buf_size = length;

err:
    free(filename);
    return ret;
}

static int read_file_from_pc(char *in_buf, int in_buf_size)
{
    int ret = 0;

    if (in_buf == NULL) {
        kpha_err("NULL in_buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (in_buf_size <= 0) {
        kpha_err("Bad in_buf_size: %d", in_buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    if ((ret = parse_buf_and_write_file(in_buf, in_buf_size))) {
        kpha_err("parse_buf_and_write_file failed");
    }

    return ret;
}

static int handle_kph_import_config(char *buf_in, int in_buf_size)
{

    int ret = 0;

    kpha_log("enter");
    if ((ret = parse_buf_and_import_config(buf_in, in_buf_size))) {
        kpha_err("kph import_config failed");
    }
    return ret;
}

static int handle_kph_import_ta_data(char *buf_in, int in_buf_size)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = parse_buf_and_import_ta_data(buf_in, in_buf_size))) {
        kpha_err("parse_buf_and_import_ta_data failed");
    }

    return ret;
}

static int kph_gen_key(char *in_buf, int in_buf_size)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = parse_buf_and_generate_key(in_buf, in_buf_size))) {
        kpha_err("parse_buf_and_generate_key failed");
    }

    return ret;
}

static int handle_kph_import_ta_key(char *in_buf, int in_buf_size)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = parse_buf_and_import_ta_key(in_buf, in_buf_size))) {
        kpha_err("parse_buf_and_import_ta_key failed");
    }

    return ret;
}

static int handle_kph_export_key(char *in_buf, int in_buf_size)
{
    int ret = 0;
    kpha_log("enter");

    if ((ret = parse_buf_and_export_key(in_buf, in_buf_size))) {
        kpha_err("parse_buf_and_export_key failed");
    }

    return ret;
}

static int write_file_to_pc(char *buf_in_out, int *buf_len)
{
    int ret;
    ret = parse_buf_and_write_file_to_pc(buf_in_out, buf_len);
    if (ret < 0) {
        kpha_err("parse_buf_and_write_file_to_pc failed", ret);
    }
    return ret;
}

static int handle_kph_export_device_config(char *buf, uint32_t buf_size)
{
    int ret = 0;

    char dev_name[256];
    uint8_t *dev_buf = NULL;
    size_t dev_len = 0;

    char conf_name[256];
    uint8_t *conf_buf = NULL;
    size_t conf_len = 0;

    kpha_log("enter");

    if (buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (buf_size <= 0) {
        kpha_err("Bad buf_size %d", buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    ret = kph_export_device(&dev_buf, &dev_len);
    if (ret != 0) {
        kpha_err("Export device inf failed, the err code is %d\n", ret);
        return ret;
    }

    ret = kph_export_config(&conf_buf, &conf_len);
    if (ret != 0) {
        kpha_err("Export config inf failed, the err code is %d\n", ret);
        return ret;
    }

    ret = kphh_get_device_filename(dev_name, sizeof(dev_name));
    if (ret != 0) {
        kpha_err("Get device filename failed, the err code is %d\n", ret);
        return ret;
    }
    ret = kphh_get_config_filename(conf_name, sizeof(conf_name));
    if (ret != 0) {
        kpha_err("Get config filename failed, the err code is %d\n", ret);
        return ret;
    }

    if (!insert_id_buf(build_id_from_name(dev_name),
                file_buffer(dev_buf, dev_len))) {
        kpha_err("Failed to duplicate id\n");
        return ret;
    }

    if (!insert_id_buf(build_id_from_name(conf_name),
                file_buffer(conf_buf, conf_len))) {
        kpha_err("Failed to duplicate id\n");
        return ret;
    }

    return 0;
}

/*
 * out_buf format:
 * +-------------------------------------------------------+
 * |  serialno length | serialno | chipid length | chipid  |
 * +-------------------------------------------------------+
 *
 **/
static int get_sn_chipid(char *out_buf, int *out_buf_len)
{
    int ret;
    int size = *out_buf_len;
    int attr_length = 0;
    int used = 0;
    char *tmp_buf = NULL;
    char *p = NULL;
    p = out_buf;

    if (out_buf == NULL) {
        kpha_err("NULL buf");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (out_buf_len == NULL) {
        kpha_err("NULL out_buf_len");
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if ((tmp_buf = (char *) malloc(*out_buf_len)) == NULL) {
        kpha_err("Bad malloc filename %dB", *out_buf_len);
        return KPHA_ERR_OUT_OF_MEMORY;
    }
    memset(tmp_buf, 0, *out_buf_len);

    if ((ret = get_serialno(tmp_buf, size, &attr_length))) {
        kpha_err("get_serialno failed");
        free(tmp_buf);
        return ret;
    }

#define COPY_BUF(dst, sour, len)  \
    do { \
        memcpy(dst, &len, sizeof(int)); \
        dst += sizeof(int); \
        memcpy(dst, sour, len); \
        dst += len; \
    }while(0);

    used += attr_length + sizeof(int);
    if (*out_buf_len < used) {
        kpha_err("Expecting buffer to be larger than %dB, got %dB", used, *out_buf_len);
        free(tmp_buf);
        return KPHA_ERR_SHORT_BUFFER;
    }
    COPY_BUF(p, tmp_buf, attr_length);
    memset(tmp_buf, 0, *out_buf_len);

    if ((ret = get_rid(tmp_buf, size, &attr_length))) {
        kpha_err("get_rid failed");
        free(tmp_buf);
        return ret;
    }

    used += attr_length + sizeof(int);
    if (*out_buf_len < used) {
        kpha_err("Expecting buffer to be larger than %dB, got %dB", used, *out_buf_len);
        free(tmp_buf);
        return KPHA_ERR_SHORT_BUFFER;
    }

    COPY_BUF(p, tmp_buf, attr_length);
    free(tmp_buf);
    *out_buf_len = used;
    return 0;
}

static int check_device_init_status(char *in_buf, int *in_buf_size)
{
    pid_t status = 0;
    int ret = 0;

    if (in_buf == NULL || in_buf_size == NULL) {
        kpha_err("NULL in_buf %p or NULL in_buf_size %p", in_buf, in_buf_size);
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (*in_buf_size < (int) sizeof(int)) {
        kpha_err("Bad in_buf size %d", *in_buf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    ret = runcmd("kph -c check_init_status", in_buf, *in_buf_size);

    if (ret && KPHA_ERR_TYPE(ret) == KPHA_ERR_EXEC_BASE) {
        int cmd_r = KPHA_ERR_SUBERR(ret);
        if (cmd_r < 0) {
            kpha_err("kph check_keystate failed");
            return ret;
        } else {
            /* It's valid for "kph -c check_keystate" to return 1/2 */
            meta_buf_header_t *mbh = reinterpret_cast<meta_buf_header_t *>(in_buf_size);
            dropcmdresp(mbh->bufptr);
        }
    }

    memcpy(in_buf, &ret, sizeof(int));
    *in_buf_size = sizeof(int);

    kpha_log("succeeds");
    return 0;
}

static int get_version(char *inbuf, int *len)
{
    if (*len < 3) {
        return KPHA_ERR_SHORT_BUFFER;
    }

    inbuf[0] = (char) MAJOR_VERSION;
    inbuf[1] = (char) MIDDL_VERSION;
    inbuf[2] = (char) MINOR_VERSION;

    *len = 3;

    return 0;
}

static int wait_for_device(char *buf, int len)
{
    (void) buf;
    (void) len;
    /* currently not implemented */
    return 0;
}

static int get_device_pl_status(char *buf, int *len)
{
    int r, digest_ok;
    uint32_t version, verify, authorize, rpmbvalid, randtoken;
    struct certificate_desc certdesc;

    uint8_t digest[32];
    uint32_t digest_size = sizeof(digest);

    if (*len < 6 * (int) sizeof(uint32_t) + 32) {
        return KPHA_ERR_SHORT_BUFFER;
    }

    if ((r = pl_device_get_cert_info(&certdesc, NULL))) {
        kpha_err("pl_device_get_cert_info failed with %d", r);
        return KPHA_ERR_POSIX(r);
    }

    if ((r = pl_device_get_status(&version, &verify, &authorize,
        &rpmbvalid, &randtoken, NULL))) {
        kpha_err("pl_device_get_status failed with %d", r);
        return KPHA_ERR_POSIX(r);
    }

    if ((digest_ok = pl_device_get_cert_digest(digest, &digest_size))) {
        kpha_err("Get cert_digest failed with 0x%x", r);
    }

    {
        char *p = buf;

        memcpy(p, &version, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &verify, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &rpmbvalid, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &randtoken, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &certdesc.version, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &certdesc.cert_type, sizeof(uint32_t));
        p += sizeof(uint32_t);

        if (digest_ok == 0) {
            /* Additionally copy cert digest if
               it is available */
            memcpy(p, digest, digest_size);
            p += digest_size;
        }

        *len = (int) (p - buf);
    }

    return 0;
}

static int get_device_truststore(char *buf, int *len)
{
    int r;
    uint32_t type, status, version = 1U;

    if (*len < 3 * (int) sizeof(uint32_t)) {
        return KPHA_ERR_SHORT_BUFFER;
    }

    if ((r = pl_device_get_truststore(&type, &status, &version))) {
        kpha_err("pl_device_get_truststore failed with %d", r);
        return KPHA_ERR_POSIX(r);
    }

    {
        char *p = buf;

        memcpy(p, &version, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &type, sizeof(uint32_t));
        p += sizeof(uint32_t);

        memcpy(p, &status, sizeof(uint32_t));
        p += sizeof(uint32_t);

        *len = (int) (p - buf);
    }

    return 0;
}

static int install_keybox(char *inbuf, uint32_t len)
{
	if (inbuf == NULL) {
		kpha_err("bad parameters, inbuf is NULL\n");
		return -1;
	}

	kpha_log("[TrustKernel]: keybox len : %d\n", len);
	return kph_import_keybox((unsigned char *)inbuf, len);
}


/* [ certbuf size ] [ hash size] [ certbuf ] [ hash ] */
static int program_cert(char *inbuf, int *len)
{
    int r;
    uint32_t cert_type;
    uint32_t certbuf_size, hashbuf_size;
    char *certbuf, *hashbuf;

    if (*len < 8) {
        kpha_err("got inbuf len=%d", *len);
        return KPHA_ERR_SHORT_BUFFER;
    }

    memcpy(&certbuf_size, inbuf, sizeof(uint32_t));

    memcpy(&hashbuf_size, inbuf + sizeof(uint32_t),
        sizeof(uint32_t));

    if ((uint32_t) (*len - 8) < certbuf_size) {
        kpha_err("got inbfu len=%d, expecting certbuf_size=%u", *len, certbuf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    if ((uint32_t) (*len - 8) - certbuf_size < hashbuf_size) {
        kpha_err("got inbfu len=%d, expecting certbuf_size=%u hashbuf_size=%u", *len, certbuf_size, hashbuf_size);
        return KPHA_ERR_SHORT_BUFFER;
    }

    certbuf = inbuf + 8;
    hashbuf = inbuf + 8 + certbuf_size;

    if ((r = pl_device_program_cert(certbuf, certbuf_size, hashbuf, hashbuf_size, &cert_type))) {
        kpha_err("program cert failed with %d. certbuf_size=%u hashbuf_size=%u", r, certbuf_size, hashbuf_size);
        return KPHA_ERR_POSIX(r);
    }

    memcpy(inbuf, &cert_type, sizeof(uint32_t));
    *len = sizeof(uint32_t);

    kpha_log("succeeds");

    return 0;
}

static int revoke_cert(char *inbuf, int len)
{
    int r;
    if ((r = pl_device_revoke_cert(inbuf, len))) {
        kpha_err("revoke cert failed with %d.", r);
        return KPHA_ERR_POSIX(r);
    }

    kpha_log("succeeds");
    return 0;
}

static int valid_cmd_buf(char *cmdbuf)
{
    uint32_t i;
    const char *candidate_cmd[] = { "kph ", "pld ", "getprop " };

    for (i = 0; i < sizeof(candidate_cmd) / sizeof(candidate_cmd[0]); i++) {
        if (strncmp(cmdbuf, candidate_cmd[i], strlen(candidate_cmd[i])) == 0)
            return true;
    }

    return false;
}

static int get_prop(const char *propname, char *respbuf, size_t len)
{
    int r;
    char linebuf[PROP_VALUE_MAX];

    uint64_t buf_ptr = 0ULL;
    uint32_t buf_size = 0;
    char *s_resp;

    meta_buf_header_t *mbh;

    if (len < sizeof(meta_buf_header_t)) {
        kpha_err("Invalid meta response buffer size: %u", len);
        return KPHA_ERR_SHORT_BUFFER;
    }

    if ((r =  __system_property_get(propname, linebuf)) < 0) {
        kpha_err("Failed to getprop(%s) with %d", propname, r);
        return KPHA_ERR_POSIX(r);
    }

    if ((s_resp = strdup(linebuf)) == NULL) {
        kpha_err("dup string length=%zu failed", r);
    } else {
        buf_ptr = reinterpret_cast<uint64_t>(s_resp);
        buf_size = static_cast<uint32_t>(r + 1);
        cmd_resps.insert(std::unordered_map<uint64_t, uint32_t>
            ::value_type(buf_ptr, buf_size));
    }

    mbh = reinterpret_cast<meta_buf_header_t *>(respbuf);

    mbh->bufsize = buf_size;
    mbh->bufptr = buf_ptr;
    mbh->ret = 0;

    return 0;
}

static int generic_execute(char *inbuf, int *len)
{
    int r;
    char *p;

    if (*len <= 1) {
        kpha_err("Invalid buffer content. Unexpected cmdlen: %d", *len);
        return KPHA_ERR_SHORT_BUFFER;
    }

    /* avoid out of bound */
    inbuf[*len - 1] = '\0';

    if (!valid_cmd_buf(inbuf)) {
        kpha_err("Invalid command buffer %s", inbuf);
        return KPHA_ERR_BAD_PARAMETERS;
    }

    if (strncmp(inbuf, "getprop", strlen("getprop")) == 0) {
        p = inbuf + strlen("getprop");

        while (*p && isspace(*p))
            ++p;

        if (*p == '\0') {
            kpha_err("Empty propname");
            return KPHA_ERR_BAD_PARAMETERS;
        }

        r = get_prop(p, inbuf, *len);
    } else {
        r = runcmd(inbuf, inbuf, *len, true);
    }

    if (r)
        return r;

    *len = sizeof(meta_buf_header_t);
    return 0;
}

/*
 * flag is 0: poweroff
 * flag is 1: reboot
 */
static int shutdown_device(char *in_buf, int in_len)
{
    int flag = -1;

    if (in_len < (int) sizeof(int)) {
        return KPHA_ERR_SHORT_BUFFER;
    }

    memcpy(&flag, in_buf, sizeof(int));

    if (flag) {
        system("reboot");
    } else {
        system("reboot -p");
    }
    return 0;
}

static int wait_for_teed(void)
{
    int r;
    struct certificate_desc certdesc;

    for (;;) {
        r = pl_device_get_cert_info(&certdesc, NULL);
        if (r) {
            /* retry if we fails to get device info */
            sleep(1);
            continue;
        }
        break;
    }

    return 0;
}

/*FIXME several customer op use strlen(buf) instead of size as the size of buffer */
/*FIXME check several handlers which need to update the size of return buffer */
/*FIXME check several handlers which need to check the whether return value is greater than zero,
 *      instead of != zero */

int handle_trustkernel_meta_command(unsigned char cmd, char **pbuf,int *psize, unsigned char *r)
{
    int ret;
    int size = *psize;
    char *buf = *pbuf;

    kpha_log("%s: cmd=0x%02x", meta_version, cmd);

    switch(cmd) {
        case KPHA_GET_VERSION:
            CALL_META_FUNC_ARGS(get_version, buf, &size);
            break;

        case KPHA_RECV_FILE:
            CALL_META_FUNC_ARGS(read_file_from_pc, buf, size);
            break;

        case KPHA_IMPORT_CFG_FILE:
            CALL_META_FUNC_ARGS(handle_kph_import_config, buf, size);
            break;

        case KPHA_IMPORT_TA_DATA:
            CALL_META_FUNC_ARGS(handle_kph_import_ta_data, buf, size);
            break;

        case KPHA_IMPORT_TA_KEY:
            CALL_META_FUNC_ARGS(handle_kph_import_ta_key, buf, size);
            break;

        case KPHA_GEN_KEY:
            CALL_META_FUNC_ARGS(kph_gen_key, buf, size);
            break;

        case KPHA_EXPORT_KEY:
            CALL_META_FUNC_ARGS(handle_kph_export_key, buf, size);
            break;

        case KPHA_EXPORT_DEV_INF:
            CALL_META_FUNC_ARGS(handle_kph_export_device_config, buf, size);
            break;

        case KPHA_GET_SN_CHIPID:
            CALL_META_FUNC_ARGS(get_sn_chipid, buf, &size);
            break;

        case KPHA_SEND_FILE:
            CALL_META_FUNC_ARGS(write_file_to_pc, buf, &size);
            break;

        case KPHA_PROGRAM_CERT:
            CALL_META_FUNC_ARGS(program_cert, buf, &size);
            break;

        case KPHA_REVOKE_CERT:
            CALL_META_FUNC_ARGS(revoke_cert, buf, size);
            break;

        case KPHA_GENERIC_EXECUTE_CMD:
            CALL_META_FUNC_ARGS(generic_execute, buf, &size);
            break;

        case KPHA_GET_PL_STATUS:
            CALL_META_FUNC_ARGS(get_device_pl_status, buf, &size);
            break;

        case KPHA_GET_TRUSTSTORE:
            CALL_META_FUNC_ARGS(get_device_truststore, buf, &size);
            break;

        case KPHA_WAIT_FOR_DEVICE:
            CALL_META_FUNC_ARGS(wait_for_device, buf, size);
            break;

        case KPHA_SHUTDOWN_DEV:
            CALL_META_FUNC_ARGS(shutdown_device, buf, size);
            break;

		case KPHA_INSTALL_KEYBOX:
			CALL_META_FUNC_ARGS(install_keybox, buf, size);
			break;

		case KPHA_READ_CMD_RESPONSE:
            {
                int capacity;
                char *respbuf;

                capacity = size < (int) sizeof(meta_buf_header_t) ?
                    (int) sizeof(meta_buf_header_t) : size;

                if (size < capacity) {
                    if ((buf = (char *) malloc(capacity)) == NULL) {
                        /* there is no way we can set error value */
                        return -1;
                    }
                    memcpy(buf, *pbuf, size);
                    free(*pbuf);
                    *pbuf = buf;
                }

                ret = readcmdresp(buf, &size, &respbuf);

                if (ret) {
                    *psize = sizeof(meta_buf_header_t);
                    *r = KPHA_META_RESULT_FAILED;
                    FILL_META_BUF_RETURN_VALUE(buf, ret);
                } else {
                    *pbuf = respbuf;
                    *psize = size;
                    *r = KPHA_META_RESULT_SUCCESS;
                    free(buf);
                }
            }

            break;
        default:
/*            *r = KPHA_META_RESULT_BAD_COMMAND; */
            kpha_log("Unkown COMMAND 0x%x", cmd);
            return 1;
    }

    kpha_log("return=%u", *r);

    return 0;
}
