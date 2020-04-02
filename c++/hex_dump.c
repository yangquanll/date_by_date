#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void dump_hex(void *buf, int len)
{
	const	char *tmp = NULL;
	tmp = (char *)buf;
	for(int i = 0; i <= 128;i++)
	{
		printf("dump_hex %02x = \n",tmp[i]);	

	}	
}
		
int main (int argc ,char argv[])
{
	FILE *fp;
	char *buf;
	void *bufv;

	fp = fopen(argv[1],"rb");

	int filelen = fread(buf,sizeof(void*),1028,fp);
  dump_hex(buf, filelen);	
	

	pclose(fp);
	
	

}
