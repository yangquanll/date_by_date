#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

const char *str ="hello yq iiii";

class Solution {
public:
	void replaceSpace(const char *str,int length) {
        char *tmp;
        int count;
		printf("str = %s length = %d \n",str,length);
        if(str != NULL && length != 0) 
        {
            //memcpy(tmp, str, length);
            strlen(str);
			for(int i=0;i<length;i++)
            {
                printf("str[] =%c\n",str[i]);
				if(str[i] ==' ') 
				{  
					count++;
//					printf("if count = %d\n",count);					

					str[i] = "%20";
				}
            }
			printf("count =%d\n",count);
        }

	}
};

int main()
{
	Solution *sl;
	int len = strlen(str);
	sl->replaceSpace(str,len);
//	printf("strlen = %d\n",strlen(str));
//	printf("sizeof = %d\n",sizeof(str));
//	printf("strnlen = %d\n",strnlen(str));
	
}
