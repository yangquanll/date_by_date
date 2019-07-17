#include<iostream>
#include<string>
#include<map>
using namespace std;

int main()
{
    int s =16;
    string inpt,out;
    string com;
    com = "0x";
    map<string, int>  mp;
    map<string, int > ::iterator mit;
    cin >> inpt;
    //string = "A B C D E F";
    for(int i =0;i <= 16;i++)
    {
		if(i <= 10)
        {
            mp.insert(pair<string, int >(to_string(i),i));
        }else{
            mp.insert(pair<string ,int>("A",11));
            mp.insert(pair<string ,int>("B",12));
            mp.insert(pair<string ,int>("C",13));
            mp.insert(pair<string ,int>("D",14));
            mp.insert(pair<string ,int>("E",15));
            mp.insert(pair<string ,int>("F",16));
        }
    }
    //out = inpt - com;
    inpt.erase(0,2); 
	cout << "inpt = "<<inpt<<endl;
	mit =mp.find(inpt);
    if(mit == mp.end())
	{
		cout<< "not find"<<endl;
	}else{
		cout<<mit->second<<endl;
	}
    
    return 0;
}
