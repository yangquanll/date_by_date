/*
智能指针的使用 c++ 11
	- 没有free 或者 delete 指针 就是：内存泄露
为什么要用智能指针：
	- 更安全的使用 动态内存
	- 自动释放所指向的对象
	- 智能指针c++ 11 新标准库 提供(类似vector)三个模板： shared_ptr,unique_ptr,weak_ptr
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <memory>
#include <string.h>
using namespace std;

#define TST_SHARED_P 1
#define TST_UNIQUE_P 1
#define TST_WEAK_P 1 //解决类引用 环回问题

#define SAFE_FREE(p) if(NULL != p) {free(p); p = NULL; }
#define SAFE_CHECK(p) if(NULL == p) { exit(1); } // 分配完内存后注意要检查 是否分配成功
class Base
{
	public:
		const int B;
		Base();
		Base(int x);
		Base(const Base &b);
		void foo(void); // inline 仅与函数声明放在一起 是没有效果的
		void foo_au(){} // --->自动地成为内联函数
		void stcp(const char *in,char **out);
		//void construct(const Base &b);
		Derived de;
	    ~Base(){cout<< " ~Base() " <<endl;}
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

void Base:: stcp(const char *in,char **out) //使用二级 指针是 能够传参成功 const的作用是 防止修改 *in
{
	char *t;
	*out = (char *)malloc(sizeof(in)); // 开辟内存后才能 cpy 最后要安全删除
	SAFE_CHECK(out); //检查 是否malloc 成功	
	memcpy(*out,in,sizeof(in));
	cout<< "stcp() out = "<< *out<<endl;
	//SAFE_FREE(out);
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
		Base ba;
	private:
		int m_d;
		int m_b;
};

Derived::Derived(int x,int y)
	:Base(x),m_d(x),m_b(y)			//在初始化表里调用基类的构造函数 1) && 也可以初始化 内部成员变量
{
	cout<<"init Derived m_d = "<< m_d <<endl;
	
}

int main()
{
#if TST_SHARED_P
	cout<< "##### smart_pointer : tst_shared_ptr #####"<<endl;
	char *_in = "yyll52"; // char in[] = "yyll52"; 效果一样
	char *_out =NULL;
	shared_ptr<Base> sp(new Base(5656)); // 使用 智能指针 
	sp->stcp(_in,&_out);
	SAFE_CHECK(_out);
	cout<< " _out = "<<_out<<endl;
//#else
	//Base nb(5454); // 相比智能指针，这一句不会 自动析构
#endif
	
	shared_ptr<Base> ssp = make_shared<Derived>(8888888,9999); /// 传递给make_shared函数的参数必须和shared_ptr所指向类型的某个构造函数相匹配
	cout<< "use_count() = "<<ssp.use_count()<<endl;
	ssp = sp;
	cout<<"ssp.m_b_ma"<<ssp->B<<endl;
	
}