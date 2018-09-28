#include "product.h"
#include "creat_factory.h"
#include "creat_product.h"
#include <iostream>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
#endif

int main()
{
	Factory *fac = new Bz();
	Icar *car = fac->Creat();
	cout << "car name = "<< car->name()<<endl;
}
