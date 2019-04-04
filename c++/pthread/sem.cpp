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

pthread_mutex_t log_lock;
sem_t sem; //Unsigned long
string goods;

class Thread
{
	public:
		virtual void set();

		Thread()
		{
			pthread_mutex_init(&log_lock,NULL);
			sem_init(&sem,0,0);
		};

		~Thread()
		{
			pthread_mutex_destroy(&log_lock);
			sem_destroy(&sem);
			
		};
};

void s_thread_call(string msg)
{
	
	cout<<"msgok ="<<msg.data()<<endl;
}
//两个线程都有result
void* product(void  *arg)
{
	int i = 0;
	char *tp = (char*)arg; //强制转换
	cout<< "fuct_par1 =  "<< tp <<endl;
	while(1)
	{
		pthread_mutex_lock(&log_lock);
		goods ="product+1";
		s_thread_call(goods);
		pthread_mutex_unlock(&log_lock); 
		sleep(1); //作用是 如果不加的话 另一个线程没有机会 invoke s_thread_call
	}

}

void* customer(void  *arg)
{
	int i = 0;
	char *tp = (char*)arg;
	cout<< "fuct_par2 =  "<< tp <<endl;
	while(1)
	{
		pthread_mutex_lock(&log_lock); // 作用是 多线程时只允许 一个线程访问 这段代码
		goods ="product-1";
		s_thread_call(goods);
		pthread_mutex_unlock(&log_lock); //unlock 后 log_lock 值为零
		sleep(1);
	}

}

int main(int argc ,char* argv[])
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
		exit(0);
	}
	
	ret2 = pthread_join(td2,NULL);
	ret1 = pthread_join(td1,NULL);

	if(ret1 && ret2)
	{
		cout<<"join phtread failed "<<endl;
		exit(0);
	}
	return 0;
}
