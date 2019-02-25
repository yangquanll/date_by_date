#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Secretary;
// 看股票的同事类（观察对象，观察者）
class StockObserver
{
public:
	StockObserver(string strName, Secretary* strSub)
	{
		name = strName;
		sub = strSub;
	}

	void Update();

private:
	string name;
	Secretary* sub;
};

// 秘书类（主题对象，通知者）
class Secretary
{

public:
	string action;
	void Add(StockObserver ob) { observers.push_back(ob); }
	void Remove(int addIndex)
	{
		if(addIndex >=0 && addIndex < observers.size())
		observers.erase(observers.begin() + addIndex);
	}
	void Notify()
	{
		vector<StockObserver>::iterator it;
		for (it=observers.begin(); it!=observers.end(); ++it)
		{
			(*it).Update();
		}
	}

private:
	vector<StockObserver> observers;
};




void StockObserver::Update()
{
	cout << name << " : " << sub->action << ", begin to work" << endl;
}

int main()
{
	// 创建通知者
	Secretary* p = new Secretary();

	// 观察者
	StockObserver* s1 = new StockObserver("Lazy", p);
	StockObserver* s2 = new StockObserver("SnowFire", p);

	// 加入通知队列
	p->Add(*s1);
	p->Add(*s2);

	char act[55];
	cin >>act;
	// 事件
	p->action = act;

	// 通知
	p->Notify();

	// 动态删除
	p->Remove(0);
	
	p->Notify();

	delete s1,s2;
	return 0;

}

