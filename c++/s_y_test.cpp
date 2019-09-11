#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
	try{
		int a;
		char b[255];
		float c;
		cin>> a>>b>>c;
		if(a == 8)
		{
			cout << " throw a = "<<a<<endl;
			throw a;
		}else
		{
			cout << " throw b ="<<b<<endl;
			throw b;
		}
		if(c >2)
		{
			throw c;
		}
	}
	    
	    catch(int i){
			cout<< "catch int i ="<< i <<endl;
		}
		catch(char *str){
			cout<< "catch char str ="<< str <<endl;
		}
		catch(float r){
			cout <<"catch float r ="<< r<<endl;
		}
		catch(...){
			cout<< "catch unkown "<<endl;
		}

	return 0;
}

