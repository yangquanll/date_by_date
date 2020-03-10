/* push() 入栈
 * pop() 删除栈顶元素
 * top() 返回栈顶元素 不删除
 * size() 栈元素多少
 * empty() 堆栈为空则返回真 
 *
*/


#include<iostream>
#include<vector>
#include<stack>

using namespace std;

int main()
{
	stack<int> stk;
	for(int i =0; i<10;i++)
	{
		cout<<i<<endl;
		stk.push(i);
	}
	cout<< "stack size = "<<stk.size()<<endl;
	while(!stk.empty())
	{
		cout<< "stack top ="<<stk.top()<<endl;
		stk.pop();
	}

	cout<<"stack end size" <<stk.size()<<endl;
}
