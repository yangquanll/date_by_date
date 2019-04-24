/*
* @内存分配方式3种
* 1) 静态区创建 如全局变量 静态变量
* 2) 栈上创建 函数内局部变量的存储单元都可以在栈上创建,函数执行结束时这些存储单元自动被释放
* 3）堆上分配 由程序员 通过 malloc new  任意大小的memory 生存周期由我们自己决定 	
* **********************************************************************
* @野指针的产生与防止
*  - “野指针”不是 NULL 指针,是指向“垃圾”内存的指针
* 产生野指针有两种原因：
* 1）free(p) 后没有置  p=NULL如下例子:
		char *p = (char *) malloc(100);
			strcpy(p, “hello”);
			free(p);// p 所指的内存被释放,但是 p 所指的地址仍然不变
		...
		if(p != NULL)
		// 没有起到防错作用
		{
			strcpy(p, “world”); // 出错
		}
* 2）指针变量没有被初始化，所以指针变量在创建的同时应当被初始化,要么
     将指针设置为 NULL： char *p = NULL;char *str = (char *) malloc(100);
* 3）指针操作超越了变量的作用范围 见 Test6()
*
*/
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
using namespace std;
#define ARRAY_POINT_USE_DIFF 0 //数组和指针的注意点
#define MEMEROY_NEW_MALLOC_WA 0 // 内存安全 操作
#define MEMEROY_ERR 0 // 常见内存使用错误和面试常见题解析
#define WILD_POINT 1 //野指针测试

#define SAFE_DETETE(p)	if(NULL != p) { delete(p); p = NULL; } // delete 动态对象 后 要制成 NULL
#define SAFE_FREE(p) if(NULL != p) {free(p); p = NULL; }
#define SAFE_CHECK(p) if(NULL == p) { exit(1); } // 分配完内存后注意要检查 是否分配成功

void fun()
{
	return;
}

class MEM_TST
{
	public:
		//MEM_OU mem_ou;
		virtual void loop(){}; //注意要 加上{} 子类要实现它
		MEM_TST(){cout<< "MEM_TST"<<endl;}
		virtual ~MEM_TST(){};
		void array_point_tst();
		void wild_point_tst(){ cout<<"test wild point"<<endl;}
	private:
};

class MEM_SON :public MEM_TST
{
	public:
		MEM_TST mem_tst;
		void loop();
		MEM_SON(){};
		~MEM_SON(){};
		
};

class MEM_OU
{
	public:
		MEM_TST *mem_tst;//静态的方法 也会 调用MEM_TST() 定义这里为了 调用 MEM_TST 方法
		
	private:
	
};

void MEM_TST :: array_point_tst()
{
		char a[] = "YYLL";
		char b[sizeof(a)];
		strcpy(b ,a); // 不可用 用 b = a!!
		cout << "b = "<<b<<endl;
		int len = strlen(a);
		char *p = (char*)malloc(sizeof(a));
		p = b;
		cout << "p1 = "<<p<<endl; //编译器没有报错 但是不建议用下面这个方法
		strcpy(p ,b);
		if(strcmp(p, a) == 0) // 不建议用if（ p = a）
			cout << "p2 = "<<p<<endl;
		
}

void MEM_SON :: loop()
{
	int i = 0;
	while(1)
	{
		cout << " do -----"<<endl;
		sleep(1);
		i++;
		if(i > 6)
			return;
	}
}

#if MEMEROY_ERR
void GetMemory1(char *p, int num)
{
	p = (char *)malloc(sizeof(char) * num); // 函数体内的局部变量在函数结束时自动消亡, 动态内存会自动释放吗?
/**
	(1)指针消亡了,并不表示它所指的内存会被自动释放。
    (2)内存被释放了,并不表示指针会消亡或者成了 NULL 指针
**/
}

void Test1(void)
{
	char *str = NULL;
	GetMemory1(str, 100); // str 仍然为 NULL
	strcpy(str, "hello1");
	// 运行错误
}

void GetMemory2(char **p, int num)
{
	*p = (char *)malloc(sizeof(char) * num);
}

