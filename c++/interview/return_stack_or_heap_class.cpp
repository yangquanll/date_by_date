/***
 * 提供user 一个heap或者stack 内存的class
 * 不在堆中分配对象，从而保证某种类型的类不会发生内存泄漏
	1.静态实例化对象 如 A a;的步骤为：先由编译器为对象在栈空间分配内存，移动栈顶指针，挪出适当的空间，然后在这个空间上调用构造函数形成一个对象，在对象使用完之后，调用析构函数回收内存，栈顶指针减一。
	2.动态实例化对象：new操作符，第一步执行operator new()函数在堆上分配一个内存，然后调用构造函数初始化这片空间。

 ***/
#include<iostream>
#include<string>
using namespace std;
#define TST_1 0
#define TST_2 0
#define TST_JUDGE
#if TST_1
class only_heap{

	public:
		only_heap(){

		};

		void destroy() const{
			delete this;
		}
    private:	
// this way can init class by in heap, only new()
		~only_heap(){
			cout<<" ~()"<<endl;
		};
//	protect:
};
int main()
{
	only_heap *os =new only_heap();
   //only_heap sos; // 错误! (在这里合法， 但是 当它的析构函数被隐式地 调用时，就不合法了)
	os->destroy();
   //delete sos; // 错误! 试图调用 private 析构函数
	return 0;
}
#endif
#if TST_2
class OH
{
	public:
		OH(){cout<< "ls OH()"<<endl;};
		void destroy() const
		{
			delete this;
		}
	protected:
//	private:
		~OH(){
			cout<<"destory OH()"<<endl;
		};
}; 
class Doh:public OH

{
	public:
		Doh();

//		};
		~Doh();
	private:
		OH *oh;
	
};

Doh ::Doh()
//	:oh(new OH())
{
	cout<<"is Doh()"<<endl;
}

Doh:: ~Doh()
{
	cout<< "is ~Doh()"<<endl;
	oh->destroy();
}
int main()
{
	Doh *doh = new Doh();
	delete doh;
}
#endif
class Ols
{
	public:
		Ols(){
			cout<<"Ols()"<<endl;
		}

		~Ols(){
			cout <<"~Ols()"<<endl;
		}
	private:
		static void *operator new(size_t size);
		static void operator delete (void* ptr);
};

class De :public Ols
{
	public:
		De(){
			cout <<"De()"<<endl;
		}
		~De(){
			cout<<"~De()"<<endl;
		}


};

int main()
{
	Ols ols;
//	Ols *os =new Ols();
//	De *de =new De(); 会继承 base的 private operator 的 new 会报错
	return 0;
}
