/*
 * Log.h
 *
 *
 *
 */

#ifndef _LOG_H
#define _LOG_H


#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define HAZARDLOF_MAX_LINE 8000
#define HAZARDLOF_DISPLAY_LINE 1000
enum _logtype
{
	DEBUG_LOG = 0,
	OPERATE_LOG,
	ERROR_LOG,
        HAZARD_LOG,
        DEV_FAULT_LOG, //yq
	LOGTYPE_MAX,
};

ostream& nl(ostream& os);

class Log : public ostringstream {
public:
	static Log log;
    void start(char *log_path);
	void stop();
    static void print(int log_type, const char*, ...);
	void print( const string&);
    void displayHex();
    void displayDec();
    
    
private:


	pthread_mutex_t log_lock;
	
	Log()
	{
		pFsHazard = NULL;
		pthread_mutex_init(&log_lock,NULL);
	};
	~Log()
	{
		pthread_mutex_destroy(&log_lock);
	};
	int openLogFile(int logtype);
	void writeLogFile(int logtype, const char* data, int length);
    int openComPort();
	static string getSysTime();
	static void getSysTime(char *buf);

	void writeHazardLog(const char *data, int length);
private:
	string log_rootpath;
	int mLogFile;
	int mLogFile_operate;
	int mLogFile_error;
    int mComPort;

	fstream *pFsHazard;
	
	int mLogFile_hazard;
	int mLogFile_devFault; //yq

	//int fd;
 };
#endif /* LOG_H_ */
