/*
   练习 重载运算符 友元
   operator  friend
   operator：注意重载的运算的返回
   
   操作符重载的概念：
注意事项：
   将现有操作符与一个成员函数相关联，并将该操作符与其成员对象（操作数）一起使用。
1）  重载不能改变操作符的基本功能，以及该操作符的优先级顺序。
2）  重载不应改变操作符的本来含义。
3）  只能对已有的操作符进行重载，而不能重载新符号。
4）  操作符重载只对类可用。
5）  以下运算符不能被重载：
.     原点操作符（成员访问符）
*     指向成员的指针
::    作用域解析符
? :   问号条件运算符
sizeof 操作数的字节数

操作符函数的一般格式：
return_type operator op(argument list);
return_type：返回类型（要得到什么）
op：要重载的操作符
argument list：参数列表（操作数有哪些）
###################################
friend:
 友元类：在一个类中指明其他的类（或者）函数能够直接访问该类中的private和protected成员
   -注意：friend在类中的声明可以再public、protected和private的如何一个控制域中，而不影响其效果。例如，如果你在protected域中有这样的声明，那么aClass类同样可以访问该类的private成员
 友元函数：它允许一个函数不需要通过其public接口就能够访问到类中的private和protected成员变量
	-注意：友元函数 不是类成员函数 可以声明再类内或者类外 不加 类的限定符 :: 

*/

#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

class Ram
{
	private:
		int a;
		int add(int b,int c);

	friend class Box; //Box class call  Ram private data  must define in there 
};

int Ram::add(int b,int c)
{
	return b+c;
}

class Box
{
	public:
		int b;
		int l;
		int g;
			
		void set_b(int a);
		void set_l(int a);
		int count();
		int call_f();
		
		friend Box operator + (const Box& x, const Box& y);
		//friend class Ram;

	private:
		Ram *ram_f;

#if 0 //method 2
	Box operator + (const Box& x)
	{
		Box box;
		box.b = this -> b + x.b;
		box.l = this -> l + x.l;
		box.g = this -> g + x.g;
		return box;  // remember return ！！！！
	}
#endif
};

#if 1 //method 1
	Box operator + (const Box& x, const Box& y)
	{
		Box box;
		box.b = y.b + x.b;
		box.l = y.l + x.l;
		box.g = y.g + x.g;
		return box;  // remember return ！！！！
	}
#endif

void Box::set_b(int a)
{
	b = a;

}

void Box::set_l(int a)
{
	l = a;
}

int Box::count()
{
	return b * l;
}

int Box::call_f()
{
	return ram_f->add(6,7);
}

int main()
{
	int t;
	Box box1;
	Box box2;
	Box box3;
/*
	box1.set_b(1);
	box1.set_l(2);
	box2.set_b(1);
	box2.set_l(2); //same under way
*/
	
	box1.b = 1;
	box1.l = 2;
	box1.g = 3;
	box2.b = 1;
	box2.l = 2;
	box2.g = 3;

	box3 = box1 + box2;
	t =box3.count();
	cout << "box3.b = " <<box3.b<<endl;
	cout << "t = "<<t<<endl;
	
	out << "Box call Ram private data call_f =  " << box1.call_f()<<endl;
}
