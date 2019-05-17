/**
	udp 
	client:						server:
	 socket()						socket()
	 bind()							bind()
	 setsocket() //可选			   setsocket()
	 sendto() //writer				recvfrom() //read()
	 closesocket()// close()		closesocket()//
	
函数说明
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                   const struct sockaddr *dest_addr, socklen_t addrlen);
sendto()，是把UDP数据报发给指定地址；recvfrom()是从指定地址接收UDP数据报。

参数说明

    \s：              socket描述符。
    \buf：         UDP数据报缓存地址。
    \len：          UDP数据报长度。
    \flags：       该参数一般为0。
    \to：            sendto()函数参数，struct sockaddr_in类型，指明UDP数据发往哪里报。
    \tolen：      对方地址长度，一般为：sizeof(struct sockaddr_in)。
    \fromlen：recvfrom()函数参数，struct sockaddr_in类型，指明从哪里接收UDP数据报。
return:
- 对于sendto()函数，成功则返回实际传送出去的字符数，失败返回-1，错误原因存于errno 中。
- 对于recvfrom()函数，成功则返回接收到的字符数，失败则返回-1，错误原因存于errno中。

- int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
I/O复用输入/输出模型。select系统调用是用来让我们的程序监视多个文件句柄的状态变化的。程序会停在select这里等待，直到被监视的文件句柄中有一个或多个发生生了状态改变
- setsockopt()
https://blog.csdn.net/c1520006273/article/details/50420408
INADDR_ANY

**/	
#include <unistd.h>
#include "udpnet.h"
#include <error.h>
#include <string.h>
#define ERRSOCKET -1
#define ERRBIND -1
#define ERRSELECT -1

Udp_Net ::Udp_Net(void)
{

}

Udp_Net ::~Udp_Net(void)
{

}

int Udp_Net :: NetInit(unsigned int ip, unsigned short port)
{
	udpskt = socket(AF_INET,SOCK_DGRAM,0); // SOCK_DGRAM 指定 UDP 方式
	if(udpskt == ERRSOCKET)
	{
		printf("socket() failed\n");
		goto safe_exit; // goto 中间不能再有变量定义
	}

	sockaddr_in local;
	local.sin_family = AF_INET;// ipv4
	local.sin_addr.s_addr = htonl(ip); //网络转换接口
	local.sin_port = htons(port);
	printf("intip = %d ,ip = %s\n",inet_addr("192.168.20.166"),inet_ntoa(local.sin_addr));
	if(bind(udpskt,(struct sockaddr *) &local,sizeof(local)) == ERRSOCKET) //udp 没有 connect()
 	{
		printf("error bind()\n");
		goto safe_exit;
	}

	struct linger opt;
	memset(&opt, 0,sizeof(linger));
// SO_LINGER 延迟关闭连接
// SOL_SOCKET 通用套接字选项

if(setsockopt(udpskt,SOL_SOCKET,SO_LINGER,&opt,sizeof(struct linger)) == -1)
{
	printf("setsockt error \n");
	goto safe_exit;
}

safe_exit:
	return udpskt;
}

int Udp_Net :: NetSend(int udpskt, sockaddr_in remote_addr, char *sendbuf, int bufflen)
{
	int sendlen = 0;
	socklen_t addrlen = sizeof(remote_addr);
	sendlen = sendto(udpskt, sendbuf, bufflen, 0, (sockaddr *)(&remote_addr), addrlen);
	
	if(sendlen == ERRSOCKET)
	{
		printf("sendto error \n");
		return sendlen;
	}
	
	return sendlen;
}

