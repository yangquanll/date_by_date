/*
* c++ 基本知识回顾
* & :: 使用方法
* new 的使用方法
* virtual 虚函数的使用方法
* socket send buff 为object时 
*
*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#define SAFE_DETETE(p) {if((p) != NULL) delete [] (P); (p) = NULL;}

struct car
{

};

class A
{
	public:
		int g_a_count;
		virtual void act();
		static int s_cs;
};

class B
{
	public:
		A a;
};

int A::s_cs = 3;
int main(int argc,char *argv[])
{
	A = a new();
	
	return 0;
}


