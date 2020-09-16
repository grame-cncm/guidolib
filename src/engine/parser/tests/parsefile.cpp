

#include <iostream>
#include <fstream>

#include "GuidoParserTest.h"

using namespace std;

int main (int argc, char* argv[])
{
	int retcode = 0;
	for (int i=1; i<argc; i++) {
		fstream file (argv[i], fstream::in);
        if (file.is_open()) {
            GuidoParser p(&file);
            retcode = p.parse();
            if (retcode) break;
        }
        else {
			cerr << "can't open file " << argv[i] << endl;
			retcode =-1;
		}
        
	}
	return retcode;
}
