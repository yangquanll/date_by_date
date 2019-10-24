#include <iostream>
using namespace std;
/*
class Base
{
public:
virtual ~Base() { cout<< "~Base" << endl ; }
};
class Derived : public Base
{
public:
virtual ~Derived() { cout<< "~Derived" << endl ; }
};
int main(void)
{
Base * pB = new Derived;
// upcast
delete pB;
}
*/

class Base
{
public:Base(){}
public:
       virtual void print(){cout<<"Base"<<endl;}
};

class Derived:public Base
{
public:Derived(){}
public:
       void print(){cout<<"Derived"<<endl;}
};

int main()
{
       Base *point=new Derived();
       point->print();

		int i=1,j=8;
	   	int &k =i;
		k =j;
		cout << k << j << i <<endl;
}
