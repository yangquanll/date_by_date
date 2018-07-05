#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PERM 0666|IPC_CREAT

int main(int argc, char **argv)
{
	int shmid;
	char *f_add,*p_add;
	
	if(argc != 2)
	{
		printf("plase input add :%s\n",argv[0]);
		exit(1);
	}
   if((shmid = shmget(IPC_PRIVATE,1024,PERM))== -1)
   {
		printf("creat error \n");
		return -1;
   }
printf("shmid = %d \n",shmid);
if(fork())
{
	f_add = shmat(shmid, 0,0);
	printf("f_add= %p \n",f_add);
	memset(f_add,0,sizeof(f_add));
	memcpy(f_add,argv[1],1024);
	exit(0);
}
else
{
	sleep(0);
	p_add =shmat(shmid,0,0);
	printf("add = %p \n",p_add);

}
//char *mid,*cmd;
char cmd[80]={0};
//memcyp();
sprintf(cmd,"ipcs | grep %d",shmid);
printf("cmd = %s \n",cmd);
system(cmd);
shmctl(shmid, IPC_RMID,0);
}
