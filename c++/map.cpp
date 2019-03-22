/***
 * 练习map和 hash_map 的基本用法
 * insert 插入
 * map 遍历
 * map find
 * object 的成员在一定范围 的find remove_if() 的替代方法
 * map erase
 * map delete key-value
 * map modify data
 * 交换 两个map
 * map vector 的memory 的释放
 * map 对象 类型的操作
 * 设计 LRU Cache 算法
***/

/*
**************************
 * map insert data 方式：pair 方式 和 make_pair
 * 遍历效率 高一些 可以用 unordered_map
 * 多线程 操作map
 * hash_map:hash_map 查找速度会比map快，而且查找速度基本和数据数据量大小,希望程序尽可能少消耗内存用map,find 要比 insert快
 * key-value的存储和查找功能
 * 改变 map的value ：1）用数组的方式 可以 cover 数据；如果已经存在的 key 再用 insert(pair ())是不会cover之前的数据
 *                  2) 可以 删除 后erase(key)，再insert(pair ())
 
**************************
*/
#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include <ctime>
#include <unordered_map>
#include <hash_map>
#include <unistd.h>

using namespace std;
using namespace __gnu_cxx; //hash_map need

class Car
{
	public:
		int nb;
		string name;
	Car(int b,string nm)
	{
		nb = b;
		name = nm;
	}
	~Car(){};
	private:
		int prin();
};

int Car:: prin()
{
	return nb;
}

bool range(Car& c)
{
	return c.nb > 40; // 大于 40 的将被删掉
}

string s ="yq_";

int main()
{
	map<int,string> mp;
	map<int,string> :: iterator it;
	
	cout << "====== 用 pair 方式插入 data ======" <<endl;
	for(int i = 0;i < 6; i++)
	{
		mp.insert(pair<int ,string>(i,s+to_string(i)));// make_pair方式 :mp.insert(make_pair(i,s+to_string(i)));
	}
	
	cout << " map size = " << mp.size() <<endl;
	cout << " map max_size() = " << mp.max_size() <<endl;//最大的个数 不一定能真正能存储的数量
	
#if 0 // hash_map use	
	hash_map<int,string> hs_mp;
	hash_map<int,string> :: iterator it_hs;
	
	int x=time(0);
	cout << "====== = hs_mp 插入 data ======" <<endl;
	cout << "" <<endl;
	for(int i = 0;i < 6; i++)
	{
		hs_mp.insert(pair<int ,string>(i,s+to_string(i)));
	}
	
	int y = time(0) - x;
	cout << "insert time = " << y <<endl;
	int s = time(0) -y;
	cout << "find time = " << s <<endl;
	it_hs = hs_mp.find(1022);// find 要比 insert 快
	if(it_hs != hs_mp.end())
	{
		//map,set,list迭代器不支持加法，减法运算，但可以++,--。                       
  		//auto it2 = it + 2;//NG 
		cout<< "it_hs find key 1022 = " << it_hs->second <<endl;
	}
#endif
	
	cout << "====== 遍历 data ======" <<endl;// for(), while()
	cout << "" <<endl;
	it = mp.begin();
	while(it != mp.end())
	{
		cout<< "pair it_map.key = "<< it->first <<" pair it_map.string = "<< it->second <<endl;
		++it;
	}
	
	cout << " map at() way  = " << mp.at(3) <<endl; // 用 at() key值方式 找 value
	cout << "====== modify data ======" <<endl;
	
	mp[4] = string("666_888_yq");
	cout << "mp modify 4 = " << mp.at(4) <<endl; //cover data success
	mp.insert(make_pair(4,"yyyqqq"));// 没有生效 ：插入已存在的键值将会失败  不会覆盖 原来value 
	cout << "mp make_pair modify 4 = " << mp.at(4) <<endl; //cover data Failed
	cout << "====== 查找 data ======" <<endl;
	cout << "" <<endl;
	it = mp.find(2);
	if(it != mp.end())
	{
		cout<< "find key 2 = " << it->second <<endl;
		mp.erase(it); // key 和 value 成对 delete 不产生覆盖  全部删除用: mp.clear();  // 效果 一样 mp.erase(mp.begin(), mp.end());
		for(it =mp.begin();it != mp.end();++it)
    	{
    	    cout<< "delete key（2） = "<< it->first <<" delete .string = "<< it->second <<endl;
    	}
	}
	
	mp.insert(make_pair(2,"yyyqqq"));	
	cout << "mp make_pair modify 2 = " << mp.at(2) <<endl; // OK
	cout << "map size = " << mp.size() <<endl; 
	cout << "====== 对象类型操作 ======" <<endl;
	cout << "" <<endl;

	map<int,Car> :: iterator itr;
	map<int,Car> cmp;
	
	Car car1(11,"bm");
	Car car2(22,"bc");
	Car car3(33,"dz");
	Car car4(44,"ad");
	Car car5(55,"jb");
	Car car6(66,"jd");
	Car car7(77,"jl");

	cmp.insert(make_pair(1,car1));// 把 object 存到map中
	cmp.insert(make_pair(2,car2));
	cmp.insert(make_pair(3,car3));
	cmp.insert(make_pair(4,car4));
	cmp.insert(make_pair(5,car5));
	cmp.insert(make_pair(6,car6));
	cmp.insert(make_pair(7,car7));
	
	for(itr = cmp.begin(); itr != cmp.end();++itr) // 类似与 vector 的 remove_if 的用法
	{
		if (range(itr-> second))
			cmp.erase(itr);
	}

	map<int,Car> tmp_map;
	cmp.swap(tmp_map); //交换两个 map 前提是这个两个 map 的 类型要一致，即 key 和 value 值类型一致
	
	for(itr = tmp_map.begin();itr != tmp_map.end(); ++itr) //遍历出 所存的 object的 成员
	{
		cout << "map object  key ="<< itr->first << " Car nb = " << (itr->second).nb << " Car nb = "<< (itr->second).name <<endl;
	}
	
	map<int,Car>().swap(tmp_map); // vector map memory 释放操作 下面 花括号方式效果一样
	
	{ // 花括号为了 退出时 自动析构
		map<int,Car> temp;
		temp.swap(tmp_map);
	}
	for(itr = tmp_map.begin();itr != tmp_map.end(); ++itr)
	{
		cout << "map object  key ="<< itr->first << " Car nb = " << (itr->second).nb << " Car nb = "<< (itr->second).name <<endl;
	}
	
	cout << "====== 设计 LRU Cache 算法 ======" <<endl;

    return 0;
}
