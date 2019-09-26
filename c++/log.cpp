/***
* 一般的软件工程中，日志的作用都非常大，下面介绍一种c/c++代码生成日志的方法
* 支持带有时间戳；写入文件并不造成覆盖；支持多线程，进程
* 封装成 3种格式：debug,error,inf,三种输出
* 自定义 log文件的位置
* 自定义定义log文件的大小 超过则删除
*
***/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <iostream>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include "Log.h"
#define CPRINT(log_type, str, args...) do { string format; format.append("%s,%s,").append(str);   \
               switch(log_type)    \
               {   \
               case DEBUG_LOG: Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[ylq]",## args);break;  \
               case OPERATE_LOG:Log::log.print(OPERATE_LOG, format.c_str(), "[OPR]", "[ylq]", ## args);break;   \
               case ERROR_LOG:Log::log.print(ERROR_LOG, format.c_str(), "[ERR]", "[ylq]", ## args);break;   \
               default :   Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[ylq]", ## args);break;  \
               }}while(0)


using namespace std;

Log Log::log;
/*
ostream& nl(ostream& os){
	string s = Log::log.str();
	Log::log.str("");
	Log::log.print(s);
	return os;
}
*/
void Log::start(char *log_path) {

	log_rootpath.clear();
	log_rootpath.append(log_path);

	char cmdbuf[256];
	sprintf(cmdbuf, "mkdir -p %s", log_path);
	system(cmdbuf);

	mLogFile = openLogFile(DEBUG_LOG);

//	mLogFile_operate = openLogFile(OPERATE_LOG);
//	mLogFile_error = openLogFile(ERROR_LOG);
 //       mLogFile_hazard = openLogFile(HAZARD_LOG);
 //       mLogFile_devFault = openLogFile(DEV_FAULT_LOG);
//	mComPort = openComPort();
	mComPort = -1;
    this->displayHex();
}

void Log::stop() {

	this->displayDec();

	if (mComPort != -1)
		close(mComPort);

	close(mLogFile);
}


void Log::displayDec() {
    setf(ios::dec, ios::basefield);
}

void Log::displayHex() {
    setf(ios::hex, ios::basefield);
}

void Log::print(const string& str)
{
	cout<< "print 1 \n"<<endl;
	
	string msg;

	msg = getSysTime()+ str;

	cout <<"print " << msg << endl;

	msg.append("\n");
	this->writeLogFile(DEBUG_LOG, msg.data(), msg.length());

	if(mComPort != -1)
	{
		::write(mComPort, msg.data(), msg.length());
	}

}

void Log::print(int logtype, const char * format, ...)
{
	va_list vl;
	va_start(vl, format);

	char buf[500] = {0};
	char timeStr[100] = {0};
	getSysTime(timeStr);
	
	if (logtype == HAZARD_LOG)
	{
		memcpy(buf, &timeStr[1], strlen(timeStr) -3);
		buf[strlen(buf)] = ';';
	}
	else
	{
		memcpy(buf, timeStr, strlen(timeStr));
	}
	int len = strlen(buf);
	cout<<"len befor = "<<len<<endl;
	len +=vsprintf(buf+len, format, vl);
	cout<<"len end = "<<len<<endl;
	va_end(vl);

	

	//if(buf[len-1]== 0x0a)
	//{
	//	buf[len-1] = 0x0d;
	//	buf[len] = 0x0a;
	//	len++;
	//}

	if (buf[len -1] != '\n')
	{
		buf[len] = '\n';
		len++;
	}
	//cout<<"buf = %s"<<buf<<endl;
	cout<<"buf = "<<buf << "  len ="<< len <<endl;
	Log::log.writeLogFile(logtype,buf, len);
	if(logtype != DEBUG_LOG)
		Log::log.writeLogFile(DEBUG_LOG, buf, len);
	
	if(Log::log.mComPort != -1)
	{
		::write(Log::log.mComPort, buf, len);
	}

}

