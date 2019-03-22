/***
* 一般的软件工程中，日志的作用都非常大，下面介绍一种c/c++代码生成日志的方法
* 支持带有时间戳；写入文件并不造成覆盖；支持多线程，进程
* 封装成 3种格式：debug,error,inf,三种输出
* 自定义 log文件的位置
* 自定义定义log文件的大小 超过则删除
*
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
#include "log.h"

#define TIMS_STAMP   1
#define WRITE_FILES  1
#define OPEN_FILES   1

#define BASE_PATH_LOG  "./log"

Log log;
#if OPEN_FILES

void Log:: open_files(int type)
{
        string  file_path;
        string  file_name;
        string  log_error;
        log_error.append()
        string  log_warn;
        string  log_oper;
        
        
}
#endif
#if WRITE_FILES
void Log:: writer_files()
{


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

        sprintf(buf, "%02d-%02d %02d:%02d:%02d %03d - ", T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);
        printf("%02d-%02d-%02d %02d:%02d:%02d %03d -  \n", T.tm_year+1900,T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);

}
#endif
void Log::run(char *buf)
{
        log_base_path.clear();
	log_base_path.append(log_path);

	char cmdbuf[256];
	sprintf(cmdbuf, "mkdir -p %s", log_path);
	system(cmdbuf);

	mLogFile = openLogFile(DEBUG_LOG);
        log.open_files(int t);
        
}



int main()
{
	
	log.gettime();
        log.run(BASE_PATH_LOG);
	return 0;		
}
