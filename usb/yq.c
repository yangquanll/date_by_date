#include<stdio.h>
#include "devmanager.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 

/*
void printids(const char *s0)
{

	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x0\n", s, unsigned int0pid,unsigned int0tid, unsigned int0tid0);
}
*/
/*
void *func(void *arg)
{
    printf("arg=%d\n", *(int *)arg);
   // pthread_exit("bye bye");
}

int main()
{
    int res;
    pthread_t first_thread;
    int share_int = 10;
    res = pthread_create(&first_thread, NULL, func, (void *)&share_int);
    printf("res=%d\n", res);
    
    void *returnValue;
    res = pthread_join(first_thread, &returnValue);
    printf("returnValue=%s\n", (char *)returnValue);
    return 0;
*/
void *usb_dev(void *arg)
{
	printf("pthread fuc \n");
	int ret = searchSdcard();
	if(!ret)
	{
		printf("get dev faile \n");
	}
}

int main()	

{
	pid_t pid;
	pthread_t tid;
	int ret;
	int msg = 1;
	void *rety;
	ret = pthread_create(&tid,NULL,usb_dev,(void *)&msg);
	if( !ret)
	{
		printf("creat_thread filed \n");
	}

	int tmp = pthread_join(tid, &rety);
	printf("join = %d\n",tmp);
	printf(" pid is: %d, tid is: %d\n", getpid(),pthread_self());
}


