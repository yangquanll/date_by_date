#ifndef SINGLETON_H
#define SINGLETON_H
#include <string.h>
#include <iostream>
using namespace std;
class  singleton
{
public:
	static	singleton *getsingleton();
// 以自己实例为返回值的静态的公有方法，静态工厂方法
	void doaction();
	void destory();
private:
		singleton(){}
		static singleton * m_porint;
    //private static Singleton1 singleton1 = new Singleton1();
};

#endif
