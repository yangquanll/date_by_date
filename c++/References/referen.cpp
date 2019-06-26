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
* 一个类调用 另一个类的方法 可以用引用的方式
* 事先再 要调用的类(B)里面实例 要被调用的类（A）
***********************************
* virtual 函数用法注意
* 编译器在编译的时候，发现基类中有虚函数，此时编译器会为每个包含虚函数的类创建一个虚表（即vtable），
* 该表是一个一维数组，在这个数组中存放每个虚函数的地址。
* c++ 多态性 包括两点 一个是 类的多态 和函数的多态 类的多态可以通过 虚函数 来实现 ，函数的多态可以 用重载
* C++的多态性用一句话概括就是：在基类的函数前加上virtual关键字，在派生类中重写该函数，运行时将会根据对象的实际类型来调用相应的函数。
* 如果对象类型是派生类，就调用派生类的函数；如果对象类型是基类，就调用基类的函数。
***********************************
* virtual 应用 场景**
* 当我们一个指针指向了子类，或者引用子类那么我们希望调用函数的时候是调用子类的函数，
* 但是实际情况却不是这样。这时候就需要virtual关键字了
* 纯虚函数： 含有 它的类叫抽象类 ，不能被实例化 即new()
* 
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
#define TEST_ST_RE 0
#define TEST_ST_VIRTL 0
#define TEST_ST_VIRTL_INHER 1

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

int A::s_cs = 3; // 类外定义成员 默认是 public

#if TEST_ST_VIRTL || TEST_ST_VIRTL_INHER 
class VA
{
	public:
		virtual void fuc(){cout<<" VA virtual fuc "<<endl;}
		int m_vacount;
		static VA vab;
		VA(){cout<<" VA() "<<endl;}
		virtual ~VA(){ cout << " ~VA() call "<< endl;}
		//rtual void pure_v() = 0; // 子类必须实现 它
	
	protected:
		int m_prot;
	
	private:
		int prv;
	
	//基类的析构函数 声明为 虚函数 防止 内存泄露
	/* 原因： 原因是基类指针指向了派生类对象，而基类中的析构函数却是非virtual的，之前讲过，
	 * 虚函数是动态绑定的基础。现在析构函数不是virtual的，因此不会发生动态绑定，而是静态绑定，指针的静态类型为基类指针，
	 * 因此在delete时候只会调用基类的析构函数，而不会调用派生类的析构函数
	 */
	
};

VA VA::vab;

class VB
{
	public:
	
		VB(){ };
		void VB_fun(){cout << " VB_fun() call "<< endl; }
		virtual void fuc(){cout<<"VB fuc "<<endl;}
		bool test();
		VA vab;
		char sock_buff[4400];
	
		virtual ~VB(){ cout << " ~VB() call "<< endl;}
		
	
		private:
		protected:
	
};

class VC :public virtual VA
{
	public:
		VC(){ };
		
		virtual ~VC(){ cout << " ~VC() call "<< endl;}
		
	private:
	protected:
	

};

class VE :public virtual VA
{
	public:
		void fuc(){cout<<" VE fuc  "<<endl;}
};

class VF :public VE,public VC
{
	public:
		VF(){}
		~VF(){}
		void fuc(int c){cout<<" VF c = "<<c<<endl;}
		void pure_v(){cout<<" VF prue virtual fuc pure_v  "<<endl;}
};

#if TEST_ST_VIRTL
class VD :public VA
{
	public:
		VD(){cout << " VD()  "<< endl; };
		void fuc(){cout<<" VD fuc "<<endl;}	
		~VD(){ cout << " ~VD() call "<< endl;}
		void pure_v(){ cout<< " VD pure_v()" <<endl;}
		
		
	private:
	protected:
	

}vd_t;
#endif
/*
 *在多继承下，虚继承就是为了解决菱形继承中，
 *B,C都继承了A，D继承了B,C，那么D关于 A的引用只有一次，
 *而不是 普通继承的 对于A引用了两次
 */
#endif
bool VB :: test()
{
	vab = VA :: vab;
	memset(sock_buff,0,sizeof(sock_buff));
	memcpy(sock_buff,&vab,sizeof(vab));
	cout << "sock_buff size = "<< sizeof(sock_buff)<<endl;
	if(sock_buff == NULL)
		   return false;

	return true;
}

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
	A  aaa;
	cout<<"aaa.g_a_count"<<aaa.g_a_count<< " A::s_cs = "<<aaa.s_cs<<endl;
	
	
#endif

#if TEST_ST_VIRTL
/***** 一个经典的例子 *****/
	VA *va = new VD(); // 基类 析构函数 定义成 virtual 可以 同时 析构基类和派生类的
	VD *vd = new VD(); // 可以同时 析构基类和派生类

	SAFE_DETETE(va); //同时  析构基类和派生类的
	SAFE_DETETE(vd);
cout<< "<<<<<< base class virtual delete >>>>>>" <<endl;
/***** virtual 多态用法 *****/
	VA *vaa = new VD(); // 不加 virtual invoke VA fuc, 加上 invoke VD fuc 
	VA *vdd; //VA *vdd = new VA() error: 定义了纯虚函数 那么 不能被实例化 !!!
	vdd= &vd_t;
/* [VD *vdd = new VD()] 和 [VA *vdd = new VD()]  同时 invoke  VA()+VD() */

	vdd -> fuc();// VD fuc 子类的 函数
	vdd -> pure_v(); // 次处注意 如果类中定义了纯虚函数 那么 不能被实例化 
	SAFE_DETETE(vaa);
cout<< "<<<<<< virtual base  for Derived fuc use >>>>>>" <<endl;	
#endif
/***** virtual inheritance  *****/
#if TEST_ST_VIRTL_INHER
	VA *vf = new VF(); //如果 用 虚继承 只产生 一个 VA，如果 VC VE 不用 虚继承则 会产生两个 VA 的副本
	vf ->fuc(); 
cout<< "<<<<<< virtual inheritance >>>>>>" <<endl;
#endif

	VB *vb;
	vb->test();
	//cout << "sock_buff size = "<< sizeof(vb->sock_buff)<<endl;

	return 0;

}


