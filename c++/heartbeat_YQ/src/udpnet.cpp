#include <unistd.h>
#include <string.h>

//#include "commonfun.h"
#include "udpnet.h"

#include <errno.h>

#define SAFE_DELETE(p) {if((p) != NULL) delete [] (p); (p) = NULL;}

#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define INVALID_DEVICE	-1

CUdpNet::CUdpNet()
{

}

CUdpNet::~CUdpNet(void)
{

}

int CUdpNet::NetInit(unsigned int haddr, unsigned short hport)
{
	int udpsocket;

	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	udpsocket = socket(AF_INET,SOCK_DGRAM,0);
	if (udpsocket == INVALID_SOCKET) {
		unprintf(ERROR_LOG, "<NetInit> -- socket() failed.");
		goto safe_exit ;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(haddr);
	local.sin_port = htons(hport);

	if (bind(udpsocket, 
		(struct sockaddr *) &local,
		sizeof(local)) == SOCKET_ERROR) 
	{
		unprintf(ERROR_LOG, "<NetInit> -- bind() failed.");
		close(udpsocket);
		udpsocket = INVALID_SOCKET;
	}

	struct linger optionval;
	memset(&optionval, 0, sizeof(struct linger));

	if (setsockopt(udpsocket, SOL_SOCKET, SO_LINGER, &optionval, sizeof(struct linger)) ==  -1)
	{
		unprintf(ERROR_LOG, "<NetInit> -- setsockopt() failed.");
		close(udpsocket);
		udpsocket = INVALID_SOCKET;
	}

safe_exit:
	return udpsocket;
}

void CUdpNet::NetClose(int udpsocket)
{
	close(udpsocket);
}

int CUdpNet::NetRcv(int udpsocket, sockaddr_in &remote, char *rcvbuf, int buflen, double timeout)
{
	int iResult = 0;

	socklen_t		remoteLen = sizeof(remote);
	struct timeval	tWait;

   	 int sec = timeout;
    	int usec = (int)(timeout * 1000 * 1000) % (1000 * 1000);
    
	tWait.tv_sec = sec;
	tWait.tv_usec = usec;

	if (rcvbuf == NULL)
	{
		unprintf(ERROR_LOG, "<NetRcv> -- receive buffer is null ");
		goto safe_exit;
	}

	if (buflen == 0)
	{
		unprintf(ERROR_LOG, "<NetRcv> -- receive buffer length = 0 ");
		goto safe_exit;
	}


	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(udpsocket, &fd);
	iResult = select( udpsocket + 1, &fd, NULL, NULL, NULL); //&tWait  Blocking process unlock NULL 
	if (iResult == -1)
	{
		unprintf(ERROR_LOG, "<NetRcv> -- Error at select()");
		goto safe_exit;
	}
	else if (iResult == 0)
	{
		//unprintfERROR_LOG, ("<NetRcv> -- Timeout at select() \n");
		goto safe_exit;
	}


	if (!FD_ISSET(udpsocket, &fd))
	{
		unprintf(ERROR_LOG, "<NetRcv> -- net receive: FD_ISSET = false");
		iResult = 0;
		goto safe_exit;
	}

	memset(rcvbuf, 0, buflen);

	iResult = recvfrom( udpsocket, rcvbuf, buflen, 0, (sockaddr *)(&remote), &remoteLen);
	unprintf(DEBUG_LOG, "<YL NetRCV > -- rcvbuf buffer  %s ",rcvbuf);
	
	if ( iResult == 0 )
	{
		unprintf(ERROR_LOG, "<NetRcv> -- Receive connection closed.");
		goto safe_exit;
	}
	else if ( iResult == SOCKET_ERROR )
	{
		unprintf(ERROR_LOG, "<NetRcv> -- Receive connection error.");
		goto safe_exit;
	}

safe_exit:
	return iResult;
}

int CUdpNet::NetSend(int udpsocket, sockaddr_in remote, char *sendbuf, int buflen )
{
	int sendlen = 0;

	socklen_t	remoteLen = sizeof(remote);
	//sendbuf = "YQ_SEND";
	//unprintf(DEBUG_LOG, "<YL NetSend> -- send buffer  %s ",sendbuf);
	if (sendbuf == NULL)
	{
		unprintf(ERROR_LOG, "<NetSend> -- send buffer is null ");
		goto safe_exit;
	}

	if (buflen == 0)
	{
		unprintf(ERROR_LOG, "<NetSend> -- send buffer length = 0 ");
		goto safe_exit;
	}

	sendlen= sendto(udpsocket, sendbuf, buflen, 0, (sockaddr *)(&remote), remoteLen);
	if (sendlen == SOCKET_ERROR)
		unprintf(ERROR_LOG, "<NetSend> -- Send connection error. errno:%d, strerror%s", errno, strerror(errno));

safe_exit:
	return sendlen;
}

