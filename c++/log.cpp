/***
* 一般的软件工程中，日志的作用都非常大，下面介绍一种c/c++代码生成日志的方法
* 支持带有时间戳；写入文件并不造成覆盖；支持多线程，进程
* 封装成 3种格式：debug,error,inf,三种输出
* 自定义 log文件的位置
* 自定义定义log文件的大小 超过则删除
* access()
    F_OK 值为0，判断文件是否存在
    R_OK 值为4，判断对文件是否有读权限
    W_OK 值为2，判断对文件是否有写权限
    X_OK 值为1，判断对文件是否有执行权限

* ::close() 是 global 函数 在某个类里面invoke
***/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <iostream>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include "log.h"

#define TIMS_STAMP   1
#define WRITE_FILES  1
#define OPEN_FILES   1

#define BASE_PATH_LOG  "./log"

Log log;
#if OPEN_FILES
ssize_t ts;
string g_oper;
int g_i;


#define CPRINT(log_type, str, args...) do { string format; format.append("%s,%s,").append(str);   \
               switch(log_type)    \
               {   \
               case DEBUG_LOG: log.Cout(DEBUG_LOG, format.c_str(), "[DBG]", "[MD]",## args);break;  \
               case OPERATE_LOG: log.Cout(OPERATE_LOG, format.c_str(), "[OPR]", "[MD]", ## args);break;   \
               case ERROR_LOG: log.Cout(ERROR_LOG, format.c_str(), "[ERR]", "[MD]", ## args);break;   \
               default :   log.Cout(DEBUG_LOG, format.c_str(), "[DBG]", "[MD]", ## args);break;  \
               }}while(0)

pthread_t threadId;

int Log:: open_files(int type)
{
        string  log_error;
        log_error.append(log_base_path).append("./log_err.txt");
        string  log_warn;
        log_warn.append(log_base_path).append("/log_war.txt");
        string  log_oper;
        log_oper.append(log_base_path).append("./log_oper.txt");
        string file_name;
        int fd = -1;
        
        switch( type)
        {
                case DEBUG_LOG: file_name = log_warn; break;
                case OPERATE_LOG: file_name = log_oper; break;
                case ERROR_LOG:  file_name = log_error; break;
                default: file_name = log_warn;  break;
        }
        fd = open(file_name.data(), O_RDWR|O_CREAT|O_APPEND, 0755);
        
        cout << file_name.data() << endl;
        
        return fd;
}
#endif
#if WRITE_FILES
void Log:: write_files(int log_type, const char *ms,int len)
{
        if(m_Fd == -1)
            return;
        ssize_t  bw;//有符号整型
        bw = ::write(m_Fd, ms, len);
        ts = bw;
        cout<< "ms =" <<ms<<endl;
        if(bw == -1 )
        {
            int err;
            err = errno;
            close(m_Fd);
            m_Fd = open_files(log_type);
		    if(m_Fd != -1)
		    {
		    	char errBuf[100];
		    	sprintf(errBuf, "Write log file error: errno = %d %s", err, strerror(err));
		    	int num = ::write(m_Fd, errBuf, sizeof(errBuf));
		    	if (num == -1)
		    	{
		    		cout<<"[Log] System ERROR: write error."<<endl;
		    		close(m_Fd);
		    		m_Fd = -1;
		    	}
		    }
            
        }
}
#endif

#if TIMS_STAMP  //time stamp get system time
string Log::gettime()
{
        char buf[100];

        gettime(buf);
        return string(buf);
}

void Log::gettime(char *buf)
{
        struct tm T;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &T);
        int millisecond = tv.tv_usec/1000;

        sprintf(buf, "%02d-%02d-%02d %02d:%02d:%02d %03d - ", T.tm_year+1900,T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);
        //printf("%02d-%02d-%02d %02d:%02d:%02d %03d -  \n", T.tm_year+1900,T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);

}
#endif
void Log::run(char *log_path)
{
    log_base_path.clear();
	log_base_path.append(log_path);
	char cmdbuf[256];
	sprintf(cmdbuf, "mkdir -p %s", log_path);
	system(cmdbuf);
	m_Fd = open_files(DEBUG_LOG);    
}

void Log:: Cout(int log_type, const char* fmt, ...)
{
    
    int re = pthread_mutex_lock(&log_lock);
    //cout<<"Cout i ===  log_lock == "<<g_i++<<re<<endl;
   // pthread_mutex_unlock(&log_lock);
	va_list ap;
	va_start(ap, fmt);
    //cout<<"fmt = "<<fmt<<endl;
	char buf[500] = {0};
	char timeStr[100] = {0};
	gettime(timeStr);
    memcpy(buf, timeStr, strlen(timeStr));
	int len = strlen(buf);
        //cout<<"len befor = "<<len<<endl;
        //string tmp;
        //tmp = buf;
        //tmp.append(fmt).append("\n");
        //cout<< "data =  "<<tmp.data()<< "  length = "<<tmp.length()<<endl;//length()
        //int len = tmp.length();
	len +=vsprintf(buf+len, fmt, ap);
	va_end(ap);

	if (buf[len -1] != '\n')
	{
		buf[len] = '\n';
		len++;
	}
	write_files(log_type,buf, len);
    pthread_mutex_unlock(&log_lock);
	if(log_type != DEBUG_LOG)
		 write_files(log_type, buf, len);
    
}

struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    char     *argv_string;      /* From command-line argument */
};

void * Log:: tst_log1(void* arg)
{   
     int i;
     pthread_t threadID = pthread_self();
     cout<< "threadID1 = "<<threadID <<endl;
        while(1)
        {
                
                i++;
                //cout<< i<<endl;
                pthread_mutex_lock(&log_lock);
                while(g_i > 10)
                {
                  printf("tst_log1 %d等待。。\n", g_i);
                  pthread_cond_wait(&cond, &log_lock);
                  printf("tst_log1 %d产品到了！！\n", g_i);
                 }
                g_oper.append(" tst_log1");
                cout<<"tst_log1 = %s " <<g_oper.data()<<endl;
                CPRINT(DEBUG_LOG,"threadID1 =  %d , i-->1 = %d ,g_oper = %s ",threadID,i,g_oper.data());
                pthread_mutex_unlock(&log_lock);
                //if(ts < 0)
                        //break;
            //usleep(2000 * 100);
        }    
}
void * Log:: tst_log2(void* arg)
{
    int i;
     pthread_t threadID = pthread_self();
     cout<< "threadID2 = "<<threadID <<endl;
    while(1)
        {
               i++;
               //cout<< i<<endl;
               pthread_mutex_lock(&log_lock); 
                while(g_i > 10)
                {
                  printf("tst_log2 %d等待。。\n", g_i);
                  pthread_cond_wait(&cond, &log_lock);
                  printf("tst_log2 %d产品到了！！\n", g_i);
                 }
               g_i++;
               g_oper = " tst_log2";
               cout<<"tst_log2 = %s " <<g_oper.data()<<endl;
               CPRINT(OPERATE_LOG,"threadID2 =  %d , i-->2 = %d ,g_oper = %s ",threadID,i,g_oper.data());
               pthread_mutex_unlock(&log_lock);
               usleep(2000 * 100);
               //sleep(0.5);
                //if(ts < 0)
                        //break;
        }
}
void Log:: excute()
{
        thread_info *ti;
        void *tret;
        int ret1,ret2;
        pthread_t thread_id1 ,thread_id2;
        pthread_create(&thread_id1,NULL,tst_log1,&ret1);

        pthread_create(&thread_id2,NULL,tst_log2,NULL);
        pthread_join(thread_id1,&tret);
        pthread_join(thread_id2,&tret);
    
}

int main()
{
        int i = 0;
        log.run(BASE_PATH_LOG);
        log.excute();
	    return 0;		
}
