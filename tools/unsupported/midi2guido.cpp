
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#define MIDIEXPORT
#include "Midi2GUIDO.h"

using namespace std;

//______________________________________________________________________________
static void usage (char* name)
{
#ifdef WIN32
	const char * bname = name;
#else
	const char * bname = basename (name);
#endif

	cerr << "usage: " << bname << " <midi file> tracknum" << endl;
	cerr << "       where tracknum is the index of the track to be converted" << endl;
	exit(1);
}

//______________________________________________________________________________
static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit(1);
}

//______________________________________________________________________________
int main(int argc, char **argv)
{
	if (argc != 3) usage(argv[0]);
	const char* file = argv[1];
	int i = atoi(argv[2]);

	string gmn;
	GuidoErrCode err = GuidoMIDIFile2AR (file, i, gmn);
	if (err) error (err);
	else cout << gmn << endl;
	return err;
}


