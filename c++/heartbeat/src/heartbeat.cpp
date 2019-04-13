/* add 2017-09-08 for heartbeat BEGIN */
#include <stdio.h>
#include <unistd.h>

#include "heartbeat.h"

void hb_usleep(int usecs)
{
  struct timespec delayTime, elaspedTime;
  
  delayTime.tv_sec  = usecs / 1000000;
  delayTime.tv_nsec = (usecs % 1000000) * 1000;

  nanosleep(&delayTime, &elaspedTime);
}

HBUdpNetADP::HBUdpNetADP(int _local_port)
{
    local_port = _local_port;
    UDPSocket = INVALID_SOCKET;
    NetInit();
}

bool HBUdpNetADP::NetInit()
{
    if (UDPSocket != INVALID_SOCKET)
    {   
		NetClose();
    }

	UDPSocket = UDPNet.NetInit(INADDR_ANY, local_port);
	if (UDPSocket == INVALID_SOCKET)
    {   
		return false;
    }

    return true;
}

void HBUdpNetADP::NetClose()
{
	UDPNet.NetClose(UDPSocket);

	UDPSocket = INVALID_SOCKET;
}

bool HBUdpNetADP::NetSend(const UINT32 dest_ip, const int dest_port, char* send_buff, int send_size, int *iRet)
{
    int iResult = 0;
    bool ret = true;
    sockaddr_in sockAddr;
        
    if (send_size <= 0)
    {
        goto ERR_RET;
    }
    
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = dest_ip; // 127.0.0.1  int  16777343
	sockAddr.sin_port = htons(dest_port);

	iResult = UDPNet.NetSend(UDPSocket, sockAddr, send_buff, send_size);
	if (iResult == SOCKET_ERROR)
	{
		NetClose();
		NetInit();
		goto ERR_RET;
	}
	else if (iResult == 0)
	{
		goto ERR_RET;
	}

RET:
    if (iRet)
    {
        *iRet = iResult;
    }

#if 0
    hbprintf(DEBUG_LOG, "[CUdpNetADP::NetSend] %d bytes --> %s:%d\n",
			            send_size,
			            inet_ntoa(sockAddr.sin_addr),
			            ntohs(sockAddr.sin_port)); fflush(stdout);
#endif

    return ret;
    
ERR_RET:
    ret = false;
    goto RET;
}

bool HBUdpNetADP::NetRecv(char* recv_buff, int &recv_len, double time_out_sec, UINT32 *peer_ip, int *peer_port)
{
    sockaddr_in sockAddr;
    
	sockAddr.sin_family = AF_INET;

    memset(recv_buff, 0, MAX_MSG_LEN);

	recv_len = UDPNet.NetRcv(UDPSocket, sockAddr, recv_buff, MAX_MSG_LEN, time_out_sec);

   	if (recv_len == SOCKET_ERROR)
	{
		NetClose();
		NetInit();
		return false;
	}
	else if (recv_len == 0)
	{
		return false;
	}

    UINT32 peer_ip_tmp = ntohl(sockAddr.sin_addr.s_addr);
    int peer_port_tmp = ntohs(sockAddr.sin_port);

#if 0
    hbprintf(DEBUG_LOG, "[CUdpNetADP::NetRecv] %d bytes <-- %s:%d\n",
			            recv_len, 
			            inet_ntoa(sockAddr.sin_addr),
			            peer_port_tmp); fflush(stdout);
#endif

    if (peer_ip)
    {
        *peer_ip = peer_ip_tmp;
    }

    if (peer_port)
    {
        *peer_port = peer_port_tmp;
    }

	return true;
}

extern bool HeartbeatDbg_ON();
extern bool HeartbeatPkgUT_ON();

/* static */ UINT32 Heartbeat::DFT_HEARTBEAT_SRC_PORT 	= 5400;
/* static */ UINT32 Heartbeat::DFT_HEARTBEAT_DST_IP 	= inet_addr("127.0.0.1");	
/* static */ UINT32 Heartbeat::DFT_HEARTBEAT_DST_PORT 	= 5500;

/* static */ const char* Heartbeat::counter_name[HEARTBEAT_COUNTER_NUM] = {
	/* HEARTBEAT_MAIN_WIN */ 	"MAIN_WIN", 
	/* HEARTBEAT_CAM_SEARCH */	"CAM_SEARCH", 
};

/* static */ Heartbeat Heartbeat::heartbeat;

Heartbeat::Heartbeat()
{
	clear();
}

void Heartbeat::clear()
{
	memset(counter, 0, sizeof(counter));
}

