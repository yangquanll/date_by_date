#include<iostream>
#include<string>
#include <string.h>

using namespace std;

int main()
{
	char *file_name1 = "/sdcard/tee/.";
	char *file_name = "/sdcard/tee/77777777_8888888888_HBBCBBCBB.config";
	char *file_name2 = "/sdcard/tee/..";
	char *obj ="/.";


	int i1 = strlen(file_name1);
	int i2 = strlen(file_name2);
	cout << "i1 = "<<i1<<" i2 ="<<i2<<endl;
	if(strstr(file_name,obj) == NULL)
	{
		cout<<"not find "<<endl;
	}
	if(strstr(file_name1,obj) == NULL)
	{
		cout<<"not find1 "<<endl;
	}
	if(strstr(file_name2,obj) == NULL)
	{
		cout<<"not find2 "<<endl;
	}





	cout<< "strstr = "<<strstr(file_name2,obj)<<endl;
}

