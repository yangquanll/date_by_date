/**********
 *
    Vector<类型>标识符
    Vector<类型>标识符(最大容量)
    Vector<类型>标识符(最大容量,初始所有值)
    Int i[5]={1,2,3,4,5}
    Vector<类型>vi(I,i+2);//得到i索引值为3以后的值
    Vector< vector< int> >v; 二维向量//这里最外的<>要有空格。否则在比较旧的编译器下无法通过
	clear();//清除容器中所以数据 不会 释放内存
	data（）//打印 内存区块
	push_back(i); //尾部 install data
	pop.back(i)   //delete top data
	capacity（）// 返回vector的实际存储空间的大小，这个一般大于或等于vector元素个数，注意与size()函数的区别
	#include <algorithm> //使用 swap 
	
	vector<int> A;  //创建一个空的的容器
    vector<int> B(10,100); //创建一个10个元素,每个元素值为100
    vector<int> C(B.begin(),B.end()); //使用迭代器,可以取部分元素创建一个新的容器
    vector<int> D(C); //复制构造函数,创建一个完全一样的容器
	//重载=
    vector<int> E;
    E = B;
	
 *
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool cmp(int a,int b){
    return a > b;
}

int main()
{

vector<int> vec;
vector<int> vc_yq(5,6);
vector<int>:: iterator it;
vector<int>::iterator begin=vec.begin();
vector<int>::iterator end=vec.end();
	
#if 1 // ;like array[]
for(int i =0;i < 5;++i)
{
	vec.push_back(i); //尾部 install data 并没有头插
	cout<<vec[i]<<",";
}
	
	
cout <<"push_back end "<<endl;
for(int a = 0;a < 5; a++)
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

cout << "=== 成员 交换==="<<endl;

reverse(vec.begin(), vec.end());//交换 

for(it = vec.begin();it != vec.end();++it)
{
	//*it =3; error static 
	cout<< "reverse iterator vec = "<<*it<<endl;
}

//从小到大
cout << "===ascend sort 从小到大==="<<endl;
sort(vec.begin(), vec.end()); 
for(it = vec.begin();it != vec.end();++it)
{
	//*it =3; error static 
	cout<< "sort iterator vec = "<<*it<<endl;
}

//从小到大
cout << "===descend sort 从大到小==="<<endl;
sort(vec.begin(),vec.end(),cmp); // 按降序排序
for(it = vec.begin();it != vec.end();++it)
{
	//*it =3; error static 
	cout<< "descend iterator vec = "<<*it<<endl;
}

cout << "=== vector交换==="<<endl;
cout<< " befor vec memory dir = "<< vec.data()<<endl;
cout<< " befor vc_yq memory dir = "<< vc_yq.data()<<endl;
vec.swap(vc_yq);
cout<< " after vec memory dir = "<< vec.data()<<endl;
cout<< " after vc_yq memory dir = "<< vc_yq.data()<<endl;

for(it = vec.begin();it != vec.end();++it)
{ 
	cout<< "swap iterator vec = "<<*it<<endl;
}
	
for(it = vc_yq.begin();it != vc_yq.end();++it)
{ 
	cout<< "swap iterator vc_yq = "<<*it<<endl;
}
	
// 查找
	it = find( vec.begin() ,vec.end(),3);
	if(it != vec.end())
	{
		cout<<"find = "<< *it<<endl;
	}
	
// use unique 去重前 先排序
cout << "=== unique 使用 ==="<<endl;
int a[]={6,10,4,6,6,8,100,300,66};
vector<int> unq(a,a+9);
sort(unq.begin(),unq.end());
int t =0;
for(it = unq.begin();it != unq.end();++it)
{
	if(t > unq.capacity() )
	{
		break;
	}
	++t;
	if(*it == *(unq.begin()+t))
	{
		cout<< " *(unq.begin()+1) unique  iterator unq = "<<*(unq.begin()+t)<<endl;
		unq.erase(it);
	}
	cout<<" t = "<<t<<endl;
	cout<< " erase unique  iterator unq = "<<*it<<endl;

}	
	
for(it = unq.begin();it != unq.end();++it)
{ 
	cout<< " befor unique  iterator unq = "<<*it<<endl;
}
cout<< " =============== " <<endl;
unique(unq.begin(),unq.end());
for(it = unq.begin();it != unq.end();++it)
{ 
	cout<< " after unique  iterator unq = "<<*it<<endl;
}
// 删除问题 
/***
if(Iter == v1.end()) //要控制迭代器不能超过整个容器
  { 
  	 break;
  } 
**/


// swap memory 释放操作

vec.clear();
cout<< " size  vec memory dir = "<< vec.size()<<endl;
//cout<< " clear  vec memory dir = "<< vec.clear()<<endl;
cout<< " capacity  vec memory dir = "<< vec.capacity()<<endl;
	
cout << "=== vector release memory==="<<endl;
vector <int>().swap(vc_yq); //和下面的 方式效果相同

{ //花括号 是 为了 退出 tmp 时自动析构
 vector <int>  tmp;
 vec.swap(tmp);
}

cout<< " clear  vc_yq memory dir = "<< vc_yq.data()<<endl;
cout<< " clear  vec memory dir = "<< vec.data()<<endl;

for(it = vc_yq.begin();it != vc_yq.end();++it)
{
	//*it =3; error static 
	cout<< "swap iterator vc_yq = "<<*it<<endl;
}
#endif

return 0;
}
