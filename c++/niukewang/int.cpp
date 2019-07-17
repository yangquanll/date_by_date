#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int n,OutputArray,inputArray,c;
	c = 0;
	vector<int> dt;
    vector<int> ::iterator itd;
    cin >> n;
	cout << n <<endl;
    if(n >= 1000)
		{
			return -1;
		}
    

    while(cin >> inputArray)
    {
		cout << "inputarry = "<<inputArray<<endl;
		if(c >= 1000)
		{
			cout << "c = "<< c<<"n = " <<n <<endl;

			break;
		}
	        
        //for(int c =0;c <= 10;c++)
       // {
			dt.push_back(inputArray);
       // }
		c++;
    }
    sort(dt.begin(), dt.end());
    dt.erase(unique(dt.begin(), dt.end()),dt.end());
    for(itd = dt.begin();itd != dt.end();++itd)
    {
		OutputArray = *itd;
        cout<<"OutputArray = "<< OutputArray <<endl;
    }
    return 0;
}
