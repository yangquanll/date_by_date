#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
 
#include "shm.h"
 
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
 
 
int main(int argc, const char *argv[])
{
    int semid, shmid;
    key_t semkey;
    key_t shmkey;
    struct people *addr = NULL;
    //获取ipc key
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
    //获取semid和shmid
    semid = semget(semkey, 0, 0666);
    if (semid < 0) {
        fprintf(stderr, "semget error");
        return -1;
    }
    shmid = shmget(shmkey, 0, 0666);
    if (shmid < 0) {
        fprintf(stderr, "shmget error");
        return -1;
    }
    //将共享内存链接到进程内存
    addr = (struct people*)shmat(shmid, 0, 0);
    if (addr == (struct people *)-1) {
        fprintf(stderr, "shmat error");
        return -1;
    }
 
    v(semid, 0);//取消server的阻塞
    sleep(1);
 
    p(semid, 0);//获取共享内存内容
    printf("name is : %s\n", addr->name);
    printf("age is : %d\n", addr->age);
    v(semid, 0);
 
    if (shmdt(addr) == -1) {//断开共享内存
        fprintf(stderr, "shmdt is error");
        return -1;
    }
 
 
    return 0;
}
