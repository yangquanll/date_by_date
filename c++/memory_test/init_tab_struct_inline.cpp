/*
 -构造函数的初始化表：
	构造函数有个特殊的初始化方式叫“初始化表达式表”(简称初始化表)。初始化表
	位于函数参数表之后,却在函数体 {} 之前。这说明该表里的初始化工作发生在函数体
	内的任何代码被执行之前
 使用规则：
	1）如果类存在继承关系,派生类必须在其初始化表里调用基类的构造函数
	2）类的 const 常量，只能在初始化表里被初始化,因为它不能在函数体内用赋值的方式来初始化
	3）类的数据成员的初始化可以采用初始化表或函数体内赋值两种方式,这两种方式的效率不完全相同。非内部数据类型的成员对象应当采用第一种方式初始化,以获取更高的效率
 -内联函数的规则：
 	1）自动内联
	2）inline 不应该出现在函数的声明中
 -const使用注意：
 	1）
 -拷贝构造函数和赋值构造函数使用方法
 -const 修饰函数的参数
 	1)如果输入参数采用“指针传递”,那么加 const 修饰可以防止意外地改动该指针,起
	到保护作用。
		例如 StringCopy 函数:
		void StringCopy(char *strDestination, const char *strSource);
		其中 strSource 是输入参数,strDestination 是输出参数。给 strSource 加上 const 修饰后,
		如果函数体内的语句试图改动 strSource 的内容,编译器将指出错误。
	2)如果输入参数采用“值传递”,由于函数将自动产生临时变量用于复制该参数,该输
		入参数本来就无需保护,所以不要加 const 修饰。
		例如不要将函数 void Func1(int x) 写成 void Func1(const int x)。同理不要将函数 void
		func2(A a) 写成 void Func2(const A a)。其中 A 为用户自定义的数据类型
	3)
		-对于非内部数据类型的输入参数,应该将“值传递”的方式改为“const 引用传
		递”,目的是提高效率。例如将 void Func(A a) 改为 void Func(const A &a)。
		-对于内部数据类型的输入参数,不要将“值传递”的方式改为“const 引用传递”。
		否则既达不到提高效率的目的,又降低了函数的可理解性。例如 void Func(int x) 不
		应该改为 void Func(const int &x)。
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
	https://www.baidu.com/ //不会编译出错
	Base *b = new Base(7);
	//Base *bb = new Base(b);
	Base b1(2);
	Base b2 (b1);
	
	Derived *d = new Derived(88,99);
	
}