void Test2(void)
{
	char *str = NULL;
	GetMemory2(&str, 100);// 注意参数是 &str,而不是 str
	strcpy(str, "hello2");
	cout << "Test2 : str = "<<str<<endl;
	SAFE_FREE(str);
	
}

char *GetMemory3(char *p,int num) // or char *GetMemory3(int num)
{
	p = (char *)malloc(sizeof(char) * num);
	return p;
}

void Test3(void)
{
	char *str = NULL;
	str = GetMemory3(str,100);
	strcpy(str, "hello3");
	cout << "Test3 : str = "<<str<<endl;
	SAFE_FREE(str);
	
}

char *Getstring1(void) // 返回指向“栈内存”的指针
{
	char a[] = "Getstring1";
	return a;
}

char *Getstring2(void) // return 语句返回常量字符串
{
	char *p ="Getstring2";
	return p;
}


void Test4(void)
{
	char *str = NULL;
	str = Getstring1();// str 的内容是垃圾
	cout << "Test4 : str = "<<str<<endl;
	
}

void Test5(void)
{
	char *str = NULL;
	str = Getstring2();
	cout << "Test5 : str = "<<str<<endl;
	
}

#endif
#if WILD_POINT
void Test6(void)
{
	MEM_TST *p;
		{
			MEM_TST a;// 注意 a 的生命期
			p =&a; // p 是“野指针”
		}
	a.wild_point_tst();
}
#endif
int main()
{
#if MEMEROY_NEW_MALLOC_WA // safe 操作 内存
	MEM_TST *mem = new MEM_SON();
	SAFE_CHECK(mem);
	mem->loop(); // 调用子类的 fuc
	SAFE_DETETE(mem);
#endif
	
#if ARRAY_POINT_USE_DIFF //数组和指针的注意点
	MEM_TST mc;
	mc.array_point_tst();
cout << "********************" <<endl;	
	MEM_OU mem_ou; 
	mem_ou.mem_tst->array_point_tst();// 调用 MEM_TST class 的 function
#endif

#if MEMEROY_ERR // 常见的面试题目和使用错误
	
/**!如果函数的参数是一个指针,不要指望用该指针去申请动态内存!**/
//	Test1();// 运行错误
/**
 毛病出在函数 GetMemory 中。编译器总是要为函数的每个参数制作临时副本,
 指针参数 p 的副本是 _p,编译器使 _p = p。如果函数体内的程序修改了_p 的内容,就导致
 参数 p 的内容作相应的修改。这就是指针可以用作输出参数的原因。在本例中,_p 申请
 了新的内存,只是把_p 所指的内存地址改变了,但是 p 丝毫未变。所以函数 GetMemory
 并不能输出任何东西。事实上,每执行一次 GetMemory 就会泄露一块内存,因为没有用
 free 释放内存 解决方法如下面 两种
**/
	
/*
 1)如果非得要用指针参数去申请内存,那么应该改用“指向指针的指针”
 看如下的例子 Test2()：
*/
Test2();
cout << "********************" <<endl;	
/*
 2)由于“指向指针的指针”这个概念不容易理解,我们可以用函数返回值来传递动态
 内存。这种方法更加简单，
 下面 Test3()：
*/
Test3();
cout << "********************" <<endl;	
/*
 3)用函数返回值来传递动态内存这种方法虽然好用,但是常常有人把 return 语句用错
 了。这里强调不要用 return 语句返回指向“栈内存”的指针,因为该内存在函数结束时
 自动消亡,
*/

//	Test4(); // 编译器提出警告，输出不了正常的字符串 警告如下：

/* 
   address of local variable 'a' returned [-Wreturn-local-addr]
   ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
*/
	
Test5(); // 可以输出字符串
	
/*
函数 Test5 运行虽然不会出错,但是函数 GetString2 的设计概念却是错误的。因为
GetString2 内的“hello world”是常量字符串,位于静态存储区,它在程序生命期内恒定
不变。无论什么时候调用 GetString2,它返回的始终是同一个“只读”的内存块。
*/
#endif
#if WILD_POINT
/** wild pointer **/
	Test6();
#endif

	return 0;
		
}
