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

		static void* tst_log2(void* arg);
		static void* tst_log1(void* arg);
		void excute();
		void Cout(int log_type, const char*, ...);
		Log()
		{
			pthread_mutex_init(&log_lock,NULL);
		};
		~Log()
		{
			pthread_mutex_destroy(&log_lock);
		};
	private:
		string log_base_path;
		fstream *pFsop;
		int m_Fd;
		
		
		
};
