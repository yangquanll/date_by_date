#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <signal.h>

#include "shm.h"

int semid, shmid;

int p(int semid, int semnum)
{
	struct sembuf sops = {semnum, -1, SEM_UNDO};
	return semop(semid, &sops, 1);
}

int v(int semid, int semnum)
{
	struct sembuf sops = {semnum, +1, SEM_UNDO};
	return semop(semid, &sops, 1);
}

void * signal_set(int signo, void (*func)(int))
{
	int ret;
	struct sigaction sig;
	struct sigaction osig;

	sig.sa_handler = func;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
#ifdef SA_RESTART
	sig.sa_flags |= SA_RESTART;
#endif
	ret = sigaction(signo, &sig, &osig);

	if (ret < 0) 
		return SIG_ERR;
	else 
		return osig.sa_handler;
}

void sigint(int sig)
{
	union semun arg;
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "shmctl delete error");
	}
	if (semctl(semid, 0, IPC_RMID, 0) == -1) {
		fprintf(stderr, "semctl delete error");
	}
	exit(sig);
}

int main(int argc, const char *argv[])
{
	key_t semkey;
	key_t shmkey;
	struct people *addr = NULL;
	union semun arg;

	signal_set(SIGINT, sigint);
	signal_set(SIGTERM, sigint);

	semkey = ftok(SEM_KEY_FILE, 0);
	if (semkey < 0) {
		fprintf(stderr, "ftok semkey error");
		return -1;
	}
	shmkey = ftok(SHM_KEY_FILE, 0);
	if (shmkey < 0) {
		fprintf(stderr, "ftok shmkey error");
		return -1;
	}

	semid = semget(semkey, 1, 0666 | IPC_CREAT);
	if (semid < 0) {
		fprintf(stderr, "semget error");
		return -1;
	}
	shmid = shmget(shmkey, sizeof(struct people), 0666 | IPC_CREAT);
	if (shmid < 0) {
		fprintf(stderr, "shmget error");
		goto err1;
	}

	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) < 0) {
		fprintf(stderr, "ctl sem error");
		goto err2;
	}
	addr = (struct people *)shmat(shmid, NULL, 0);
	if (addr == (struct people *)-1) {
		fprintf(stderr, "shmat error");
		goto err2;
	}

	p(semid, 0);
	strncpy(addr->name, "xiaoming", 10);
	addr->age = 10;
	v(semid, 0);

	if (shmdt(addr) == -1) {
		fprintf(stderr, "shmdt is error");
		goto err2;
	}

	p(semid, 0);

err2:
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "shmctl delete error");
	}
err1:
	if (semctl(semid, 0, IPC_RMID, 0) == -1) {
		fprintf(stderr, "semctl delete error");
	}

	return 0;
}

