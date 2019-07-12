
/*******
 * 保证一个类仅有一个实例，并提供一个访问它的全局访问点
 * 线程安全的实现方式
 * 单例模式的要点有三个：

    单例类有且仅有一个实例
    单例类必须自行创建自己的唯一实例
    单例类必须给所有其他对象提供这一实例

从具体实现角度来说，可分为以下三点：

    提供一个 private 构造函数（防止外部调用而构造类的实例）
    提供一个该类的 static private 对象
    提供一个 static public 函数，用于创建或获取其本身的静态私有对象（例如：GetInstance()

防止产生拷贝的构造函数 Singletion singt = Singletion :: Getinstance() 会产生一个拷贝构造函数
    可以改为返回类型为指针
	可以重载 = 号运算符

*******/
#include <iostream>
#include <string>
#define TST_BETTER  1 
#define TST_UNDERSTAND 0
using namespace std;
/** common **/
class Singletion
{
	public:
		static Singletion *getSingt();
		static void Delet_Sigt(void);// 手动释放内存
	private:
		static Singletion m_sigt;
		static Singletion *m_singt; // = Singletion  singt = new Singletion() 指向单例的指针
		Singletion (const Singletion &);// 无需实现
		Singletion& operator = (const Singletion &); //无需实现 方式拷贝
		Singletion () 
		{
			cout<< "Singletion creat"<<endl;
		} //构造函数被保护
		~Singletion()
		{
			cout<< "destroy Singletion "<<endl;
		}

};

#if 0
/** 懒汉式 **/
Singletion *Singletion::m_singt = NULL;
Singletion *Singletion:: getSingt()
{
	if (m_singt =NULL)
	{
		m_singt = new Singletion();
		return m_singt;
	}
}
#else
/** 饿汉式 **/
// 竟然 没有析构  原因：此时全局数据区中，存储的并不是一个实例对象，而是一个实例对象的指针，即一个地址变量而已！实例对象呢？在堆区，因为是通过new得来的！虽然这样能够减小全局数据区的占用，把实例对象这一大坨都放到堆区。可是！如何释放资源呢？
//Singletion *Singletion:: m_singt = new Singletion();
Singletion Singletion:: m_sigt;
Singletion *Singletion:: getSingt()// 要点是 返回这个静态的变量的指针类型
{
	//static Singletion instance;
	//return &instance;
	return &m_sigt;
}

void Singletion :: Delet_Sigt(void)
{
	//delete m_singt;
}
#endif

int main()
{
	Singletion *sing1 = Singletion::getSingt();
	Singletion *sing2 = Singletion::getSingt();
	Singletion *sing3 = Singletion::getSingt();
	Singletion *sing4 = Singletion::getSingt();
	//Singletion::Delet_Sigt();// 手动释放
}


