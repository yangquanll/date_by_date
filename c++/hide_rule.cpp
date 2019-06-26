#include<iostream>
using namespace std;

class Base
{
	public:
		virtual void f(int a)
		{
			cout << "Base :: f(int )" << endl;
		}

		void g(void)
		{
			cout << "Base :: g(void)" << endl;
		}

		void h(float a)
		{
			cout << "Base :: h(float)" << endl;
		}

};

class Derived :public Base
{
	public:
		void f(int a)
		{
			cout << "Derived :: f(int )" <<endl;
		}

		void g(int a)
		{
			cout << "Derived :: g(int)" << endl;
		}

		void h(float a)
		{
			cout << "Derived :: h(float)" << endl;
		}

};

int main ()
{
	Derived d;
	Base *b;
	b = &d;
	b -> f(2);
	b -> g();
	b -> h(3.14);
	Derived *de = &d;
	printf("=============\n");
	de -> f(2);
	de -> g(3);
	//de -> g(); bulied error error: no matching function for call to 'Derived::g(0)
	// so that Derived behided Base fun
	de -> h(3.14);

}
