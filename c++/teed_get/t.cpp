#include <iostream>
#include <string.h>
#include <stdio.h>
#include <linux/prctl.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

#define CONFIG_TEED_ENV(var, size, value) \
    do { if (value && snprintf((var), (size), "%s", value) == (size)) { \
	        printf(  "[%d] cannot hold \"%s\"\n", (size), value); exit(252); } \
	   } while(0)



void *tst_tread()
{
	int ret;
	if((ret = prctl(PR_SET_NAME,(unsigned long)"YQQQ",0,0,0)) < 0)
	{
		cout<< "error prctl set" <<ret<<endl;
	}
}

void *tst_get_thread_name()
{
	int ret;
	char *thread_name;
	if((ret = prctl(PR_GET_NAME,(unsigned long)thread_name,0,0,0)) < 0)
	{
		cout<< "error prctl get "<<ret<<endl;
	}

}

int main()
{
	int i = 1u<< 12;
	int ret = 0;
	cout<< "1 << 12 = "<<i<<endl;
	char *prebuilt_path_arg = "/vendor/app/t6/data/";
    char obj[1024];
	char *thread_name;
	CONFIG_TEED_ENV(obj,i, prebuilt_path_arg);
	cout <<obj<<endl;
	
	pthread_t tst_pthr;
	sem_t sem;

	sem_init(&sem,0,0);

	int t = -1;

	t = (i >= 10);
	cout << "juge "<<t<<endl;

	while(1)	
	{


	}
}
