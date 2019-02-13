#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>  
#include <iterator>
#include <string>
#include <dirent.h>
#include <io.h>
#include <dirent.h>
#include<io.h>
#include<stdlib.h>
using namespace std;
vector<int>rec;

void  get_files_onpath(char *path,vector<string> &vec)
{
	struct _finddata_t fd;
	int hfiles;
	hfiles = _findfirst(path,&fd);


}

int main()
{
#if 0
	int y,q,w;
    vector<int> vec(3,2);
    vector<int>::iterator iter;
    vector<int>::iterator begin=vec.begin();
    vector<int>::iterator end=vec.end();
    cout<<"vec:";
    for(iter=begin; iter!=end; iter++)
    {
        static std::size_t i=0;
        cout<<*iter<<",";
        i++;
    }
    cout<<endl;
    cout<<"size:"<<vec.size()<<endl;
    cout<<"capacity:"<<vec.capacity()<<endl;

	for(iter=begin; iter!=end; iter++)
    {
        static std::size_t i=0;
        cout<<*iter<<",";
        i++;
    }
	y  = 6;
	rec.push_back(y);
	q = rec.size();
	cout<<rec[0]<<endl;
	cout<< q<<endl;
#endif

	vector<std::string>::iterator iter;
	return 0;
}
