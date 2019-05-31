#include <iostream>
#include "udpnet.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //sleep()
#include "opfiles.h"

#define SAFE_DELETE(p) {if((p) != NULL) delete [] (p); (p) = NULL;}

typedef struct send_data
{
	int sa;
	char *sb;
	char sd[1024];
}SEND_DATA;

Udp_Net *gudpnet;
FilesOperation *fo;
int main(int argc, char *argv[])
{
	printf("argc = %d, *argv[] = %s\n",argc ,argv[argc-1]);
	if(argv[argc-1] == NULL)
	{
		printf("please input file \n");
		exit(0);
	}
	fo->get_file_op(argv[argc-1]);
	Send *sed = new Send();
	sed->start();

}

