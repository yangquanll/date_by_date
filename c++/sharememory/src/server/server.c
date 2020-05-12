#include "yq.h"
#include <stdio.h>


int main()
{
	int shmid = CreatShm(4666);

	char *addr = shmat(shmid,NULL,0);
	sleep(2);
	int i = 0;
	while(i++ < 26)
	{
		printf("client# %s\n",addr);
		sleep(1);
	}
	shmdt(addr);
	sleep(2);
	DestroyShm(shmid);
	return 0;
}
