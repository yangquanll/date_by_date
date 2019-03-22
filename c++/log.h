#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Log
{
	public:
		static string gettime();
		static void gettime(char *buf);
};