void Log::writeHazardLog(const char *data, int length)
{
	char buf[250];
	int rowNum = 1;
	
	if (pFsHazard->tellg() > 0)
	{
		pFsHazard->seekg(0, ios::beg);
		pFsHazard->getline(buf, 250,'\n');
/*		if (a > "0")
		{
			rowNum = ::atoi(&buf[7]) + 1;
			cout << "rowNum:" << rowNum <<endl;
		}
	*/	
	}
	if (rowNum >= HAZARDLOF_MAX_LINE)
	{
		rowNum = HAZARDLOF_DISPLAY_LINE;
		
		vector<string> vecContent;
		string strLine;
		while (!pFsHazard->eof())
		{
			pFsHazard->getline(buf, 250,'\n');
			strLine.clear();
			strLine.append(buf);
			//cout << "strLine:" << strLine <<endl;
			vecContent.push_back(strLine);
		}

		vecContent.erase(vecContent.begin(), vecContent.begin() + (HAZARDLOF_MAX_LINE - rowNum + 1));
		pFsHazard->close();
		string hazardLog;
		hazardLog.append(log_rootpath).append("/hazard_log.txt");
		//cout << "rootpath:" << hazardLog.data() <<endl;

		pFsHazard->open(hazardLog.data(), ios_base::in | ios_base::out | ios_base::ate | ios_base::trunc); 
		pFsHazard->write(buf, strlen(buf));
		vector<string>::const_iterator iter = vecContent.begin();
		for (; vecContent.end() != iter; ++iter)
		{
			if ((*iter).compare("") == 0)
			{
				continue;
			}
			pFsHazard->write((*iter).c_str(), (*iter).size());
        	(*pFsHazard) << '\n';
		}
	}
	else
	{
		//cout << "buf string:" << buf <<endl;
		memset(buf, 0, 20);
		sprintf(buf, "rowNum=%d\n", rowNum);
		pFsHazard->seekp(0, ios::beg);
		pFsHazard->write(buf, strlen(buf));
		pFsHazard->flush();
	}
	pFsHazard->seekp(0, ios::end);
	pFsHazard->write(data, length);
	pFsHazard->flush();
}

void Log::writeLogFile(int logtype, const char *data, int length)
{
	//if(mLogFile == -1) return;
	//cout<<"buf =  %s "<<data<<endl;
	int mLogFile_temp;
	if(logtype == OPERATE_LOG)
	{
		mLogFile_temp = mLogFile_operate;
	}
	else if(logtype == ERROR_LOG)
	{
		mLogFile_temp = mLogFile_error;
	}
        else if (logtype == HAZARD_LOG)
	{
		mLogFile_temp = mLogFile_hazard;
        }
        else if (logtype == DEV_FAULT_LOG)
	{
		mLogFile_temp = mLogFile_devFault;
	}
	else
	{
		mLogFile_temp = mLogFile;
	}

	if(mLogFile_temp == -1)
		return;
	
	ssize_t bytesWritten;

	pthread_mutex_lock(&log_lock);
	if(logtype == HAZARD_LOG)
	{
		if (NULL == pFsHazard)
		{
			pthread_mutex_unlock(&log_lock);
			//openLogFile(logtype);
			return;
		}

		writeHazardLog(data, length);
		pthread_mutex_unlock(&log_lock);
		return;
	}
	bytesWritten = ::write(mLogFile_temp, data, length);
	//pthread_mutex_unlock(&log_lock);
	//pthread_mutex_destroy(&log_lock);
	
	if(bytesWritten == -1)
	{
		int err;
		err = errno;
		::close(mLogFile_temp);
		mLogFile_temp = openLogFile(logtype);
		if(mLogFile_temp != -1)
		{
			char errBuf[100];
			sprintf(errBuf, "Write log file error: errno = %d %s", err, strerror(err));
			int num1 = ::write(mLogFile_temp, errBuf, sizeof(errBuf));
//			int num2 = ::write(mLogFile_temp, data, length);
//			if(num1 == -1 || num2 == -1)
			if (num1 == -1)
			{
				cout<<"[Log] [Minor] System ERROR: write error."<<endl;
				close(mLogFile_temp);
				mLogFile_temp = -1;
			}
		}

	}
	
	pthread_mutex_unlock(&log_lock);
	//pthread_mutex_destroy(&log_lock);
	
}

