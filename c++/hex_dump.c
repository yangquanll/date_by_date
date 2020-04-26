#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <errno.h>

void dump_hex(void *buf, int len)
{
	char *tmp = NULL;
	tmp = (char *)buf;
	//memcpy(tmp,(char *) buf, strlen(buf)+1);
	for(int i = 0; i <= 128;i += 8)
	{
		printf("0x%02x 0x%02x  0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n",
		tmp[i+0],tmp[i+1],tmp[i+2],tmp[i+3],tmp[i+4],tmp[i+5],tmp[i+6],tmp[i+7]);	

	}	
}
		
int main(int argc, char *argv[])
{
	char file_name[10];
	FILE *fp;
	char *buf = NULL;
	void *bufv;

	strcpy(file_name, argv[1]);
	printf("input file name = %s\n",file_name);
	if(argv[1] == NULL)	{
		return -1;
	}

	if ((fp = fopen(file_name,"rb")) == NULL)
	{
		printf("failed open file erro = %s",strerror(errno));
	}

	fseek(fp, 0, SEEK_END); 
	int size = ftell(fp);

//	int filelen = fread(buf,sizeof(void *),50,fp);
	int filelen = read(fp,buf,size);

	printf("filelen = %d , size = %d\n",filelen,size);

	if(buf != NULL)
	{
	  dump_hex(buf, filelen);	
	}else{

	printf("buff =NULL\n");
	goto err;
}	

err:
	pclose(fp);

	return 0;	
	

}


