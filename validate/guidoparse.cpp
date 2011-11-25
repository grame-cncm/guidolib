
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <stdlib.h>

#include "GUIDOEngine.h"

using namespace std;


int main(int argc, char **argv)
{
	for (int i=1; i < argc; i++) {
		cout << "======== parsing " << argv[i] << endl;
		GuidoErrCode err;
		ARHandler arh;
		err = GuidoParseFile (argv[i], &arh);
		if (err != guidoNoErr) 
			cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
		GuidoFreeAR (arh);
	}
	return 0;
}


