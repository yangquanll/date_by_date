/*
* c++ 基本知识回顾
* 
* new 的使用方法
* ***********************************
* new 三个动作 ：获得一块内存空间、调用构造函数、返回正确的指针
* virtual 虚函数的使用方法
* socket send buff 为object时 
* 以 new A() 这样方式的功能如下:
* 	- 在堆上分配空间
* 	- 在分配的空间上调用对象的构造函数
* 	（这也是 new 和 malloc的主要区别，是否调用构造函数）
* 
* 同理: 在调用 delete obj的时候：
* 	1. 首先调用 这个对象 的析构函数
* 	2. 然后释放这个对象的空间
* 
* new创建类对象需要指针接收，一处初始化，多处使用
* new创建类对象使用完需delete销毁
* new创建对象直接使用堆空间，而局部不用new定义类对象则使用栈空间
* new对象指针用途广泛，比如作为函数返回值、函数参数等
* 频繁调用场合并不适合new，就像new申请和释放内存一样
* 如果程序退出而没有执行delete则会造成内存泄漏
* //调用new：
* 
* 	A* pa = new A(3);
* 	那么上述动态创建一个对象的过程大致相当于以下三句话（只是大致上）：
* 	A* pa = (A*)malloc(sizeof(A));
* 	pa->A::A(3);
* 	return pa;
* 
***********************************
* & :: 使用方法
* 一个类调用 两一个类的方法 可以用引用的方式
* 事先再 要调用的类(B)里面实例化要被调用的类（A）
*/
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <string.h>
using namespace std;
#define SAFE_DETETE(p)	if(p) { delete(p); p = NULL; }
#define TEST_ST_NEW 0
#define TEST_ST_RE 1
#define TEST_ST_VIRTL 1

typedef struct car
{
	string name;
	int num;
	char *typ;
} CAR,*P_CAR;

class A
{
	public:
		A(){ };
		int g_a_count ;
		string sa = "ls yq";
		void act();
		static int s_cs;
		~A(){ };
	
	private:

};

class B // HM
{
	public:
		A abb; //for 为了B 能引用
		B(){};
	
		~B(){ };
	
	private:
	
};

int A::s_cs = 3; // 类外定义 public

class VA
{
	public:
		virtual fuc(){cout<<"virtual VA "<<endl;};
		virtual ~VA(); // 基类的析构函数 声明为 虚函数 防止 内存泄露
	/* 原因： 原因是基类指针指向了派生类对象，而基类中的析构函数却是非virtual的，之前讲过，
	 * 虚函数是动态绑定的基础。现在析构函数不是virtual的，因此不会发生动态绑定，而是静态绑定，指针的静态类型为基类指针，
	 * 因此在delete时候只会调用基类的析构函数，而不会调用派生类的析构函数
	 */
	
};

class VB
{
	virtual ~VB();
};

class VC :public virtual VA
{

};
/*
 *在多继承下，虚继承就是为了解决菱形继承中，
 *B,C都继承了A，D继承了B,C，那么D关于 A的引用只有一次，
 *而不是 普通继承的 对于A引用了两次
 */

class VD :public  VA
{
	public:
		fuc(){cout<<"virtual VA "<<endl;};
};



int main(int argc,char *argv[])
{
#if TEST_ST_NEW	
	CAR *ca_t;
	ca_t->num =99;
	cout << "ca_t = "<<ca_t->num<<endl;

	car *ca = new car();// =  car *ca = new car
	ca->name = "BAOMA";
	ca->num = 66;
	ca->typ = "QICHE";
	cout << "ca->name = "<<ca->name<<endl;
	
	A* a = new A;// = A* a = new A();
	B* b = new B(); //在堆上
	
	cout << "a->g_a_count"<<a->g_a_count<<endl;
	
	A aa; //在栈上
	int buf[sizeof(A)];  //在栈上，分配一个数组
	A *ar = new(buf) A(); //在这个数组上构造一个 对象 A
	cout << "sizeof(A) = "<< sizeof(A)<<endl;
	SAFE_DETETE(a);
	SAFE_DETETE(b);
#endif

#if TEST_ST_RE
	B* bb;
	A &abb = bb->abb; // 用引用的方法 来调用 A 的 g_a_count
	abb.g_a_count =1999;
	cout << "abb.g_a_count = "<<abb.g_a_count<<endl;
#endif

#if TEST_ST_VIRTL
	
	
#endif

	A  aaa;
	cout<<"aaa.g_a_count"<<aaa.g_a_count<< " A::s_cs = "<<aaa.s_cs<<endl;

	return 0;

}


