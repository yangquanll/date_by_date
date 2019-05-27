#include <iostream>
#include "udpnet.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //sleep()


#define SAFE_DELETE(p) {if((p) != NULL) delete [] (p); (p) = NULL;}

typedef struct send_data
{
	int sa;
	char *sb;
	char sd[1024];
}SEND_DATA;

Udp_Net *gudpnet;

int main(int argc, char *argv[ ])
{
	Send *sed = new Send();
	sed->start();
//	while(1)
//	{
//		sleep(2);
//	}

}