string Log::getSysTime()
{
	char buf[100];

	getSysTime(buf);
	return string(buf);
}

void Log::getSysTime(char *buf)
{
	struct tm T;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &T);
	int millisecond = tv.tv_usec/1000;

	sprintf(buf, "%02d-%02d %02d:%02d:%02d %03d - ", T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec, millisecond);
	//sprintf(buf, "[%02d-%02d %02d:%02d:%02d],", T.tm_mon+1, T.tm_mday, T.tm_hour, T.tm_min, T.tm_sec);
}

int Log::openComPort()
{
	termios comConfig;
	int fdCom;
	int errFlags;

	fdCom = open("/dev/tts/1", O_RDWR|O_NOCTTY|O_NDELAY);
	errFlags = errno;
	if(fdCom == -1)
	{
		cout<<"**********"<<endl;
		cout<<"[LOG] [Minor] System Error: open diagnostic com port error: errno = "<<errFlags<<endl;
		cout<<"**********"<<endl;
		return -1;
	}

	if(tcgetattr(fdCom, &comConfig) != 0)
	{
		cout<<"******************"<<endl;
		cout<<"[LOG] [Minor] System error : tcgetattr(), errno = "<<errno<<endl;
		cout<<"*****************"<<endl;
		close(fdCom);
		return -1;
	}
	termios newtio;
	bzero(&newtio, sizeof(termios));

	newtio.c_cflag |= (CLOCAL |CREAD);
	cfsetispeed(&newtio,  B19200);
	cfsetospeed(&newtio, B19200);

	newtio.c_cflag &= ~PARENB;
	newtio.c_cflag |= CSTOPB;
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;

	newtio.c_cc[VMIN]=0;
	newtio.c_cc[VTIME]=0;

	tcflush(fdCom, TCIOFLUSH);
	if(tcsetattr(fdCom , TCSANOW, &newtio) !=0)
	{
		cout<<"******************"<<endl;
		cout<<"[LOG] [Minor] System error :tcsetattr(), errno = "<<errno<<endl;
		cout<<"*****************"<<endl;
		close(fdCom);
		return -1;
	}
	
	return fdCom;
}

