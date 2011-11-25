

#include <iostream>
#include "GUIDOEngine.h"

using namespace std;
int main(int argc, char **argv)
{
	int M, m, s;	
	GuidoGetVersionNums(&M, &m, &s);
	cout << "GUIDOEngine version " << M << '.' << m << '.' << s << endl;
	return 0;
}


