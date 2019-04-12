

#include <iostream>
#include <fstream>

#include "../gddefs.h"

using namespace std;

static char* read (const char* file)
{
	fstream f (file, fstream::in);
	f.seekg(0, ios_base::end);
	int length = f.tellg();
	f.seekg(0, ios::beg);
	char * buffer = new char [length];
	f.read (buffer,length);
	return buffer;
}

int main (int argc, char* argv[])
{
	cout << "Guido Parser kit test" << endl;
	for (int i=1; i<argc; i++) {
		cout << "parsing buffer from " << argv[i] << endl;
		char* buffer = read (argv[i]);
		gd_parse_buffer (buffer);		
		delete buffer;
	}
	return 0;
}
