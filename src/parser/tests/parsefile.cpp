

#include <iostream>

#include "../guido.h"

using namespace std;

int main (int argc, char* argv[])
{
	cout << "Guido Parser kit test" << endl;
	for (int i=1; i<argc; i++) {
		cout << "parsing file " << argv[i] << endl;
		gd_parse (argv[i], 0);		
	}
	return 0;
}