void Heartbeat::showContent(const char *dbg_head /*= ""*/)
{
	int  dbg_idx = 0;
	char dbg_buff[MAX_LINE_SIZE];

	dbg_idx += sprintf(dbg_buff + dbg_idx, "%s { ", dbg_head);
	
	for (int i = 0; i < HEARTBEAT_COUNTER_NUM; ++i)
	{
		dbg_idx += sprintf(dbg_buff + dbg_idx, "\"%s\": %d; ", counter_name[i], counter[i]);
	}

	dbg_idx += sprintf(dbg_buff + dbg_idx, "}");

	hbprintf(DEBUG_LOG, "YL -- showContent  = %s", dbg_buff);
}

void updateMainWinHeartBeat()
{
	Heartbeat::heartbeat.counter[HEARTBEAT_MAIN_WIN] += 1;
}

void updateCameraSearchHeartBeat()
{
	Heartbeat::heartbeat.counter[HEARTBEAT_CAM_SEARCH] += 1;
}

HeartbeartStatistic::HeartbeartStatistic(bool _as_monitor)
{
	as_monitor = _as_monitor;
	
	pkg_lost_cnt = 0;
	pkg_lost_kill_cnt = 0;
	
	clear();
}

void HeartbeartStatistic::clear()
{
	cycle_ms = HEARTBEAT_CYCLE_MS;
	print_cycle = HEARTBEAT_PRINT_CYCLE;
	
	ok_cnt = 0;
	timeout_cnt = 0;
	err_cnt = 0;

	kill_cnt.clear();
	lost_cnt.clear();
	last_heart.clear();
}

void HeartbeartStatistic::showContent(const char *dbg_head /*= ""*/)
{
	hbprintf(DEBUG_LOG, "%s statisic {", dbg_head);
	hbprintf(DEBUG_LOG, "    BaseCycleMs: %d; PrintCycle: %d; Ok: %d; Timeout: %d; Err: %d;", 
							 cycle_ms,        print_cycle,    ok_cnt, timeout_cnt, err_cnt);

	if (as_monitor)
	{
		hbprintf(DEBUG_LOG,  "    PkgLostCnt: %d; PkgLostKillCnt: %d; ", pkg_lost_cnt, pkg_lost_kill_cnt);
		lost_cnt.showContent("    LostCounter");
		kill_cnt.showContent("    KillCounter");
	}

	last_heart.showContent("    LastHeartBeat");   

	hbprintf(DEBUG_LOG, "}");
}

HeartbeatSender::HeartbeatSender(int _src_port /*= Heartbeat::DFT_HEARTBEAT_SRC_PORT*/)
	: netAdp(_src_port), statistic(/*_as_monitor=*/ false)
{
	memset(&sendThreadId, 0, sizeof(pthread_t));
	dest_ip = 0;
	dest_port = 0;
	memset(send_buff, 0, sizeof(send_buff));
	send_size = 0;
}

HeartbeatSender::~HeartbeatSender()
{
	stop();
}

bool HeartbeatSender::generateHeartbeatPkg()
{
	heartbeat = Heartbeat::heartbeat; // class  heartbeat = heartbeat 

	if (HeartbeatDbg_ON()) heartbeat.showContent("[Send]");

	memcpy(send_buff, &heartbeat, sizeof(Heartbeat)); // send_buff =  class Heartbeat
	send_size = sizeof(Heartbeat);

	return true;
}

Heartbeat HeartbeatSender::getHeartbeat()
{
	return heartbeat;
}

bool HeartbeatSender::start(	UINT32 _dest_ip /*= DFT_HEARTBEAT_DST_IP*/, 
								int _dest_port /*= DFT_HEARTBEAT_DST_PORT*/ )
{	
#define DBG_HEAD "[ YL -HSender] "

	dest_ip = _dest_ip;
	dest_port = _dest_port;

	hbprintf(DEBUG_LOG, DBG_HEAD "_dest_ip =  %d,  _dest_port  = %d", _dest_ip,_dest_port);
	if (pthread_create(&sendThreadId, NULL, sendThread, (void *)this) != 0)  // (void*)this 
	{
		hbprintf(DEBUG_LOG, DBG_HEAD "pthread_create sendThread failed.");
		return false;
	}
	
	hbprintf(DEBUG_LOG, DBG_HEAD "pthread_create sendThread ok.");
	return true;

#undef DBG_HEAD
}

void HeartbeatSender::stop()
{
	netAdp.NetClose();
}

