#include "product.h"
#include "creat_product.h"
#include "factory.h"

class BZ_fac
{
	Icar *creat_bz()	
	{
		return new BZ();
	}
};
class BM_fac
{
	Icar *creat_bz()	
	{
		return new BM1();
	}
};


