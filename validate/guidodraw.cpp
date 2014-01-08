
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

	for (int i = 1; i < argc; i++)
    {
		cout << "======== guidodraw " << argv[i] << endl;
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
			err = GuidoAR2GR (arh, 0, &grh);

			if (err != guidoNoErr)
                error(err);
			else
            {
				GuidoOnDrawDesc desc;
				desc.handle = grh;
				desc.hdc = dev;
				desc.page = 1;
				desc.updateRegion.erase = true;
				desc.scrollx = desc.scrolly = 0;
				desc.sizex = desc.sizey = 500;
				GuidoResizePageToMusic( grh );
				GuidoOnDraw (&desc);

				GuidoFreeGR (grh);
			}
			GuidoFreeAR (arh);
		}
		else {
			int line, col;
			err = GuidoParserGetErrorCode (parser, line, col, 0);
			error (err);
		}

        GuidoCloseParser(parser);
	}

	delete dev;

	return 0;
}


