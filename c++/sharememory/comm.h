//comm.h
#ifndef _COMM_H__
#define _COMM_H__

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

static int CreateShm(int size);
static int DestroyShm(int shmid);
static int GetShm(int size);

#endif
