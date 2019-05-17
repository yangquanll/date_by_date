#include <iostream>
#include "udpnet.h"
#include <pthread.h>

#define SAFE_DELETE(p) {if((p) != NULL) delete [] (p); (p) = NULL;}


bool Recv :: start()
{
	int ret;
	ret = pthread_create(&recvThreadId,NULL,recvthread,(void*)this);
	if(ret)
	{
		printf("recv pthrteat_creat error \n");
		exit(0);
	}
	ret = -1;	
	ret = pthread_join(this->recvThreadId,NULL);

	if(ret)
	{
		printf("recv pthread_join error \n");
		exit(0);
	}
}

int main(int argc, char *argv[ ])
{
	Recv *rcv = new Recv();
	rcv->start();
	SAFE_DELETE(rcv);
}

