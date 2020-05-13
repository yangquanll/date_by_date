#include "yq.h"
#include "struct_fuc.c"
#include <stdio.h>


int main()
{
	int shmid = CreatShm(4666);
	struct parm pm;
	char *addr = shmat(shmid,NULL,0);
	sleep(2);
	int i = 0;
#if 0
	while(i++ < 26)
	{
		printf("client# %s\n",addr);
		sleep(1);
	}
#endif

	shmdt(&pm);
	printf(" pa.a = %d pa.b = %d pa.buf = %s\n",pm.a, pm.b, pm.buf);
	sleep(2);
	DestroyShm(shmid);
	return 0;
}
