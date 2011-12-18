
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <string>
#include <stdlib.h>

#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"

#include "GUIDOEngine.h"

using namespace std;

static void usage (char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << " [-f fontfile] <gmn file> pageNum" << endl;
	exit(1);
}

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit(1);
}

static void lopts(int argc, char **argv, int& page, int& file, int& fontfile)
{
	if (argc < 3) usage(argv[0]);
	if (argc == 3) {
		page = atoi(argv[2]);
		if (page <= 0) usage(argv[0]);
		file = 1;
		fontfile = 0;
	}
	else if (argc == 5) {
		if (strcmp(argv[1], "-f")) usage(argv[0]);
		page = atoi(argv[4]);
		if (page <= 0) usage(argv[0]);
		file = 3;
		fontfile = 2;	 
	}
}

int main(int argc, char **argv)
{
	int page, font, file;
	lopts (argc, argv, page, file, font);
	const char* filename = argv[file];
	const char* fontfile = font ? argv[font] : 0;

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

	err = GuidoSVGExport( grh, page, cout, fontfile);
	if (err != guidoNoErr) error (err);
	return 0;
}


