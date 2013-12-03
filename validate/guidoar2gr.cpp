
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "NullGSystem.h"

using namespace std;


static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
}

int main(int argc, char **argv)
{
	NullGSystem sys;
	VGDevice* dev = sys.CreateDisplayDevice();
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);

	for (int i=1; i < argc; i++) {
		cout << "======== guidoar2gr " << argv[i] << endl;
		GuidoErrCode err;
		ARHandler arh;
        
        GuidoParser *parser = GuidoOpenParser();

        std::ifstream ifs(argv[i], ios::in);
        if (!ifs)
            return 0;

        std::stringstream streamBuffer;
        streamBuffer << ifs.rdbuf();
        ifs.close();

        arh = GuidoString2AR(parser, streamBuffer.str().c_str());
		
        if (arh)
        {
			GRHandler grh;
			err = GuidoAR2GR(arh, 0, &grh);

			if (err != guidoNoErr)
                error(err);
			else
                GuidoFreeGR(grh);

			GuidoFreeAR(arh);
		}
		else
            error(err);

        GuidoCloseParser(parser);
	}

	delete dev;

	return 0;
}


