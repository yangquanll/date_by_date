/***
 * 为什么说 饿汉式是线程安全的
 * 因为在线程访问之前，单例类就已经创建好自己的实例的引用了 ，并且再一个生命周期中一个类只会被加载化一次
使用场景：
有状态的工具类对象
频繁访问文件或者数据库的类对象
 ****/
#include <iostream>
using namespace std;

class Single
{
	public:
		static Single *Getinstance(void); 
		void sngt_tst(void);

	private:
		static Single m_singt;
		
		//operator = 是为了 防止产生拷贝构造函数
		Single(const Single &);
		Single& operator = (const Single &);
		Single(){
			cout<< "creat Single" <<endl;
		}
		~Single(){
			cout << "destroy Single" <<endl;
		}
};
// 使用饿汉式 先创建再使用
Single Single::m_singt;
// 用这个指针类型的防止内存泄漏
Single *Single:: Getinstance()
{
	return &m_singt;
}

void Single :: sngt_tst()
{
	cout<< "single fuctoin call" <<endl;

}

int main()
{
	Single *singt1 = Single :: Getinstance();
	Single *singt2 = Single :: Getinstance();
	singt2->sngt_tst();
	Single:: Getinstance()->sngt_tst();//用这个 可以不用 重载 = 了
}
