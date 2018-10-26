#include <iostream>
using namespace std;

int f(int a,int b)
{
	int n;
	n = a+b;
	cout <<n<<endl;
	return  n;

}

int main()

{
	int i = 2,p = 5;
	auto y = [=]{f(i,p);};
	auto x = [=]{return i+p;};
	x();
//	auto z = 
	cout<<x()<<endl;
}
