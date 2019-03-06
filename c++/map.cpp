/***
 * 练习map 的基本用法
 * 设计 LRU Cache 算法
 * 使用map 和hash 组合
***/

/*
**************************

 * map insert data 方式：pair 方式 和 make_pair
 * 遍历效率 高一些 可以用 unordered_map
 * 多线程 操作map
 * hash_map:hash_map 查找速度会比map快，而且查找速度基本和数据数据量大小,希望程序尽可能少消耗内存用map,find 要比 insert快
 * key-value的存储和查找功能
**************************
*/
#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include<ctime>
#include <unordered_map>
#include <hash_map>
#include <unistd.h>

using namespace std;
using namespace __gnu_cxx; //hash_map need

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
	
#if 0 // hash_map use
		
	hash_map<int,string> hs_mp;
	hash_map<int,string> :: iterator it_hs;
	
	int x=time(0);
	cout << "====== = hs_mp 插入 data ======" <<endl;
	for(int i = 0;i < 666666; i++)
	{
		hs_mp.insert(pair<int ,string>(i,s+to_string(i)));
	}
	int y = time(0) - x;
	cout << "insert time = " << y <<endl;
	
	int s =time(0) -y;
	cout << "find time = " << s <<endl;
	it_hs = hs_mp.find(1022);
	if(it_hs != hs_mp.end())
	{
		//map,set,list迭代器不支持加法，减法运算，但可以++,--。                       
  		//auto it2 = it + 2;//NG 
		cout<< "it_hs find key 1022 = " << it_hs->second <<endl;
	}
#endif
	
	cout << "====== 遍历 data ======" <<endl;// for(), while()
	it = mp.begin();
	while(it != mp.end())
	{
		cout<< "pair it_map.key = "<< it->first <<" pair it_map.string = "<< it->second <<endl;
		++it;
	}
	
	cout << "====== modify data ======" <<endl;
	
	cout << "====== 对象的操作 ======" <<endl;
	cout << "====== 查找 data ======" <<endl;
	it = mp.find(2);
	if(it != mp.end())
	{
		cout<< "find key 2 = " << it->second <<endl;
		mp.erase(it); // key 和 value 成对 delete 不产生覆盖  全部删除用: mp.clear();  // 效果 一样 mp.erase(mp.begin(), mp.end());
		for(it =mp.begin();it != mp.end();++it)
    	{
    	    cout<< "delete key = "<< it->first <<" delete .string = "<< it->second <<endl;
    	}
	}
	
	cout << "====== 删 data ======" <<endl;
	int n = mp.size();
	cout << " map size = " << n <<endl;
	
	cout << "====== hash list map ======" <<endl;
	cout << "====== 设计 LRU Cache 算法 ======" <<endl;

    return 0;
}
