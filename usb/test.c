//这是最简单的使用了
#include <stdio.h>
#include <pthread.h>

void *func(void *arg)
{
    printf("arg=%d\n", *(int *)arg);
   // pthread_exit("bye bye");
}

int main()
{
    int res;
    pthread_t first_thread;
    int share_int = 10;
    res = pthread_create(&first_thread, NULL, func, (void *)&share_int);
    printf("res=%d\n", res);
    
    void *returnValue;
    res = pthread_join(first_thread, &returnValue);
    printf("returnValue=%s\n", (char *)returnValue);
    return 0;
}
