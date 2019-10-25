#include <iostream>
#include <string.h>
#include <stdio.h>
#include <linux/prctl.h>
#include <sys/prctl.h>

using namespace std;

#define CONFIG_TEED_ENV(var, size, value) \
    do { if (value && snprintf((var), (size), "%s", value) == (size)) { \
	        printf(  "[%d] cannot hold \"%s\"\n", (size), value); exit(252); } \
	   } while(0)

int main()
{
	int i = 1u<< 12;
	int ret = 0;
	cout<< "1 << 12 = "<<i<<endl;
	char *prebuilt_path_arg = "/vendor/app/t6/data/";
    char obj[1024];
	CONFIG_TEED_ENV(obj,i, prebuilt_path_arg);
	cout <<obj<<endl;
while(1)	
{
	if((ret = prctl(PR_SET_NAME,(unsigned long)"YQQQ",0,0,0)) < 0)
	{
		cout<< "error prctl"<<ret<<endl;
	}
}
}
