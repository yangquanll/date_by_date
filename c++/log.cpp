/***
* 一般的软件工程中，日志的作用都非常大，下面介绍一种c/c++代码生成日志的方法
* 支持带有时间戳；写入文件并不造成覆盖；支持多线程，进程
* 封装成 3种格式：debug,error,inf,三种输出
* 自定义 log文件的位置
* 自定义定义log文件的大小 超过则删除
*
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
#define PATH_LOG  "./log"

Log log;

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



int main()
{
	
        log.gettime();
	return 0;		
}
