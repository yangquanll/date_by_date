#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 0x6666 


int CreateShm(int size);
int DestroyShm(int shmid);
int GetShmint(int size);

void tst();

