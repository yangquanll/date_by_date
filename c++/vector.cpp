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
	
	#include <algorithm>
	
 *
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


int main()
{

vector<int> vec;
	
vector<int>:: iterator it;
vector<int>::iterator begin=vec.begin();
vector<int>::iterator end=vec.end();
	
#if 1 // ;like array[]
for(int i =0;i < 10;++i)
{
	vec.push_back(i); //尾部 install data 并没有头插
	cout<<vec[i]<<",";
}
	
	
cout <<"push_back end "<<endl;
for(int a = 0;a < 6; a++)
{
	//vec.pop_back(); //尾删 例如 尾插 0-9  这里 尾删 4-9

	cout<<vec[a]<<",";
	
}
/** iterator use
1)只能读取容器中的元素，而不能修改

**/
cout <<"pop_back"<<endl;
	
// 遍历
for(it = vec.begin();it != vec.end();++it)
{
	//*it =3; error static 
	cout<< "iterator vec = "<<*it<<endl;
}

for(int i =0 ; i < vec.size();i++) //size_t 
{
	cout<< " at fu = "<< vec.at(i)<<endl;	
}
	
// 排序
reverse(vec.begin(), vec.end());//交换 

for(it = vec.begin();it != vec.end();++it)
{
	//*it =3; error static 
	cout<< "reverse iterator vec = "<<*it<<endl;
}

sort(vec.begin(), vec.end()); //从小到大
for(it = vec.begin();it != vec.end();++it)
{
	//*it =3; error static 
	cout<< "sort iterator vec = "<<*it<<endl;
}

// 查找
	it = find( vec.begin() ,vec.end(),3);
	if(it != vec.end())
	{
		cout<<"find = "<< *it<<endl;
	}
// 释放操作

#endif

return 0;
}
