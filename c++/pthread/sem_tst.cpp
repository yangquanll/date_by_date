/* review sem for pthread && pthread_mutex
 * c++ virtual function use
 * pthread_mutex 使用场景：多线程操作global变量&&share function时（互斥）
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h> //sem add must
#include <fstream>
#include <stdlib.h>
#include <string.h>
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

using namespace std;

sem_t sem;
int  fd;
pthread_mutex_t log_lock;
string path ="./sm.log";
void s_thread_call(string msg, ...);
//两个线程都有result
void* product(void  *arg)
{
	int i = 0;
	char *tp = (char*)arg; //强制转换
	cout<< "fuct_par1 =  "<< tp <<endl;
	while(1)
	{
		sem_wait(&sem);
		//pthread_mutex_lock(&log_lock);
		string goods ="product+1";
		s_thread_call(goods);
		//sleep(1);//作用是 如果不加的话 另一个线程没有机会 invoke s_thread_call
		sem_post(&sem);
		//pthread_mutex_unlock(&log_lock); 
	}

}

void* customer(void  *arg)
{
	int i = 0;
	char *tp = (char*)arg;
	cout<< "fuct_par2 =  "<< tp <<endl;
	while(1)
	{;	
		sem_wait(&sem);
		//pthread_mutex_lock(&log_lock); // 作用是 多线程时只允许 一个线程访问 这段代码
		string goods ="customer-1";
		s_thread_call(goods);
		//sleep(1);
		sem_post(&sem);
		//pthread_mutex_unlock(&log_lock); //unlock 后 log_lock 值为零
		
	}

}

void gettime(char *buf)
{
        struct tm T;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &T);
        int millisecond = tv.tv_usec/1000;

        sprintf(buf, "%02d-%02d-%02d %02d:%02d:%02d %03d - ", T.tm_year+1900,T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);
        //printf("%02d-%02d-%02d %02d:%02d:%02d %03d -  \n", T.tm_year+1900,T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);

}
void opend_files(string file_name)
{
	fd = open(file_name.data(), O_RDWR|O_CREAT|O_APPEND, 0755);
}

void write_files(const char *ms,int len)
{
        ssize_t  bw;//有符号整型
		cout<<"ms ="<<ms<<endl;
        bw = write(fd, ms, len);
}

void s_thread_call(string msg, ...)
{
	char cmdbuf[256];
	const char* tmp = msg.data();
	
	opend_files(path);
    va_list ap;
	va_start(ap,tmp);
	
	char buf[500] = {0};
	char timeStr[100] = {0};
	gettime(timeStr);
    memcpy(buf, timeStr, strlen(timeStr));
	
	int len = strlen(buf);
	len +=vsprintf(buf+len, tmp, ap);
	va_end(ap);

	if (buf[len -1] != '\n')
	{
		buf[len] = '\n';
		len++;
	}
	
	write_files(buf,len);
	cout<<"msgok ="<<tmp<<endl;
}

int main(int argc ,char* argv[])
{
	
	sem_init(&sem,0,2);
	char* msg1 = "thread 1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	char* msg2 = "thread 2 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	
	pthread_t td1,td2;
	int ret1 = -1 ,ret2 = -1;
	ret2 = pthread_create(&td2,NULL,product,msg2);
	ret1 = pthread_create(&td1,NULL,customer,msg1);
	
	if(ret1 && ret2)
	{
		cout<<"creat phtread failed "<<endl;
		return false;
	}
	
	ret2 = pthread_join(td2,NULL);
	ret1 = pthread_join(td1,NULL);

	if(ret1 && ret2)
	{
		cout<<"join phtread failed "<<endl;
		return false;
	}
	
	sem_destroy(&sem);
	return 0;
}

