#include <iostream>
using namespace std;

void output( int x);
void output( int x, float y);
void output( int x)
{
cout << " output int " << x << endl ;
}
void output( int x, float y)
{
cout << " output int " << x << " and float " << y << endl ;
}
int main(void)
{
int x=1;
float y=0.5;
//
//output(x);
// error! ambiguous call
output(x,y);
// output int 1 and float 0.5
}
