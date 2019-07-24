
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
	cerr << "usage: " << name << " file" << endl;
	exit (1);
}

static ostream& operator << (std::ostream& os, const GuidoDate& d) {
	os << d.num << "/" << d.denom;
	return os;
}

static ostream& operator << (std::ostream& os, const GuidoTempo& t) {
	os << "Voice " << t.voice << " date " << t.date << ":\t" << t.text << " ";
	switch (t.type) {
		case kTextualTempo:
			break;
		case kUnitValueTempo:	os << t.unit << "=" << t.value;
			break;
		case kUnitUnitTempo:	os << t.unit << "=" << t.unitvalue;
			break;
	}
	return os;
}

int main(int argc, char **argv)
{
	if (argc != 2) usage(argv[0]);

	VGDevice* dev = gSys.CreateMemoryDevice(kSize, kSize);
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);
	
	const char* file = argv[1];
	GuidoParser *parser = GuidoOpenParser();
	ARHandler arh = GuidoFile2AR (parser, file);
	if (arh) {
		GuidoTempoList tempi;
		int n = GuidoGetTempoList(arh, tempi );
		if (n >= 0) {
			for (int i=0; i<n; i++) {
				cout << tempi[i] << endl;
			}
			GuidoFreeTempoList (tempi);
		}
		else cerr << "error: " << GuidoGetErrorString(GuidoErrCode(n)) << endl;
		GuidoFreeAR (arh);
	}
	else {
		int line, col;
		GuidoErrCode err = GuidoParserGetErrorCode (parser, line, col, 0);
		error (err);
	}

	GuidoCloseParser(parser);
	delete dev;

	return 0;
}
