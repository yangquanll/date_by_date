#include"comm.h"

static int CommShm(int size,int flags)
{
	key_t key = ftok(PATHNAME,PROJ_ID);
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}
	int shmid = 0;
	if((shmid = shmget(key,size,flags)) < 0)
	{
		perror("shmget");
		return -2;
	}
	return shmid;
}

static int DestroyShm(int shmid)
{
	if(shmctl(shmid,IPC_RMID,NULL) < 0)
	{
		perror("IPC_RMIDshmctl");
		return -1;
	}
	return 0;
}

static int CreateShm(int size)
{
	return CommShm(size,IPC_CREAT | IPC_EXCL | 0666);
}

static int GetShm(int size)
{
	return CommShm(size,IPC_CREAT);
}

void deg()
{
	printf("link ok \n");


/*
int main()
{

}
*/
