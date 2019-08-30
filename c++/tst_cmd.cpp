#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
//#include <erron.h>

using namespace std;

#if 0
static int __runcmd(const char *p, char *meta_resp, size_t meta_resp_size, int *r, bool raw)
{
    FILE *fp;

    int size;
    char linebuf[1000];

    std::string resp;
    if (!raw) {
        resp.assign(p);
        resp += ":\n";
    }

    char *s_resp;

    if (p == NULL || meta_resp == NULL || r == NULL) {
        kpha_err("NULL cmd buf");
    }

    if (meta_resp == NULL) {
        kpha_err("NULL meta_resp buf");
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

/*
    if ((fp = popen(p, "r")) == NULL) {
        kpha_err("popen(%s) failed with %s(%d)", p, strerror(errno), errno);
        return KPHA_ERR_POSIX(errno);
    }
*/
    while (fgets(linebuf, LOG_BUF_MAX_LENGTH, fp)) {
        resp += linebuf;
    }

    *r = pclose(fp);

    if (*r == -1) {
        kpha_err("pclose(%s) failed with %s(%d)", p, strerror(errno), errno);
        return KPHA_ERR_POSIX(errno);
    }

    if (raw || *r != 0) {
        uint64_t buf_ptr = 0ULL;
        uint32_t buf_size = static_cast<uint32_t>(resp.length() + 1);
        meta_buf_header_t *mbh = reinterpret_cast<meta_buf_header_t *>(meta_resp);

        if (*r)
            *r = WEXITSTATUS(*r);

        kpha_log("pclose(%s) exited with %d", p, *r);

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
#endif

int main (int argc,char* argv[])
{
	FILE *fp;
	string resp;
	char linbuf[1000];

	if(argc > 0)
	{
		if((fp = popen(argv[1],"r")) == NULL)
		{
			cout<< argv[1]<<endl;
	   //strerror(errno)<<erron<<endl;
		}
	}
	while(fgets(linbuf,1000,fp))
	{
		resp =+ linbuf;
	}
	cout<< "resp = "<<resp<<endl;
   if( -1 == pclose(fp))//注意只能用这个 pclose 关闭
   {
	   cout <<"pclose failed"<<endl;
   }
}
