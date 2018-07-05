#include <stdio.h>
#include <unistd.h>	// getpagesize()
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define SHARE_MEMORY_KEY 674
int main() {
	// 获取系统中页面的大小，
	printf("page size = %d\n", getpagesize());

	int shareMemoryId, ret;
	//　创建页面大小的共享内存区段
	shareMemoryId = shmget(SHARE_MEMORY_KEY, getpagesize(), 0666|IPC_CREAT);

	if (shareMemoryId > 0)
		printf("Create a shared memory segment %d\n", shareMemoryId);

	// 共享内存区段的挂载
	void* mem;
	mem = shmat(shareMemoryId, (const void *)0, 0);

	// 获取一个内存区段的信息
	struct shmid_ds shmds;
	ret = shmctl(shareMemoryId, IPC_STAT, &shmds);
	if (ret == 0) {
		printf("Size of memory segment is %d bytes.\n", (int) shmds.shm_segsz);
		printf("Number of attach %d\n", (int) shmds.shm_nattch);
	} else
		printf("shmctl() call failed.\n");

	// 共享内存区段的脱离
	ret = shmdt(mem);
	if (ret == 0)
		printf("Successfully detach memory.\n");
	else
		printf("Memory detached failed %d\n", errno);

	// 删除该共享内存区
	ret = shmctl(shareMemoryId, IPC_RMID, 0);
	if (ret == 0)
		printf("Share memory removed.\n");
	else
		printf("Share memory remove failed.\n");

	return 0;
}
