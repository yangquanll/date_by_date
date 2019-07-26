/****
 * 输入第一个n 代表一共几个整数，并且最大是1000，
 * 要求从小到大排序，并且去除重复的数
 * 输出 排好序并且去重的数据
****/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int n,OutputArray,inputArray,c;
	c = 0;
	vector<int> dt;
    vector<int> ::iterator itd;
    cin >> n;
	cout << n <<endl;
    if(n >= 1000)
		{
			return -1;
		}
    

    while(cin >> inputArray)
    {
		cout << "inputarry = "<<inputArray<<endl;
		if(c >= 1000)
		{
			cout << "c = "<< c<<"n = " <<n <<endl;

			break;
		}
	        
        //for(int c =0;c <= 10;c++)
       // {
			dt.push_back(inputArray);
       // }
		c++;
    }
    sort(dt.begin(), dt.end());//从小到大排序 
    dt.erase(unique(dt.begin(), dt.end()),dt.end());//unique() 找出重复的数据 用erase() 删除
    for(itd = dt.begin();itd != dt.end();++itd)
    {
		OutputArray = *itd;
        cout<<"OutputArray = "<< OutputArray <<endl;
    }
    return 0;
}