int Udp_Net :: NetRcv(int udpskt, sockaddr_in &addr, char *rcvbuf, int bufflen, double timeout)
{
	int rcv = 0;
	socklen_t addrlen = sizeof(addr);
	struct timeval	tWait;
	
	int sec = timeout;
	int usec = (int)(timeout * 1000 * 1000) % (1000 * 1000);
    
	tWait.tv_sec = sec;
	tWait.tv_usec = usec;
	
	if (rcvbuf == NULL)
	{
		printf("<NetRcv> -- receive buffer is null \n");
		goto safe_exit;
	}

	if (bufflen == 0)
	{
		printf("<NetRcv> -- receive buffer length = 0 \n");
		goto safe_exit;
	}	
	
	
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(udpskt, &fd);
	
	rcv = select( udpskt + 1, &fd, NULL, NULL, &tWait); //&tWait  Blocking process unlock NULL 
	if(rcv == ERRSELECT)
	{
		printf(" receive select error \n");
		goto safe_exit;
	}
	else if(rcv == 0)
	{
		printf(" receive select timeout \n");
		goto safe_exit;
	}
	// rcv if = > 0 接收 当监视的相应的文件描述符集中满足条件时，比如说读文件描述符集中有数据到来时，内核(I/O)根据状态修改文件描述符集，并返回一个大于0 的数	

	rcv = recvfrom(udpskt, rcvbuf, bufflen, 0, (sockaddr *)(&addr), &addrlen);
	if(rcv == ERRSOCKET)
	{
		printf("rcv error \n");
		return rcv;
	}
	
safe_exit:
	return rcv;
}

void Udp_Net :: NetClose()
{
	close(udpskt);
}


//==============================================================
// recv 
//==============================================================
#define SEND_IPADDR inet_addr("192.168.20.166")
#define SEND_PORT 5500

Recv :: Recv()
{
	memset(&recvThreadId, 0, sizeof(pthread_t)); //后面是 类型
	memset(recvbuff, 0, sizeof(recvbuff));
	recv_size = 0;
	
}

Recv :: ~Recv()
{
	udpnet.NetClose();
}

/*static*/ void* Recv :: recvthread(void* param)
{	
	int bufflen;
	Recv re;
	bufflen = sizeof(re.recvbuff);
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	memset(re.recvbuff,0,bufflen);
	re.recv_size = re.udpnet.NetRcv(re.udpnet.udpskt, sockAddr, re.recvbuff, bufflen, CYCLE_MS); //5000 ms
	if(re.recv_size = ERRSOCKET)
	{
		printf("NetRcv  error \n");
		re.udpnet.NetClose();
		re.udpnet.NetInit(INADDR_ANY, SEND_PORT);
		exit(0);
	}
	printf("recv_buflen = %d,recvbuff = %s \n",re.recv_size,re.recvbuff);
}
//==============================================================
// send
//==============================================================
Send :: Send()
{
	memset(&sendThreadId, 0, sizeof(pthread_t));
	dest_ip = SEND_IPADDR;
	dest_port = SEND_PORT;
	memset(bufsed, 0, sizeof(bufsed));
	//printf(" send dest_ip =%d,dest_port = %d\n",inet_addr("192.168.20.166"),dest_port); // 打印的类型要一致 要不然会出现段错误
	send_size = 0;
}

bool Send :: start(int _dest_ip, int _dest_port)
{
	_dest_port = dest_port;
	int ret = -1;
	printf("dest_ip =%d,dest_port = %d\n",dest_ip,dest_port);
	udpnet.NetInit(INADDR_ANY, dest_port);
	
	ret = pthread_create(&this->sendThreadId,NULL,sendthread,(void *)this);
	if(ret)
	{
		printf("pthrteat_creat error \n");
		exit(0);
	}
	ret = -1;	
	ret = pthread_join(this->sendThreadId,NULL);

	if(ret)
	{
		printf("pthread_join error \n");
		exit(0);
	}
}

bool Send :: start()
{
	return start(dest_ip, dest_port);
}

Send:: ~Send()
{
	udpnet.NetClose();
}

int Send::senddata()
{
	char *sendbuff =  "CALLING YL 6666";
	int buflen = sizeof(sendbuff);
	int sendsize;
	sockaddr_in socsend;
	socsend.sin_family = AF_INET;// ipv4
	socsend.sin_addr.s_addr = this->dest_ip; //网络转换接口
	socsend.sin_port = htons(dest_port);
	
	sendsize = udpnet.NetSend(udpnet.udpskt, socsend, sendbuff, buflen);
	printf("sendsize = %d\n",sendsize);
	return sendsize;
}

/* static */void* Send :: sendthread(void* param) // 静态的函数 只能call 静态的变量或者函数
{
	
	Send *send = (Send*)param;
	if(send->senddata() == ERRSOCKET)
	{
		send->udpnet.NetClose();
		send->udpnet.NetInit(INADDR_ANY, send->dest_port);
	}
	
	sleep(1);
}



