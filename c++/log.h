#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

static pthread_mutex_t log_lock;
pthread_cond_t cond;
pthread_cond_t n_empty;

enum _logtype
{
	DEBUG_LOG = 0,
	OPERATE_LOG,
	ERROR_LOG,
};

class Log
{
	public:
		static string gettime();
		static void gettime(char *buf);
		int open_files(int type);
		void write_files(int log_type, const char *ms,int len);
		void run(char *buf);
		void stop();
		//pthread_mutex_t log_lock;
		static void* tst_log2(void* arg);
		static void* tst_log1(void* arg);
		void excute();
		void Cout(int log_type, const char*, ...);
		Log()
		{
			pthread_mutex_init(&log_lock,NULL);
			pthread_cond_init(&cond,NULL);
			pthread_cond_init(&n_empty,NULL);
			
		};
		~Log()
		{
			pthread_mutex_destroy(&log_lock);
			pthread_cond_destroy(&cond);
			pthread_cond_destroy(&n_empty);
		};
	private:
		string log_base_path;
		fstream *pFsop;
		int m_Fd;
		
		
		
};
