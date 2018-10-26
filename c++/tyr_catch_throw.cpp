#include <iostream>
#include<stdexcept>
#include <stdio.h>
using namespace std;
void  divide(int a,int b)
{
	try {
			if(!b)
			{
				throw "b Divided 0";
			}else
			{
				printf("a/b = %d \n",a/b);
			}
		}	
}

int main()
{
	int a,b;
	cin >>a>>b;
	divide(a,b);
	catch (int i){cout<< "int error"<<endl;}
	catch (...){cout<<	"Unkown Error"<<endl;}
	catch (char str*);
}
