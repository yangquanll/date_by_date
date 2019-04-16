#include <iostream>
using namespace std;
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
