/*
 * do {}while(0) use
 * getopt_long_only() fun use
 * popen pclose use
 */

#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
//#include <erron.h>
#include <sys/wait.h> 
#include <getopt.h>
#include <string.h>
#include <mcheck.h>
/* 换行的话 需要 " \",后面需要加一个 空格*/
#define COPY_DATA(dts,src,len) \
	do { \
		memcpy(dts,src,len); \
		src+= len; \
	} while(0);

#define TST_POPEN 0
#define TST_GETOPT 1
#define TST_WHILE 1

using namespace std;


struct option opreat[]=
{
	{"y1",1,NULL,'a'},
	{"y2",1,NULL,'b'},
	{"y3",1,NULL,'c'},
	{0,0,0,0}
};

static void parse_args(int argc,char* argv[]){

	int c,ret = 0;
	while(ret == 0){

	c = getopt_long_only(argc,(char *const*)argv,"",opreat,NULL);
	
	if(c == -1)
	{
		break;
	}
	switch(c){

		case 'a':
			cout<<optarg<<endl;
			break;
		case 'b':
			 cout<<optarg<<endl;
			  break; 

		default:
			  ret = -1;
			  break;
	}
}
}

void tst_do(char *buf)
{
char const *p = buf;
char *o =NULL;
int o_len = 0;
COPY_DATA(&o_len,p,sizeof(int));

//mtrace();
if(o_len+1 <0)
{
	cout<< "bad len"<<endl;
}

if((o = (char *)malloc(o_len+1) ) == NULL)
{
	cout << "malloc failed"<<endl;
	free(o);
}

memset(o,0,sizeof(o));
COPY_DATA(o,p,o_len);

//setenv("MALLOC_TRACE", "taoge.log",1);

cout<<o_len<<endl;
cout<<"tst_do out = "<<o<<endl;

}


int main (int argc,char* argv[])
{
#if TST_POPEN
	FILE *fp;
	string resp;
	int *r;
	char linbuf[1000];

	if(argc > 0)
	{
		if((fp = popen(argv[1],"r")) == NULL)
		{
			cout<< argv[1]<<endl;
	   //strerror(errno)<<erron<<endl;
		}
	}
	while(fgets(linbuf,1000,fp))
	{
		resp =+ linbuf;
	}
	cout<< "resp = "<<resp<<endl;
	r = (int *)pclose(fp);
   if( r == NULL)//注意只能用这个 pclose 关闭
   {
	   cout <<"pclose failed"<<endl;
   }
	if(r)
		r = (int *)WEXITSTATUS(*r);

	cout<< r<<endl;
#endif
#if TST_GETOPT 
parse_args(argc,argv);

#endif
char *buf = "kph -c generate -n 6";
tst_do(buf);



}