int Log::openLogFile(int logtype) {

        string logFile_operate;
        //logFile_operate.append(log_rootpath).append("/log_operate.txt");
	string logFile_error;
       // logFile_error.append(log_rootpath).append("/log_error.txt");
	string logFile;
	logFile.append(log_rootpath).append("./log.txt");
	string loglogFile;
        //loglogFile.append(log_rootpath).append("./log_log.txt");
	string log1File;
        //log1File.append(log_rootpath).append("./log1.txt");

        string hazardLog;
        //hazardLog.append(log_rootpath).append("./hazard_log.txt");

	string logFile_devFault;//yq
        //logFile_devFault.append(log_rootpath).append("./log_dev_fault.txt"); //yq


	string fileName;

	int fd = -1;
	int errFlags;
	switch(logtype)
	{
		case OPERATE_LOG:	fileName = logFile_operate;	break;
		case ERROR_LOG:		fileName = logFile_error;	break;
		case DEV_FAULT_LOG: fileName = logFile_devFault;	break;
		case HAZARD_LOG:
			fileName = hazardLog;
			break;
		case DEBUG_LOG:		
		default:				fileName = logFile;			break;
	}
	if (HAZARD_LOG == logtype)
	{
		if (NULL == pFsHazard)
		{
			pFsHazard = new fstream();
		}
		cout << fileName.data() << endl;
		if (-1 == access(fileName.data(), 0))
		{
			cout << "file does not exit. " << endl;
			pFsHazard->open(fileName.data(), ios_base::in | ios_base::out | ios_base::app);
			pFsHazard->close();
		}
		
		pFsHazard->open(fileName.data(), ios_base::in | ios_base::out | ios_base::ate);
		
		return 0;
	}
//	fileName = logFile;
    for(int i=0;i<3;i++) {
		fd = open(fileName.data(), O_RDWR|O_CREAT|O_APPEND, 0755);
		errFlags = errno;
		if(fd == -1)
		{
            cout<<"**********"<<endl;
			cout<<"[LOG] [Minor] System Error: open log file: errno = "<<errFlags<<endl;
			cout<<"**********"<<endl;
			sleep(1);
		}else
			break;
    }

    if( fd == -1 ) {
		fileName = loglogFile;
		fd = open(fileName.data(), O_RDWR|O_CREAT|O_APPEND, 0755);
		errFlags = errno;
        if( fd == -1 ) {
			cout<<"**********"<<endl;
            cout <<"[LOG] [Minor] System ERROR: open extra log file: errno = "<<errFlags<<endl;
			cout<<"**********"<<endl;
            return -1;
        }
    }

	struct stat fs;
	fstat(fd, &fs);
	//cout<<"[open]fs.st_size = "<<fs.st_size<<endl;
	if(fs.st_size > 50000000)
	{
		if(fs.st_size >= 1000000000)	//100M
		{
			close(fd);
			fd = open(fileName.data(), O_CREAT|O_RDWR|O_APPEND|O_TRUNC, 0755);
			errFlags = errno;
			if(fd == -1)
			{
		        cout<<"**********"<<endl;
				cout<<"[LOG] [Minor] System Error: open log file: errno = "<<errFlags<<endl;
				cout<<"**********"<<endl;
				return -1;
			}
		}
		else
		{	int  fd1 = -1;
			char buf[2048];

			fd1 = open(log1File.data(), O_RDWR|O_CREAT|O_APPEND|O_TRUNC, 0755);
			errFlags = errno;
			if(fd1 == -1)
			{
				cout<<"**********"<<endl;
				cout << "[LOG] [Minor] System ERROR: open backup log file: errno = "<<errFlags<<endl;
				cout<<"**********"<<endl;
				return -1;
			}

			size_t bytesRead = 0;
			size_t bytesWrite = 0;
			lseek(fd, 0, SEEK_SET);
			bool bErr = false;
			do{
		        bytesRead = ::read(fd, (void*)buf, 2048);
				if(bytesRead < 0) 
				{
					bErr = true;
					errFlags = errno;
					break;
				}

		        size_t bytesToWrite = bytesRead;
				char *pData = buf;
		        while(bytesToWrite > 0)
				{
					bytesWrite = ::write(fd1, (void*)pData, bytesRead);
					if(bytesWrite < 0) 
					{
						bErr = true;
						errFlags = errno;
						break;
					}
					else
					{
						bytesToWrite -= bytesWrite;
						pData += bytesWrite;
					}
				}

				if(bErr) break; //jump the outer loop as the write() function error.
			
			}while(bytesRead > 0);

			close(fd);
			close(fd1);

			if(bErr)
			{
			   	cout<<"**********"<<endl;
				cout<<"[LOG] [Minor] System Error: copy file error: errno = "<<errFlags<<endl;
				cout<<"**********"<<endl;
				return -1;
			}

			fd = open(fileName.data(), O_CREAT|O_RDWR|O_APPEND|O_TRUNC, 0755);

			errFlags = errno;
			if(fd == -1)
			{
				cout<<"**********"<<endl;
				cout<<"[LOG] [Minor] System Error: open log file: errno = "<<errFlags<<endl;
				cout<<"**********"<<endl;
				return -1;
			}
		}
	}


	return fd;
}
#define LOG_FILE_PATH               "./log/"
int main()
{	
int i = 0;
while (i < 10)
{
Log::log.start(LOG_FILE_PATH);                                                                                                                                                                     
CPRINT(DEBUG_LOG,"****************** start ****************** %d",i);
//CPRINT(DEBUG_LOG,"oooooooooooooooooooooooooooooooooooooooooooooo");
//CPRINT(DEBUG_LOG,"i = %d",i);
 sleep(0.2);
i++;
}
}
