#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>

#include "Log.h"

#define unprintf(log_type, str, args...) do { string format; format.append("%s,%s,").append(str);	\
				switch(log_type)	\
				{	\
				case DEBUG_LOG:	Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[udpnet]", ## args);break;	\
				case OPERATE_LOG:Log::log.print(OPERATE_LOG, format.c_str(), "[OPR]", "[udpnet]", ## args);break;	\
				case ERROR_LOG:Log::log.print(ERROR_LOG, format.c_str(), "[ERR]", "[udpnet]", ## args);break;	\
				default :	Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[udpnet]", ## args);break;	\
				}}while(0)

class CUdpNet
{
public:
	CUdpNet();
public:
	~CUdpNet(void);

public:

	int		NetInit(unsigned int haddr, unsigned short hport);
	void	NetClose(int udpsocket);

	int		NetRcv(int udpsocket, sockaddr_in &remote, char *rcvbuf, int buflen, double timeout); /* by lq 2016-09-26 */
	int		NetSend(int udpsocket, sockaddr_in remote, char *sendbuf, int buflen );
};