/* static */ void* HeartbeatSender::sendThread( void* param )
{
#define DBG_HEAD "[ YL -- Send] "

	static int call_cnt = 0;

	int iRet = true;
         bool ret = true;

	HeartbeatSender* sender = (HeartbeatSender*)param;
	HeartbeartStatistic	&statistic = sender->statistic;

	while(1)
	{   
		if (HeartbeatPkgUT_ON())
		{
			goto CYCLE_DELEY; 
		}
		
		sender->generateHeartbeatPkg();
		//string buff = "YQ_SEND_BUFF";
		//strcpy(sender->send_buff ,buff.data());
		//char *buff;// = "YQ_SEND_BUFF";
		//memcpy(sender->send_buff,buff,sizeof(sender->send_buff));
		hbprintf(DEBUG_LOG," YL  -- dest_ip = %d ,dest_port = %d,sender->send_size =%d , sender->send_buff =%s",sender->dest_ip,sender->dest_port,sender->send_size,sender->send_buff);
	    	ret = sender->netAdp.NetSend(sender->dest_ip, 
									sender->dest_port, 
									sender->send_buff, 
									sender->send_size, 
									&iRet);
	    if (ret == false)
	    {
	        if (iRet == 0)
	        {
	            hbprintf(DEBUG_LOG, DBG_HEAD "Time out at NetSend.");  
				statistic.timeout_cnt++;
	        }
	        else
	        {
	            hbprintf(DEBUG_LOG, DBG_HEAD "ERROR at NetSend.");
				statistic.err_cnt++;
	        }

			goto CYCLE_DELEY;
	    }

		// hbprintf(DEBUG_LOG, DBG_HEAD "Hearbeat send ok.");
		statistic.last_heart = sender->getHeartbeat();
		statistic.ok_cnt++;
		
	CYCLE_DELEY:

		++call_cnt;
		if (call_cnt % HEARTBEAT_PRINT_CYCLE == 0)
		{
			statistic.showContent("HeartbeatSender");
		}

		hb_usleep(HEARTBEAT_CYCLE_MS * 1000); // 3000 ms
	}

	return (void *)0;

#undef DBG_HEAD
}

HeartbeatChecker::HeartbeatChecker()
{		
	clear();
}

void HeartbeatChecker::clear()
{
	pkg_recv_cnt = 0;
	pkg_lost_cnt = 0;
	memset(lost_cnt, 0, sizeof(lost_cnt));	
	heartbeat.clear();
}

void HeartbeatChecker::recover()
{
#define DBG_HEAD "[HeartbeatChecker] "

	//hbprintf(DEBUG_LOG, DBG_HEAD "pgrep ssa_tlcd | xargs kill -9");
	//system("pgrep ssa_tlcd | xargs kill");
	hbprintf(DEBUG_LOG, DBG_HEAD " YL --- PING  ping 127.0.0.1 ");
	system(" ping 127.0.0.1");

	hbprintf(DEBUG_LOG, DBG_HEAD "Clear heartbeat and lost counter.");
	clear();
	
#undef DBG_HEAD
}

HeartbeatMonitor::HeartbeatMonitor(int _bind_port /*= Heartbeat::DFT_HEARTBEAT_DST_PORT*/)
	: netAdp(_bind_port), statistic(/*_as_monitor=*/ true)
{
	memset(&recvThreadId, 0, sizeof(pthread_t));
	memset(&checkThreadId, 0, sizeof(pthread_t));
	memset(recv_buff, 0, sizeof(recv_buff));
	recv_size = 0;
	pkg_cnt = 0;
}

HeartbeatMonitor::~HeartbeatMonitor()
{
	stop();
}

bool HeartbeatMonitor::parseHeartbeatPkg()
{
	memcpy(&heartbeat, recv_buff, sizeof(Heartbeat));

	if (HeartbeatDbg_ON()) heartbeat.showContent("[YL --Recv]");

	return true;
}

Heartbeat HeartbeatMonitor::getHeartbeat()
{
	return heartbeat;
}

void HeartbeatMonitor::stop()
{
	netAdp.NetClose();
}

bool HeartbeatMonitor::start()
{	
#define DBG_HEAD "[HeartbeatMonitor] "

	if (pthread_create(&recvThreadId, NULL, recvThread, (void*)this) != 0)
	{
		hbprintf(DEBUG_LOG, DBG_HEAD "pthread_create recvThread failed.");
		return false;
	}
	hbprintf(DEBUG_LOG, DBG_HEAD "pthread_create recvThread ok.");

	if (pthread_create(&checkThreadId, NULL, checkThread, (void*)this) != 0)
	{
		hbprintf(DEBUG_LOG, DBG_HEAD "pthread_create checkThread failed.");
		return false;
	}
	hbprintf(DEBUG_LOG, DBG_HEAD "pthread_create checkThread ok.");

	return true;

#undef DBG_HEAD
}

