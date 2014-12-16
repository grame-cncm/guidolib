
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

//--------------------------------------------------------------------------
// utility for making correspondance between ar and directory path
//--------------------------------------------------------------------------
vector<string> fillPathsVector (char *argv[])
{
    vector<string> pathsVector;

    string fileDirectoryStr(argv[0]);
    string fileNameStr(argv[1]);

    if (fileNameStr[0] == '.' && fileNameStr[1] == '.')
    {
        for (size_t i = 0; i < 2; i++)
        {
            size_t posSlash = 0;

            for (size_t j = 0; j < fileDirectoryStr.size(); j++)
            {
                if (fileDirectoryStr[j] == '/' || fileDirectoryStr[j] == '\\')
                    posSlash = j;
            }

            fileDirectoryStr.erase(posSlash, fileDirectoryStr.size());
        }

        size_t posSlash = 0;

        for (size_t i = 0; i < fileNameStr.size(); i++)
        {
            if (fileNameStr[i] == '/' || fileNameStr[i] == '\\')
            {
                fileNameStr[i] = '\\';
                posSlash = i;
            }
        }

        fileNameStr.erase(posSlash + 1, fileNameStr.size());

        fileNameStr.erase(0, 2);
        fileDirectoryStr.append(fileNameStr);

        pathsVector.push_back(fileDirectoryStr);
    }
    else
    {
        size_t posSlash = 0;

        for (size_t i = 0; i < fileNameStr.size(); i++)
        {
            if (fileNameStr[i] == '/' || fileNameStr[i] == '\\')
            {
                fileNameStr[i] = '\\';
                posSlash = i;
            }
        }

        fileNameStr.erase(posSlash + 1, fileNameStr.size());

        pathsVector.push_back(fileNameStr);
    }

#ifdef WIN32
    // For windows
    string homePath(getenv("HOMEDRIVE"));
    homePath.append(getenv("HOMEPATH"));
#else
    // For unix
    string homePath(getenv("HOME"));
#endif
    pathsVector.push_back(homePath);

    return pathsVector;
}

int main(int argc, char **argv)
{
	int page = getIntOption (argc, argv, "-p", 1);

	if (page < 0)
    {
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
	if (!filename)						// std in mode
    {
		int c;
		while (read(0, &c, 1) == 1)
			gmn += char(c);				// read the standard input into a string
	}

	SVGSystem sys;
	VGDevice *dev = sys.CreateDisplayDevice();
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit(&gd);                    // Initialise the Guido Engine first

	GuidoErrCode err;
    ARHandler arh;

    /* For symbol-tag */
    vector<string> pathsVector = fillPathsVector(argv);

    GuidoParser *parser = GuidoOpenParser();

	if (gmn.size())
		arh = GuidoString2AR(parser, gmn.c_str());
	else
    {
        std::ifstream ifs(filename, ios::in);
        if (!ifs)
            return 0;

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

	err = GuidoAbstractExport( grh, page, cout);

	if (err != guidoNoErr)
        error(err);

    GuidoCloseParser(parser);

	return 0;
}


