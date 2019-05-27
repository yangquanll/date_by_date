#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>
#define MAX_MSG_LEN 1024
#define CYCLE_MS 		5000

//#define SAFE_DETETE(p)  if(NULL != p) { delete(p); p = NULL; } 
class Udp_Net
{
public:

	Udp_Net();

	~Udp_Net();

	int NetInit(unsigned int ip,unsigned short port);
	int NetSend(int udpsocket, sockaddr_in remote, char *sendbuf, int buflen); // udp sendto()
	int NetRcv(int udpsocket, sockaddr_in &remote, char *rcvbuf, int buflen, double timeout);  // udp recvfrom()
	int udpskt;
	void NetClose(void);
	
};

class Recv //client_socket
{
public:
	Recv();
	~Recv();
	Udp_Net udpnet;
	static void* recvthread(void* param);
	pthread_t recvThreadId;
	int udpskt_recv;
	//char recvbuff[MAX_MSG_LEN];
	bool start(void);
	int recvdata(void);
	
};

class Send //dest_socket
{
public:
	Send();
	~Send();
	Udp_Net udpnet;
	int udpskt;
	char bufsed[MAX_MSG_LEN];
//	static UINT32 SEND_IP;
//	static UINT32 SEND_PORT;
	static void* sendthread(void* param);
	pthread_t sendThreadId;
	
	int 		dest_ip;
	int			dest_port;
	int 		send_size;
	
	bool start(int _dest_ip, int _dest_port);
	bool start(void);
	int senddata(void);
};
