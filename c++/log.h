#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
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
		void open_files(int type);
		void write_files();
		void run(char *buf);
		void stop();
	private:
		string log_base_path;
		
		
		
};
