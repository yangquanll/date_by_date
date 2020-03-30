#include<iostream>
#include<string>
#include<string.h>

using namespace std;


 typedef struct Boot
{
	char *name;
	int pg;
	int id;
}Boots;

void prininf1(Boots *book)
{
	cout << "name  : " << book->name <<endl;
	cout << "pg  : " << book->pg <<endl;
	cout << "id  : " << book->id <<endl;
}
void prininf(Boot book)
{
	cout << "name  : " << book.name <<endl;
	cout << "pg  : " << book.pg <<endl;
	cout << "id  : " << book.id <<endl;
}


class device
{
	public:
		static char *buf;
	
		virtual	void fun1() {cout << "device-fun1"<<endl;}

		device(int l,int w){len =l;a = w;}

		device(){};
		virtual ~device(){}; //析构函数加virtual 防止内存泄漏
		static void setdev(const int d) {len =d;}
		static int getdev() {return len;}

	private:
		int a;
		static int len;
		void fun2(){cout<<"dev-fun2"<<endl;}

	protected:
		int b;
		void fun3() {cout<<"dev-fun3"<<endl;}
		
	friend  void test(device &dev);// 一般不放在protect private public 中 仅仅声明并且要传类对象 做参数
};

class soc :public device
{
	public:
	   void fun1(){
		   fun2();
		   fun3();
		   cout << "soc-fun1"<<endl;
	   }

	private:
	    void fun2(){cout << "soc-fun2" <<endl;}

	protected: 
	   void fun3(){cout << "soc-fun3"<<endl;}
};


void test(device &dev)
{
	cout << dev.b<<endl;
}

 int device::len; //使用前先定义 static

int main()
{ 	string yq;

	char imei[10];
	int i=88;
	sprintf(imei,"imei%d",i);
	cout<<imei<<endl;
	
	if(strlen(yq.c_str()) != 0)
	{
		cout << "not null ls "<<yq.c_str()<<endl;
	}else{
		cout << "ls null"<<endl;
	}
	
	Boot boot1,boot2;
	boot1.name = (char*)"c++ str";
//	strcpy(boot1.name, "c++ str");
	boot1.pg = 66;
	boot1.id = 666;

	prininf(boot1);
	prininf1(&boot1);

///////////////////////////////
	device *dev = new soc();
	dev->fun1();
	cout << "----so -tst----"<<endl;
	soc *so =new soc();
	so->fun1();
//////////////////////////////
	 device::setdev(88888);// set 后再次set 也是88888
	int num = device::getdev();
      device::setdev(66666);
	cout <<num<<endl;
	int num2 = device::getdev();//依然还是 88888 
	cout <<num<<endl; 

	return 0;
}
