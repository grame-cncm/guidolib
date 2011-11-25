
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <stdlib.h>

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
		cout << "======== guidodraw " << argv[i] << endl;
		GuidoErrCode err;
		ARHandler arh;
		err = GuidoParseFile (argv[i], &arh);
		if (err == guidoNoErr) {
			GRHandler grh;
			err = GuidoAR2GR (arh, 0, &grh);
			if (err != guidoNoErr) error (err);
			else {
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
		else error (err);
	}
	delete dev;
	return 0;
}


