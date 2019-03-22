#include<iostream>
using namespace std;

 typedef struct Boot
{
	char *name;
	int pg;
	int id;
}Boots;

void prininf1(Boots *book)
{
	cout << "name  : " << book->name <<endl;
	cout << "pg  : " << book->pg <<endl;
	cout << "id  : " << book->id <<endl;
}
void prininf(Boot book)
{
	cout << "name  : " << book.name <<endl;
	cout << "pg  : " << book.pg <<endl;
	cout << "id  : " << book.id <<endl;
}


int main()
{
	Boot boot1,boot2;
	boot1.name = "c++ str";
	boot1.pg = 66;
	boot1.id = 666;

	prininf(boot1);
	prininf1(&boot1);
	return 0;
}
