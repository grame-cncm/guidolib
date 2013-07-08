
#ifndef WIN32
#include <libgen.h>
#include <unistd.h>
#else
#include <io.h>
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
	cerr << "usage: " << tool << " [options] <gmn file>" << endl;
	cerr << "       convert GMN code to svg" << endl;
	cerr << "       options:" << endl;
	cerr << "           	-f fontfile : include the guido font taken from fontfile" << endl;
	cerr << "           	-p pagenum  : an optional page number (default is 1)" << endl;
	cerr << "       reads the standard input when gmn file is omitted." << endl;
	exit(1);
}

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit(1);
}



//--------------------------------------------------------------------------
// utility for command line arguments 
//--------------------------------------------------------------------------
static int getIntOption (int argc, char *argv[], const std::string& option, int defaultValue)
{
	for (int i=1; i < argc-1; i++) {
		if (option == argv[i]) {
			int val = strtol( argv[i+1], 0, 10);
			if (val) return val;
		}
	}
	return defaultValue;
}
static const char* getOption (int argc, char *argv[], const std::string& option, const char* defaultValue)
{
	for (int i=1; i < argc-1; i++) {
		if (option == argv[i])
			return argv[i+1];
	}
	return defaultValue;
}
static const char* getFile (int argc, char *argv[])
{
	int i;
	for (i=1; i < argc-1; i++) {
		if (*argv[i] == '-') i++;	// skip option value
	}
	return (i < argc) ? argv[i] : 0;
}

//static void lopts(int argc, char **argv, int& page, int& file, int& fontfile)
//{
//	page = 1;
//	if (argc < 3) usage(argv[0]);
//	if (argc == 3) {
//		page = atoi(argv[2]);
//		if (page <= 0) usage(argv[0]);
//		file = 1;
//		fontfile = 0;
//	}
//	else if (argc == 5) {
//		if (strcmp(argv[1], "-f")) usage(argv[0]);
//		page = atoi(argv[4]);
//		if (page <= 0) usage(argv[0]);
//		file = 3;
//		fontfile = 2;	 
//	}
//}

int main(int argc, char **argv)
{
	int page = getIntOption (argc, argv, "-p", 1);
	if (page < 0) {
		cerr << "page number should be positive" << endl;
		usage(argv[0]);
	}
	const char* fontfile = getOption (argc, argv, "-f", 0);
	const char* filename = getFile (argc, argv);

//	int page, font, file;
//	lopts (argc, argv, page, file, font);
//	const char* filename = argv[file];
//	const char* fontfile = font ? argv[font] : 0;

	string gmn;							// a string to read the standard input
	if (!filename) {					// std in mode
		int c;
		while (read(0, &c, 1) == 1)
			gmn += char(c);				// read the standard input into a string
	}


	SVGSystem sys;
	SVGDevice dev (cout, &sys);
    GuidoInitDesc gd = { &dev, 0, 0, 0 };
    GuidoInit (&gd);                   // Initialise the Guido Engine first

	GuidoErrCode err;
	ARHandler arh;
	if (gmn.size())
		err = GuidoParseString (gmn.c_str(), &arh);
	else
		err = GuidoParseFile (filename, &arh);
	if (err != guidoNoErr) error (err);

	GRHandler grh;
    err = GuidoAR2GR (arh, 0, &grh);
	if (err != guidoNoErr) error (err);

	err = GuidoSVGExport( grh, page, cout, fontfile);
	if (err != guidoNoErr) error (err);
	return 0;
}


