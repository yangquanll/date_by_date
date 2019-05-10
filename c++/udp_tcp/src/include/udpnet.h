#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <strlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

class Udp_Net
{
public:
	Udp_Net();

	~Udp_Net();

	int NetInit(unsigned int ip,unsig short port);
	int NetSend(int udpsocket, sockaddr_in remote, char *sendbuf, int buflen); // udp sendto()
	int NetRcv(int udpsocket, sockaddr_in &remote, char *rcvbuf, int buflen, double timeout);  // udp recvfrom()
	void NetClose(int Socket);
};
