#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
class Thread
{
	public:
		Thread()
		{
			pthread_mutex_init(&log_lock,NULL);
		};

		~Thread()
		{
			pthread_mutex_destory(&log_lock,NULL);
		};
};



































