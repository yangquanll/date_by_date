#include<iostream>
#include<stdexcept>
//exception/stdexcept/new/type_info头文件中都有定义的标准异常类
using namespace std;
int main()
{
	try{
		int a,b;    char s;
		cin>>a>>b>>s;
		if(s=='y'){
			if(b==1) 
			{
				cout<< "throw b "<<endl;
//				throw "yyyyyyyyyyyyyyyyyyyyyyyyyyq";
				throw b;
// 这里throw 的型 回在后面的catch 捕捉到 !!!!!!!!!!!!!!!

				cout<< "throw b end "<<endl;
			}
			cout<<a<<"/"<<b<<"="<<a<<b<<endl;
		}
		else
			if(s=='%') {   
				if(b==0)      throw a;
				cout<<a<<"%"<<b<<"="<<a%b<<endl;
			}
			else
				cout<<"Option must be % or /."<<endl;
	}//try
	//捕获int类型的异常并处理
	catch(int i)  {       cout<<"Error occur:"<<i<<"%0"<<endl;  }
	//捕获char* 类型的异常并处理
	catch(char *str) {cout<<"Error occur:"<<str<<endl;  }
	catch(runtime_error err){ cout<<err.what()<<endl;}
	//捕获其它任何异常并处理
	catch(...){cout<<"Unkown Error"<<endl;}
	//没错误不运行catch，有错误至多运行一个catch语句块，且不再返回try语句块中。
	//不管有无异常都要运行到此处
	cout<<"Hello world"<<endl;
	return 0;
}

