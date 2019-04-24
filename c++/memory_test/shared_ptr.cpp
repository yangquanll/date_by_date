/*
	智能指针的使用 c++ 11
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
using namespace std;

class Base
{
	public:
		const int B;
		Base();
		Base(int x);
		Base(const Base &b);
		void foo(void); // inline 仅与函数声明放在一起 是没有效果的
		void foo_au(){} // --->自动地成为内联函数
		void stcp(const char *in,char *out);
		//void construct(const Base &b);
		private:
			int m_b_ma = 666;

};

inline void foo() // 好的写法 ：类内定义 类外实现
{
	cout<<"Base : inline foo() "<<endl;
}

Base::Base(int x)
	:B(x)
{
	cout<< "Base(int x) x = " <<B<<endl;
}

void Base:: stcp(const char *in,char *out)
{
	char *t;
	strcpy(out,in);
	//strcpy(in,"yq"); // 如果修改 const修饰的 入参 则会保错
}

Base::Base(const Base &b)
	:B(22)
{
	cout<<"Base(const Base &b) m_b_ma = "<<b.m_b_ma<<endl;
}

class Derived : public Base
{
	public:
		Derived(int x,int y);
	private:
		int m_d;
		int m_b;
};

Derived::Derived(int x,int y)
	:Base(x),m_d(x),m_b(y)			//在初始化表里调用基类的构造函数 1) && 也可以初始化 内部成员变量
{
	cout<<"init Base(init) m_d = "<< m_d <<endl;
	
}


int main()
{
	https://www.baidu.com/
	Base *b = new Base(7);
	//Base *bb = new Base(b);
	Base b1(2);
	Base b2 (b1);
	
	Derived *d = new Derived(88,99);
	
}