
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <string>
#include <stdlib.h>

#include "GUIDOEngine.h"
#define MIDIEXPORT
#include "GUIDO2Midi.h"

using namespace std;

//______________________________________________________________________________
static void usage (char* name)
{
#ifdef WIN32
	const char * bname = name;
#else
	const char * bname = basename (name);
#endif

	cerr << "usage: " << bname << " [option] <gmn file>" << endl;
	cerr << "       converts the <gmn file> to midi file" << endl;
	cerr << "       options: -o <midi file> the output file name, defaults to <gmn file>.mid" << endl;
	exit(1);
}

//______________________________________________________________________________
static void initialize (char *argv[], string& infile, string& outfile) 
{
	int	i;
	string outopt = "-o";
	for (i=1; argv[i]; i++) {
		const char * ptr = argv[i];
		if (*ptr == '-') {
			if ((outopt == ptr) && (argv[i+1])) {
				outfile = argv[++i];
			}
			else usage (argv[0]);
		}
		else {
			infile = argv[i];
		}
	}
}

//______________________________________________________________________________
static string defaultOutName (const string& inName)
{
	string outname;
	size_t found = inName.find_last_of ('.');
	if (found != string::npos)
		outname = inName.substr(0,found) + ".mid";
	else 
		outname = inName + ".mid";
	return outname;
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
	if (argc > 4) usage(argv[0]);

	string infile;
	string outfile;
	initialize (argv, infile, outfile);
	if (infile.empty()) usage (argv[0]);
	if (outfile.empty()) outfile = defaultOutName (infile);

	cout << "converting " << infile << " to " << outfile << endl;
	GuidoErrCode err;
	ARHandler arh;
    err = GuidoParseFile (infile.c_str(), &arh);
	if (err != guidoNoErr) error (err);

    err = GuidoAR2MIDIFile (arh, outfile.c_str(), 0);
	GuidoFreeAR (arh);
	if (err != guidoNoErr) error (err);

	return 0;
}


