#include <iostream>
#include "udpnet.h"
#include <pthread.h>
#include <unistd.h> //sleep()

int main(int argc, char *argv[ ])
{
	Recv *rcv = new Recv;
	printf("come man()\n");
	rcv->start();
	//while()
}

