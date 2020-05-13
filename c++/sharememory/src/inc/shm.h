#ifndef __SHM_H__
#define __SHM_H__

struct people {
	char name[10];
	int age;
};

union semun {
	int val; // value for SETVAL
	struct semid_ds *semid_dsbuf; // buffer for IPC_STAT, IPC_SET
	unsigned short *array; // array foror GETALL, SETALL
	struct seminfo *__buf; // buffer for IPC_INFO
};

#define SHM_KEY_FILE "./client.c"
#define SEM_KEY_FILE "./server.c"
 
#endif
