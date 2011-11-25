
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <stdlib.h>

#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"

#include "GUIDOEngine.h"

using namespace std;

static void usage (char* name)
{
#ifndef WIN32
	cerr << "usage: " << basename (name) << " <gmn file> pageNum" << endl;
#else
	cerr << "usage: " << name << " <gmn file> pageNum" << endl;
#endif
	exit(1);
}

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit(1);
}

int main(int argc, char **argv)
{
	if (argc != 3) usage(argv[0]);
	const char* filename = argv[1];
	int page = atoi(argv[2]);
	if (page <= 0) usage(argv[0]);

	SVGSystem sys;
	SVGDevice dev (cout, &sys);
    GuidoInitDesc gd = { &dev, 0, 0, 0 };
    GuidoInit (&gd);                   // Initialise the Guido Engine first

	GuidoErrCode err;
	ARHandler arh;
    err = GuidoParseFile (filename, &arh);
	if (err != guidoNoErr) error (err);

	GRHandler grh;
    err = GuidoAR2GR (arh, 0, &grh);
	if (err != guidoNoErr) error (err);

	err = GuidoSVGExport( grh, page, cout );
	if (err != guidoNoErr) error (err);
	return 0;
}


