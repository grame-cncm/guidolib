
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "GUIDOEngine.h"

using namespace std;


int main(int argc, char **argv)
{
	for (int i=1; i < argc; i++) {
		cout << "======== parsing " << argv[i] << endl;
		GuidoErrCode err;
		ARHandler arh;

        GuidoParser *parser = GuidoOpenParser();

        std::ifstream ifs(argv[i], ios::in);
        if (!ifs)
            return 0;

        std::stringstream streamBuffer;
        streamBuffer << ifs.rdbuf();
        ifs.close();

        err = GuidoNewParseString(parser, streamBuffer.str().c_str(), &arh);
		if (err != guidoNoErr) 
			cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
		GuidoFreeAR (arh);

        GuidoCloseParser(parser);
	}
	return 0;
}


