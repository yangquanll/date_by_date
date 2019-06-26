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
INADDR_ANY 允许任意ip 链接

**/	
#include <unistd.h>
#include "udpnet.h"
#include <error.h>
#include <string.h>
#include <stdlib.h>
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
	printf("NetInit start \n");
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

	printf("NetInit() udpskt = %d\n",udpskt);
#if 1
	if(bind(udpskt,(struct sockaddr *) &local,sizeof(local)) == ERRSOCKET) //udp 没有 connect()
	{
		printf("error bind()\n");
		goto safe_exit;
	}
#endif
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

	return udpskt;	
}

int Udp_Net :: NetSend(int udpskt, sockaddr_in remote_addr, FILES sfl, int bufflen)
{
	int sendlen = 0;
	socklen_t addrlen = sizeof(remote_addr);
	printf("sendto  fl.name = %s fl.data_size = %ld fl.data = %s,sizeof(FILES) = %d\n",sfl.name,sfl.data_size,sfl.data,sizeof(FILES));
	sendlen = sendto(udpskt, &sfl, bufflen, 0, (sockaddr *)(&remote_addr), addrlen);//send qian要 sleep()
	printf("[send -->] send_data_size = %d\n",sendlen);

	if(sendlen == ERRSOCKET)
	{
		printf("sendto error \n");
		return sendlen;
	}

	return sendlen;

}

int Udp_Net :: NetSend(int udpskt, sockaddr_in remote_addr, char *sendbuf, int bufflen)
{
	int sendlen = 0;
	socklen_t addrlen = sizeof(remote_addr);

	sendlen = sendto(udpskt, sendbuf, bufflen, 0, (sockaddr *)(&remote_addr), addrlen);//send qian要 sleep()

	if(sendlen == ERRSOCKET)
	{
		printf("sendto error \n");
		return sendlen;
	}

	return sendlen;

}

