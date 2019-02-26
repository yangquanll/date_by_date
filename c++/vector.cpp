/**********
 *
    Vector<类型>标识符
    Vector<类型>标识符(最大容量)
    Vector<类型>标识符(最大容量,初始所有值)
    Int i[5]={1,2,3,4,5}
    Vector<类型>vi(I,i+2);//得到i索引值为3以后的值
    Vector< vector< int> >v; 二维向量//这里最外的<>要有空格。否则在比较旧的编译器下无法通过
	clear();//清除容器中所以数据
	push_back(i); //尾部 install data
	pop.back(i)   //delete top data
 *
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


int main()
{

vector<int> vec;
#if 1 // ;like array[]
for(int i =0;i<10;i++)
{
	vec.push_back(i); //尾部 install data
	cout<<vec[i]<<",";
}
	
	
cout <<"push_back end "<<endl;
for(int a = 0;a < 6; a++)
{
	vec.pop_back();
	cout<<vec[a]<<",";
	
}
/** iterator use
1)只能读取容器中的元素，而不能修改

**/
cout <<"pop_back"<<endl;
	
vector<int>:: iterator it;
vector<int>::iterator begin=vec.begin();
vector<int>::iterator end=vec.end();

for(it = begin;it != end;++it)
{
	//*it =3; error static 
	cout<< "iterator vec = "<<*it<<endl;
}

for(int i =0 ; i < vec.size();i++) //size_t 
{
	cout<< " at fu = "<< vec.at(i)<<endl;	
}
	

	
	
	
// 排序
// 遍历
// 查找
	it = find( vec.begin() ,vec.end(),3);
// 释放操作

#endif

return 0;
}
