#include <iostream>
#include <stdio.h>
using namespace std;

//template<class T>
template <typename T> void swap(T& s1,T& s2)
{
	T tmps;
	tmps = s1;
	s1 = s2;
	s2 = tmps;
}

class cu
{
	public:
		int min(int &a,int &b);
		int max(float &a,float &b);


};

int main()
{
	int w =111;
	int e = 222;
	float r = 333.44;
	float t =555.55;
	cout<< "swap befort "<<w<<e<<r<<t<<endl;
	::swap<int>(w,e); //error: call of overloaded 'swap<float>(float&, float&)' is ambiguous resoved  用全局的函数 覆盖c++ 库中的函数
	::swap<float>(r,t);
	cout<< "swap end "<<w<<e<<r<<t<<endl;
}
