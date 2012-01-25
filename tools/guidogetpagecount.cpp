
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <stdlib.h>

#ifdef WIN32
#include "GSystemWin32.h"
GSystemWin32 gSys(0,0);
#elif __APPLE__
#include "GSystemOSX.h"
GSystemOSX gSys (0,0);
#else
#include "CairoSystem.h"
CairoSystem gSys(0);
#endif

#include "GUIDOEngine.h"
#include "VGDevice.h"

using namespace std;

#define kSize	500

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit (err);
}


int main(int argc, char **argv)
{
	VGDevice* dev = gSys.CreateMemoryDevice(kSize, kSize);
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);

	for (int i=1; i < argc; i++) {
		GuidoErrCode err;
		ARHandler arh;
		err = GuidoParseFile (argv[i], &arh);
		if (err == guidoNoErr) {
			GRHandler grh;
			err = GuidoAR2GR (arh, 0, &grh);
			if (err != guidoNoErr) error (err);
			else {
				int n = GuidoGetPageCount (grh);
				cout << argv[i] << " : " << n << endl;
				GuidoFreeGR (grh);
			}
			GuidoFreeAR (arh);
		}
		else error (err);
	}
	delete dev;
	return 0;
}


