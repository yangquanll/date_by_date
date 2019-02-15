#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
	try{
		int a;
		char b;
		cin>> a>>b;
		if(a == 8)
		{
			cout << " throw a = "<<a<<endl;
			throw a;
		}else
		{
			cout << " throw b ="<<b<<endl;
			throw b;
		}

	}
	    
	    catch(int i){
			cout<< "catch int i ="<< i <<endl;
		}
		catch(char *str){
			cout<< "catch char str ="<< str <<endl;
		}
		catch(...){
			cout<< "catch unkown "<<endl;
		}

	return 0;
}