/* static */ void* HeartbeatMonitor::recvThread( void* param )
{
#define DBG_HEAD "[Recv] "

	bool 	ret = true;
	UINT32 	peer_ip;
	int 	peer_port;
	double  heartbeat_cycle_sec = 1.0 * HEARTBEAT_CYCLE_MS / 1000;

	HeartbeatMonitor* monitor = (HeartbeatMonitor*)param;
	HeartbeartStatistic	&statistic = monitor->statistic;

	while(1)
	{    	
	    ret = monitor->netAdp.NetRecv(monitor->recv_buff, monitor->recv_size, heartbeat_cycle_sec, &peer_ip, &peer_port);
        if (ret == false)
        {
            if (monitor->recv_size == 0)
            {
            	hbprintf(DEBUG_LOG, DBG_HEAD "Time out at NetRecv.");
				statistic.timeout_cnt++;
			}
            else
            {
            	hbprintf(DEBUG_LOG, DBG_HEAD "ERROR at NetRecv.");
				statistic.err_cnt++;
            }

			continue;
        }
		
		//hbprintf(DEBUG_LOG, DBG_HEAD "Hearbeat recv ok.");
		monitor->pkg_cnt++;
		statistic.ok_cnt++;

		monitor->parseHeartbeatPkg();
	}

	return (void *)0;

#undef DBG_HEAD
}

/* static */ void* HeartbeatMonitor::checkThread( void* param )
{
#define DBG_HEAD "[Check] "

	const int LOST_LIMIT = 5;

	static int call_cnt = 0;

	HeartbeatMonitor* monitor = (HeartbeatMonitor*)param;
	HeartbeartStatistic	&statistic = monitor->statistic;

	int	pkg_cnt = 0;
	bool err_found = false;
	Heartbeat new_heartbeat;
	HeartbeatChecker checker;

	hb_usleep(HEARTBEAT_CYCLE_MS * 1000); // 3000 ms

	while(1)
	{ 
		int &pkg_lost_cnt = checker.pkg_lost_cnt;

		cout <<"YL -- monitor->pkg_cnt =  "<<monitor->pkg_cnt<<"  pkg_cnt = "<< pkg_cnt<<endl;
		if (monitor->pkg_cnt == pkg_cnt)
		{
			pkg_lost_cnt++;
			statistic.pkg_lost_cnt++;
			hbprintf(DEBUG_LOG, DBG_HEAD " YQ Pkg lost for %d times.", pkg_lost_cnt); 

			if (LOST_LIMIT < pkg_lost_cnt)
			{	
				checker.recover();

				statistic.pkg_lost_kill_cnt++;			
			}

			goto LOOP_DELAY;
		}
		else
		{
			pkg_cnt = monitor->pkg_cnt;
			pkg_lost_cnt = 0;
		}		
	
		new_heartbeat = monitor->getHeartbeat();

		err_found = false;
		for (int i = 0; i < HEARTBEAT_COUNTER_NUM; ++i)
		{
			
			hbprintf(DEBUG_LOG, DBG_HEAD " YQbefor  new _counter =    %d  ,old_ counter =    %d .",new_heartbeat.counter[i],checker.heartbeat.counter[i]); 
			
			if (new_heartbeat.counter[i] == checker.heartbeat.counter[i])
			{
				err_found = true;
				break;
			}
		}

		if (err_found)
		{
			//new_heartbeat.showContent(DBG_HEAD);
		}
	
		for (int i = 0; i < HEARTBEAT_COUNTER_NUM; ++i)
		{
			int &lost_cnt = checker.lost_cnt[i];
			hbprintf(DEBUG_LOG, DBG_HEAD " YQ  new _counter =    %d  ,old_ counter =    %d .",new_heartbeat.counter[i],checker.heartbeat.counter[i]); 
			if (new_heartbeat.counter[i] != checker.heartbeat.counter[i])
			{
				lost_cnt++;
				statistic.lost_cnt.counter[i]++;
				hbprintf(DEBUG_LOG, DBG_HEAD "\"%s\" lost for %d times.", Heartbeat::counter_name[i], lost_cnt); 
				
				if (LOST_LIMIT < lost_cnt)
				{	
					checker.recover();

					statistic.kill_cnt.counter[i]++;
					
					goto LOOP_DELAY;
				}
			}
			else
			{
				lost_cnt = 0;
			}
		}

		checker.heartbeat = new_heartbeat;
		statistic.last_heart = new_heartbeat;

	LOOP_DELAY:

		++call_cnt;
		if (call_cnt % HEARTBEAT_PRINT_CYCLE == 0)
		{
			statistic.showContent("HeartbeatMonitor");
		}
		
		hb_usleep(HEARTBEAT_CYCLE_MS * 1000); // 3000 ms
	}

	return (void *)0;
}

/* add 2017-09-08 for heartbeat END */
/////////////////////////////////////////////////////EOF//////////////////////////////////////////////