int Udp_Net :: NetRcv(int udpskt, sockaddr_in &addr, FILES pfl, int bufflen, double timeout)
{
	int rcv = 0;
	socklen_t addrlen = sizeof(addr);
	struct timeval	tWait;

	int sec = timeout;
	int usec = (int)(timeout * 1000 * 1000) % (1000 * 1000);

	tWait.tv_sec = 5;
	tWait.tv_usec = 0;


	if (bufflen == 0)
	{
		printf("<NetRcv> -- receive buffer length = 0 \n");
		goto safe_exit;
	}	

	printf("ip =%s port = %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
#if 1 		
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(udpskt, &fd);

	rcv = select( udpskt + 1, &fd, NULL, NULL, &tWait); //&tWait  Blocking process unlock NULL 
	printf("select rcv = %d\n",rcv);
	if(rcv == ERRSELECT)
	{
		printf(" receive select error \n");
		//	goto safe_exit;
	}
	else if(rcv == 0)
	{
		printf(" receive select timeout \n");
		goto safe_exit;
	}
	// rcv if = > 0 接收 当监视的相应的文件描述符集中满足条件时，比如说读文件描述符集中有数据到来时，内核(I/O)根据状态修改文件描述符集，并返回一个大于0 的数	
#endif
	memset(&pfl,0,sizeof(FILES));
	rcv = recvfrom(udpskt, &pfl, bufflen, 0, (sockaddr *)(&addr), &addrlen);
	printf("recv success rcv data_size = %d\n",rcv);
//	printf("fl.data_size = , pfl.revc.name = %s ,rcv = %d\n", pfl.name, rcv);

	if(rcv == ERRSOCKET)
	{
		printf("rcv error \n");
		return rcv;
	}

safe_exit:
	return rcv;
}


int Udp_Net :: NetRcv(int udpskt, sockaddr_in &addr, char *rcvbuf, int bufflen, double timeout)
{
	int rcv = 0;
	socklen_t addrlen = sizeof(addr);
	struct timeval	tWait;

	int sec = timeout;
	int usec = (int)(timeout * 1000 * 1000) % (1000 * 1000);

	tWait.tv_sec = 5;
	tWait.tv_usec = 0;

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

	printf("ip =%s port = %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
#if 1 		
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(udpskt, &fd);

	rcv = select( udpskt + 1, &fd, NULL, NULL, &tWait); //&tWait  Blocking process unlock NULL 
	printf("select rcv = %d\n",rcv);
	if(rcv == ERRSELECT)
	{
		printf(" receive select error \n");
		//	goto safe_exit;
	}
	else if(rcv == 0)
	{
		printf(" receive select timeout \n");
		goto safe_exit;
	}
	// rcv if = > 0 接收 当监视的相应的文件描述符集中满足条件时，比如说读文件描述符集中有数据到来时，内核(I/O)根据状态修改文件描述符集，并返回一个大于0 的数	
#endif
	rcv = recvfrom(udpskt, rcvbuf, bufflen, 0, (sockaddr *)(&addr), &addrlen);
	printf("recv_size = %d rcvbuf = %s \n",rcv,rcvbuf);
	memset(rcvbuf,0,sizeof(char));
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
#define SEND_IPADDR inet_addr("127.0.0.1")  //192.168.20.128 127.0.0.1
#define SEND_PORT 5500
#define RECV_PORT 5400

Recv :: Recv()
{
	memset(&recvThreadId, 0, sizeof(pthread_t)); //后面是 类型
	printf(" Recv()\n");
	//memset(recvbuff, 0, sizeof(recvbuff));

}

Recv :: ~Recv()
{
	udpnet.NetClose();
}

int Recv :: recvdata()
{
	int bufflen,recv_size;

	char recvbuff[250];
	//bufflen = sizeof(recvbuff);
	bufflen = sizeof(FILES);
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	//sockAddr.sin_addr.s_addr = SEND_IPADDR;  //注意网络序转换
	//sockAddr.sin_port = htons(SEND_PORT);  //注意网络序转换
	//memset(recvbuff, 0, sizeof(recvbuff));
	memset(&rfl, 0, sizeof(FILES));
	udpskt_recv = udpnet.NetInit(INADDR_ANY, SEND_PORT); //server client port same !

//udpskt_recv = socket(AF_INET,SOCK_DGRAM,0); // SOCK_DGRAM 指定 UDP 方式
	printf("udpskt_recv = %d\n",udpskt_recv);
	if(udpskt_recv == ERRSOCKET)
	{
		printf("socket() failed\n");

	}
	while(1)
	{
		recv_size = udpnet.NetRcv(udpskt_recv, sockAddr, rfl, bufflen, CYCLE_MS); //5000 ms
		if(recv_size == ERRSOCKET)
		{
			printf(" NetRcv  error \n");
			//udpnet.NetClose();
			//re.udpnet.NetInit(INADDR_ANY, SEND_PORT);
			//exit(0);
		}
		//sleep(1);
	}
//	printf("recv_buflen = %d,recvbuff = %s \n",recv_size,);
	return recv_size;

}
/*static*/ void* Recv :: recvthread(void* param)
{	
	Recv *re = (Recv*)param;
	if(re->recvdata() == ERRSELECT)
	{
		re->udpnet.NetClose();
		printf("recvdata error re =%d \n",re);
	}
	sleep(1);
}

bool Recv :: start()
{
	int ret;
	ret = pthread_create(&recvThreadId,NULL,recvthread,(void *)this);
	if(ret)
	{
		printf("recv pthrteat_creat error \n");
		exit(0);
	}
	ret = -1;	
	ret = pthread_join(recvThreadId,NULL);
	if(ret)
	{
		printf("recv pthread_join error \n");
		exit(0);
	}
}
//==============================================================
// end
//==============================================================
Send :: Send()
{

}

Send :: Send(FILES fl)
{
	memset(&sendThreadId, 0, sizeof(pthread_t));
	dest_ip = SEND_IPADDR;
	dest_port = SEND_PORT;
	memset(bufsed, 0, sizeof(bufsed));
	memcpy(&sfl,&fl,sizeof(FILES));//get file content
	//printf(" send dest_ip =%d,dest_port = %d\n",inet_addr("192.168.20.166"),dest_port); // 打印的类型要一致 要不然会出现段错误
	send_size = 0;
	printf("[send -- >>]fl.name = %s fl.data_size = %ld fl.data = %s,sizeof(FILES) = %d\n",fl.name,fl.data_size,fl.data,sizeof(FILES));
}

bool Send :: start(int _dest_ip, int _dest_port)
{
	_dest_port = dest_port;
	int ret = -1;
	printf("dest_ip =%d,dest_port = %d\n",dest_ip,dest_port);
	udpskt = socket(AF_INET,SOCK_DGRAM,0); // SOCK_DGRAM 指定 UDP 方式
	if(udpskt == ERRSOCKET)
	 {
			printf("socket() failed\n");
//			goto safe_exit; // goto 中间不能再有变量定义
	}

//	udpnet.NetInit(INADDR_ANY, dest_port);

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
	char sendbuff[250];
	char tmp[250] = "[yq] send- ";
	int sendsize;
	int i;
	sockaddr_in socsend;
	socsend.sin_family = AF_INET;// ipv4
	socsend.sin_addr.s_addr = this->dest_ip; //网络转换接口
	socsend.sin_port = htons(dest_port);
	while(1)
	{
		usleep(1000*1000);// 1s
#if 0
		sprintf(sendbuff," %s + %d",tmp,i++);
		int buflen = sizeof(sendbuff);
		printf(" sendbuff = %s  buflen = %d\n",sendbuff,buflen);
		sendsize = udpnet.NetSend(udpnet.udpskt, socsend, sendbuff, buflen);
		printf("sendsize = %d\n",sendsize);
#endif
		int buflen = sizeof(FILES);
		sendsize = udpnet.NetSend(udpskt, socsend, sfl, buflen);
	}
	return sendsize;
}

/* static */void* Send :: sendthread(void* param) // 静态的函数 只能call 静态的变量或者函数
{

	Send *send = (Send*)param;
	if(send->senddata() == ERRSOCKET)
	{
		printf("----->>>> re init send NetInt() \n");
		send->udpnet.NetClose();
		send->udpnet.NetInit(INADDR_ANY, send->dest_port);
	}

	sleep(1);
}


