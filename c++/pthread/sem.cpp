/* review sem for pthread && pthread_mutex
 * c++ virtual function use
 * pthread_mutex 使用场景：多线程操作global变量&&share function时（互斥）
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h> //sem add must
#include <fstream>
#include <stdlib.h>
#include <string.h>
using namespace std;

class Thread
{
	public:
		virtual void set();
		pthread_mutex_t log_lock;
		//static string goods;
		static sem_t sem; //Unsigned long	
		static void s_thread_call(string msg);
	    static void* product(void  *arg);
		static void* customer(void  *arg);
		bool start_thd();
			

		Thread()
		{
			pthread_mutex_init(&log_lock,NULL);
			cout<<"Thread() ="<<endl;
			sem_init(&sem,0,2);
		};

		~Thread()
		{
			pthread_mutex_destroy(&log_lock);
			sem_destroy(&sem);
			
		};
	
	private:
};

void Thread:: s_thread_call(string msg)
{
	
	cout<<"msgok ="<<msg.data()<<endl;
}
//两个线程都有result
void* Thread:: product(void  *arg)
{
	int i = 0;
	char *tp = (char*)arg; //强制转换
	cout<< "fuct_par1 =  "<< tp <<endl;
	while(1)
	{
		sem_wait(&sem);
		//pthread_mutex_lock(&log_lock);
		string goods ="product+1";
		s_thread_call(goods);
		//sleep(1);
		sem_post(&sem);
		//pthread_mutex_unlock(&log_lock); 
		sleep(1); //作用是 如果不加的话 另一个线程没有机会 invoke s_thread_call
	}

}

void*  Thread:: customer(void  *arg)
{
	int i = 0;
	char *tp = (char*)arg;
	cout<< "fuct_par2 =  "<< tp <<endl;
	while(1)
	{
		//sem_post(&sem);	
		sem_wait(&sem);
		//pthread_mutex_lock(&log_lock); // 作用是 多线程时只允许 一个线程访问 这段代码
		string goods ="customer-1";
		s_thread_call(goods);
		//sleep(1);
		sem_post(&sem);
		sleep(1);
		//sem_wait(&sem);
		//pthread_mutex_unlock(&log_lock); //unlock 后 log_lock 值为零
		
	}

}

bool Thread::start_thd()
{
	char* msg1 = "thread 1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	char* msg2 = "thread 2 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	
	pthread_t td1,td2;
	int ret1 = -1 ,ret2 = -1;
	ret2 = pthread_create(&td2,NULL,product,msg2);
	ret1 = pthread_create(&td1,NULL,customer,msg1);
	
	if(ret1 && ret2)
	{
		cout<<"creat phtread failed "<<endl;
		return false;
	}
	
	ret2 = pthread_join(td2,NULL);
	ret1 = pthread_join(td1,NULL);

	if(ret1 && ret2)
	{
		cout<<"join phtread failed "<<endl;
		return false;
	}
	return true;
}

int main(int argc ,char* argv[])
{
	
	Thread *thd;
	if(!thd->start_thd())
			exit(0);
	
	return 0;
}
