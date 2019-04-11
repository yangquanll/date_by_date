/* add 2017-09-08 for heartbeat BEGIN */
#pragma once

#include <pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "udpnet.h"
#include "Log.h"

#ifndef UINT32
#define UINT32			u_int32_t
#endif

#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN		4400
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET	-1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR	-1
#endif

#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 	512
#endif

#define HEARTBEAT_CYCLE_MS 		5000
#define HEARTBEAT_PRINT_CYCLE 	6

#define hbprintf(log_type, str, args...) do { string format; format.append("%s,%s,").append(str);	\
				switch(log_type)	\
				{	\
				case DEBUG_LOG:	Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[heartbeat]", ## args);break;	\
				case OPERATE_LOG:Log::log.print(OPERATE_LOG, format.c_str(), "[OPR]", "[heartbeat]", ## args);break;	\
				case ERROR_LOG:Log::log.print(ERROR_LOG, format.c_str(), "[ERR]", "[heartbeat]", ## args);break;	\
				default :	Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[heartbeat]", ## args);break;	\
				}}while(0)

class HBUdpNetADP
{
private:
	CUdpNet	UDPNet;
	int 	local_port;

public: /* data */
	int		UDPSocket;

public: /* func */		
	HBUdpNetADP(int _local_port);
	bool NetInit();
	void NetClose();
	bool NetSend(const UINT32 dest_ip, const int dest_port, char* send_buff, int send_size, int *iRet = NULL);
	bool NetRecv(char* recv_buff, int &recv_len, double time_out_sec, UINT32 *peer_ip = NULL, int *peer_port = NULL);
};

typedef enum
{
	HEARTBEAT_MAIN_WIN,
	HEARTBEAT_CAM_SEARCH,	
	HEARTBEAT_COUNTER_NUM,
} HEARTBEAT_COUNTER_EN;

class Heartbeat
{
public:
	static UINT32 DFT_HEARTBEAT_SRC_PORT;
	static UINT32 DFT_HEARTBEAT_DST_IP;   
	static UINT32 DFT_HEARTBEAT_DST_PORT; 
	
	static const char* counter_name[HEARTBEAT_COUNTER_NUM];

	static Heartbeat heartbeat;

public:
	int counter[HEARTBEAT_COUNTER_NUM];

public:
	Heartbeat();
	void clear();
	void showContent(const char *dbg_head = "");
};

class HeartbeartStatistic
{
public:
	bool 		as_monitor;

	/* For sender and monitor */
	int 		cycle_ms;
	int 		print_cycle;
	int			ok_cnt;
	int 		timeout_cnt;
	int 		err_cnt;

	/* For monitor */
	int 		pkg_lost_cnt;
	int 		pkg_lost_kill_cnt;
	Heartbeat	kill_cnt;
	Heartbeat 	lost_cnt;
	Heartbeat	last_heart;

public:
	HeartbeartStatistic(bool _as_monitor);
	void clear();
	void showContent(const char *dbg_head = "");
};

class HeartbeatSender
{
private:
	HBUdpNetADP	netAdp;
	pthread_t 	sendThreadId;  

	UINT32		dest_ip;
	int			dest_port;
	
	char		send_buff[MAX_MSG_LEN];
	UINT32		send_size;

	Heartbeat 	heartbeat;

	HeartbeartStatistic	statistic;

public:
	static void* sendThread(void *);

	HeartbeatSender(int _src_port = Heartbeat::DFT_HEARTBEAT_SRC_PORT);
	~HeartbeatSender();

	bool		generateHeartbeatPkg();
	Heartbeat   getHeartbeat();
	
	bool 		start(UINT32 _dest_ip = Heartbeat::DFT_HEARTBEAT_DST_IP, 
						int _dest_port = Heartbeat::DFT_HEARTBEAT_DST_PORT); 
	void 		stop();
};

class HeartbeatChecker
{
public:
	int 		pkg_recv_cnt;
	int 		pkg_lost_cnt;
	int 		lost_cnt[HEARTBEAT_COUNTER_NUM];
	Heartbeat 	heartbeat;

public:
	HeartbeatChecker();
	
	void 		recover();

private:
	void 		clear();
};

class HeartbeatMonitor
{
private:

	HBUdpNetADP	netAdp;
	pthread_t	recvThreadId;  
	pthread_t	checkThreadId;

	char		recv_buff[MAX_MSG_LEN];
	int			recv_size;

	Heartbeat	heartbeat;

	int			pkg_cnt;
	HeartbeartStatistic	statistic;
	
public:
	static void* recvThread(void *);
	static void* checkThread(void *);

	HeartbeatMonitor(int _bind_port = Heartbeat::DFT_HEARTBEAT_DST_PORT);
	~HeartbeatMonitor();

	bool		parseHeartbeatPkg();

	Heartbeat	getHeartbeat();

	bool 		start(); 
	void 		stop();
};

/* add 2017-09-08 for heartbeat END */
/////////////////////////////////////////////EOF//////////////////////////////////////
