
#ifndef WIN32
#include <libgen.h>
#include <unistd.h>
#else
#include <io.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"

#define NANOSVG_IMPLEMENTATION

//------------------------------------------------------------------------------------------
// Version 1.00 (2014-12-08)
//------------------------------------------------------------------------------------------

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
	cerr << "           	-f fontfile        : include the guido font taken from fontfile" << endl;
	cerr << "           	-p pagenum         : an optional page number (default is 1)" << endl;
	cerr << "       reads the standard input when gmn file is omitted." << endl;
	cerr << "           	-voicemap  boolean : enables or not event mapping draw (default is false)" << endl;
    cerr << "           	-staffmap  boolean : enables or not staff mapping draw (default is false)" << endl;
    cerr << "           	-systemmap boolean : enables or not system mapping draw (default is false)" << endl;
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
	for (int i = 1; i < argc - 1; i++) {
		if (option == argv[i]) {
			int val = strtol(argv[i + 1], 0, 10);
			if (val)
                return val;
		}
	}

	return defaultValue;
}

static int getBoolOption (int argc, char *argv[], const std::string& option, int defaultValue)
{
	for (int i = 1; i < argc - 1; i++) {
		if (option == argv[i]) {
            if (!strcmp(argv[i + 1], "true"))
			    return true;
            else
                return false;
		}
	}

	return defaultValue;
}

static const char* getOption (int argc, char *argv[], const std::string& option, const char* defaultValue)
{
	for (int i = 1; i < argc - 1; i++) {
		if (option == argv[i])
			return argv[i + 1];
	}

	return defaultValue;
}

static const char* getFile (int argc, char *argv[])
{
	int i;
	for (i = 1; i < argc - 1; i++)
		if (*argv[i] == '-') i++;	// skip option value

	return (i < argc) ? argv[i] : 0;
}

#ifdef WIN32
const char pathSep = '\\';
#else
const char pathSep = '/';
#endif
//--------------------------------------------------------------------------
// utility for making correspondance between ar and directory path
//--------------------------------------------------------------------------
vector<string> fillPathsVector (const char *filename)
{
    vector<string> pathsVector;
	pathsVector.push_back(".");			// look first for external rsrc in the current folder
	if (filename) {
		string s(filename);
		size_t n = s.find_last_of (pathSep);
		if (n != string::npos) {		// add also the gmn file path
			cerr << "fillPathsVector add " << s.substr(0,n) << endl;
			pathsVector.push_back(s.substr(0,n));
		}
	}

#ifdef WIN32
    // For windows
    string homePath(getenv("HOMEDRIVE"));
    homePath.append(getenv("HOMEPATH"));
#else
    // For unix
    string homePath(getenv("HOME"));
#endif
    pathsVector.push_back(homePath);	// and add the HOME directory

    return pathsVector;
}

//_______________________________________________________________________________
static void check (int argc, char *argv[]) {
	if (argc > 12)
        usage(argv[0]);

	for (int i = 1; i < argc; i++) {
		const char* ptr = argv[i];
		if (*ptr++ == '-') {
			if ((*ptr != 'p') && (*ptr != 'f') && (strcmp(ptr, "staffmap")) && (strcmp(ptr, "voicemap")) && (strcmp(ptr, "systemmap")))
                usage(argv[0]);
		}
	}
}

//_______________________________________________________________________________
static bool readfile (FILE * fd, string& content) 
{
	if (!fd)
        return false;

	do {
		int c = getc(fd);
		if (feof(fd) || ferror(fd)) break;
		content += c;
	} while (true);

	return ferror(fd) == 0;
}

//_______________________________________________________________________________
int main(int argc, char **argv)
{
	check (argc, argv);
	int page = getIntOption (argc, argv, "-p", 1);

	if (page < 0) {
		cerr << "page number should be positive" << endl;
		usage(argv[0]);
	}

	const char* fontfile = getOption (argc, argv, "-f", 0);
	const char* filename = getFile (argc, argv);

	string gmn;							// a string to read the standard input
	if (!filename)						// std in mode
		readfile (stdin, gmn);

	SVGSystem sys(fontfile, 0);
	VGDevice *dev = sys.CreateDisplayDevice();
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit(&gd);                    // Initialise the Guido Engine first

	GuidoErrCode err;
    ARHandler arh;

    /* For symbol-tag */
    vector<string> pathsVector = fillPathsVector(filename);
    GuidoParser *parser = GuidoOpenParser();
	if (gmn.size())
		arh = GuidoString2AR(parser, gmn.c_str());
	else {
        std::ifstream ifs(filename, ios::in);
        if (!ifs)  return 0;
        std::stringstream streamBuffer;
        streamBuffer << ifs.rdbuf();
        ifs.close();

		arh = GuidoString2AR(parser, streamBuffer.str().c_str());
    }

	if (!arh)
        error(err);

    /* For symbol-tag */
    GuidoSetSymbolPath(arh, pathsVector);
    /******************/
	
    GRHandler grh;
    err = GuidoAR2GR (arh, 0, &grh);
	
    if (err != guidoNoErr)
        error(err);

    /**** MAPS ****/
    
    int mappingMode = kNoMapping;

    if (getBoolOption(argc, argv, "-voicemap", false))
        mappingMode += kVoiceMapping;

    if (getBoolOption(argc, argv, "-staffmap", false))
        mappingMode += kStaffMapping;

    if (getBoolOption(argc, argv, "-systemmap", false))
        mappingMode += kSystemMapping;

    /*************/

	err = GuidoSVGExport(grh, page, cout, fontfile, mappingMode);
	
    if (err != guidoNoErr)
        error(err);
    
    GuidoCloseParser(parser);

	return 0;
}


