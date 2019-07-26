/****
 * 输入字符串 输出重复的字符个数
 * 例如 输入 aaabbcc 
 *      输出 a:3,b:2,c:2
****/

#include<iostream>
#include<string.h>
#include<stdio.h>
#include <iomanip>
using namespace std;

void fc(const char *inpt,long inplen)
{
	int cont[26] ={0};
	for(int i= 0;i<inplen;i++)
	{
		int ct = inpt[i] - 'a';
		//cont[inpt[i] -'a']++;
		printf("inpt[i] -'a' = %c && %d ;; count[intpt[i] -'a'] = %c && %d \n",inpt[i] -'a', inpt[i] -'a',cont[inpt[i] -'a'],cont[inpt[i] -'a']++);

//		printf("inpt[i] -'a' =  --- %c && %d ---   \n",inpt[i] -'a', inpt[i] -'a');
//	cout<< (cont[ct]++)<<endl;
	}
	
	
	for(int i = 0;i<26;i++)
	{
		if(cont[i] == 0)
		{
		
		}
		else{

			//cout<<i+'a'<<":"<<cont[i]<<",";
			printf("%c:%d,",i+'a',cont[i]);
		}
	}
	cout<<endl;
}

int main()
{
	char s[256];
	while(cin >> s)
	{
		fc((const char*) s,strlen(s));
	
	}

	return 0;
}
