#include <iostream>
#include "udpnet.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //sleep()
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include "devmanager.h"

#define SEND_FILE "yq.webm"

typedef struct send_data
{
	int sa;
	char *sb;
	char sd[1024];
}SEND_DATA;

Udp_Net *gudpnet;
FilesOperation *fo;
FILES files;
//FILES *filesl;
int main(int argc, char *argv[])
{
	//FILES *filesl = (FILES*)malloc(sizeof(FILES*));
	printf("argc = %d, *argv[] = %s\n",argc ,argv[argc-1]);
	if(argv[argc-1] == NULL)
	{
		printf("please input file \n");
		exit(0);
	}
	FILES ft = fo->get_file_op(argv[argc-1]);
	memcpy(&files,&ft,sizeof(FILES));
	printf("sizeof(FILES) = %d\n",sizeof(FILES));
	FILE *fp = fopen(argv[argc-1], "rb");
	fread(files.data,sizeof(char),MAX_DATA_SIZE,fp);
	printf("files.data = %ld\n",files.data,files.name);	
	Send *sed = new Send(files);
	//sed->start();
/*
	while(1)
{
	printf("startGetDevInfo() = %d\n",startGetDevInfo());
	sleep(2);
}*/
}

