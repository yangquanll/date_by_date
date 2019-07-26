#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

int main()
{
	char *it;
	int a[26]={0};//第一個 元素是 0，而不是全部都是0
	for(int i= 0;i<26;i++)
	{
	//a[i] = ++i;
		printf("'a[i] = %d\n",a[0]++);

//		printf("a[26] = %d ,it[i] = %d \n",a[it[i]]++,it[i]);
	}
}
