
/***
	- 删除 某一个范围的 vector 的元素 可以使用 remove_if()
	- 练习使用 vector 对象类型的对象

***/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Car
{
	public:
		string name;
		int nub;
		int speed;
	Car(string na,int nb)
	{
		name = na;
		nub = nb;
	}

	Car(int p_speed)
	{
		speed = p_speed;
		cout<<"p_speed"<<p_speed<<endl;
	}
	~Car(){};
};

bool cmp(Car& ca)
{
	return ca.nub < 100; //eraes 小于 100的
}

int main()
{
	vector<Car> cars;
	vector<Car> :: iterator it;

#if 1
	#if 1
	Car ca1("yq1",666);
	Car ca2("yq2",888);
	Car ca3("yq3",999);
	Car ca4("yq4",66);
	#else
	Car ca1(666);
	Car ca2(888);
	Car ca3(999);
	Car ca4(66);
	
	#endif

	cars.push_back(ca1);
	cars.push_back(ca2);
	cars.push_back(ca3);
	cars.push_back(ca4);
#else
	cars.push_back(666);
	cars.push_back(888);
	cars.push_back(999);
	cars.push_back(66);
#endif
	cars.erase(remove_if(begin(cars),end(cars),cmp),end(cars)); //remove_if()使用
	for(it = cars.begin();it != cars.end();++it)
	{
		cout<< " car.mame = "<<(*it).name << " car.nub = "<< (*it).nub<<endl;
	}
	
	return 0;
}