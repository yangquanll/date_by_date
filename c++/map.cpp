/***
 * 使用map 和hash 组合
 * 练习map 的基本用法
 * 设计 LRU Cache 算法
***/

#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
	map<int,string> mp;
	map<int,string> :: iterator it;
	for(int i = 0;i<100; i++)
	{
		mp.inster(pair<int ,string>(i,"yq"));
	}
}
