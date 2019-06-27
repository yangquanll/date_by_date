#include<iostream>

namespace clas_ts{

	class de
	{
		public:
		char *yq = "yyyyyyyqqqq";
			int u =9999;
			void tst();
	};

	void de::tst()
	{	
		std::cout<<"de tst call "<<std::endl;
	}
}

namespace ct{

	int y = 1111;
	float x = 3.1415926;
}

int main()
{
	using namespace clas_ts;
	de d;
	std::cout<< "class de  = "<< d.u<<std::endl;
//	using ct;
	std::cout<< "namespace ct  = "<< ct::x<<std::endl;

}

