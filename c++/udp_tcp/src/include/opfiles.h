#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //close()
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include <iostream>

#define MAX_DATA_SIZE 1024

typedef struct files
{
	char *name;
	char data[MAX_DATA_SIZE];
	int nalen;
	long data_size;
}FILES;

class FilesOperation
{
public:
	FilesOperation();
	~FilesOperation();
	FILES get_file_op(char *name);
};
