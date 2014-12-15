

#include <iostream>
#include <fstream>

#include "../GuidoParser.h"

using namespace std;

int main (int argc, char* argv[])
{
	cout << "Guido Parser kit test" << endl;
	for (int i=1; i<argc; i++) {
		cout << "parsing file " << argv[i] << endl;
		fstream file (argv[i], fstream::in);
        if (file.is_open()) {
            guido::GuidoParser p(&file);
            ARHandler music = p.parse();
            if (music) cout << "success" << endl;
            else cerr << "failed" << endl;
        }
        else cerr << "can't open file " << argv[i] << endl;
        
	}
	return 0;
}
