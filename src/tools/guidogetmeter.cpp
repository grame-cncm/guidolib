
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
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

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "VGDevice.h"

using namespace std;

#define kSize	500

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit (err);
}

static void usage (const char* name)
{
	cerr << "usage: " << name << " voice n d file" << endl;
	cerr << "       where 'voice' is a voice number (indexed from 1)" << endl;
	cerr << "             'n' and 'd' represents a musical date expressed as a rational" << endl;
	cerr << "             'file' is a gmn file" << endl;
	exit (1);
}


int main(int argc, char **argv)
{
	if (argc != 5) usage(argv[0]);

	VGDevice* dev = gSys.CreateMemoryDevice(kSize, kSize);
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);
	
	int voice = atoi(argv[1]);
	int n = atoi(argv[2]);
	int d = atoi(argv[3]);
	const char* file = argv[4];

	GuidoErrCode err;

	GuidoParser *parser = GuidoOpenParser();
	ARHandler arh = GuidoFile2AR (parser, file);
	if (arh)
	{
		GuidoDate date = {n, d};
		GuidoMeter meter;
		err = GuidoGetMeterAt(arh, voice, date, meter );
		if (err == guidoNoErr) {
			if (meter.count[0] == 0) cout << "no meter" << endl;
			else {
				const char * sep = "";
				for (int i = 0; (i<kMaxGuidoMeterCounts) && meter.count[i]; i++) {
					cout << sep << meter.count[i];
					sep = "+";
				}
				cout << "/" << meter.unit << endl;
			}
		}
		else cerr << "error: " << GuidoGetErrorString(err) << endl;
		GuidoFreeAR (arh);
	}
	else {
		int line, col;
		err = GuidoParserGetErrorCode (parser, line, col, 0);
		error (err);
	}

	GuidoCloseParser(parser);
	delete dev;

	return 0;
}
