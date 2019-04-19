#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <string.h>
using namespace std;
#define SAFE_DETETE(p)	if(p) { delete(p); p = NULL; }

class VA
{
	public:
		void fuc(){cout<<" VA virtual fuc "<<endl;}
		int m_vacount;
		static VA vab;
		
	
		VA(){cout<<" VA() "<<endl;}
		~VA(){ cout << " ~VA() call "<< endl;}

	protected:
		int m_prot;
	
	private:
		int prv;

	
};

VA VA::vab;

class VB
{
	public:
	
		VB(){cout << " VB() call "<< endl; };
	
		bool test();
		VA vab;
		char sock_buff[4400];
	
		~VB(){ cout << " ~VB() call "<< endl;}
		
	
		private:
		protected:
	
};

bool VB :: test()
{
	vab = VA :: vab;
	memset(sock_buff,0,sizeof(sock_buff));
	memcpy(sock_buff,&vab,sizeof(vab));
	cout<<"sock_buff ="<<sock_buff<<endl;
	cout << "sock_buff size = "<< sizeof(sock_buff)<<endl;
	if(sock_buff == NULL)
		   return false;

	return true;
}

int main()
{
	VB *vb = new VB();
	vb->test();
	//cout
	
}