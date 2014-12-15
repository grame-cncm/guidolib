
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


#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"
//______________________________________________________________________________
int main(int argc, char **argv)
{
	if (argc > 4) usage(argv[0]);

	string infile;
	string outfile;
	initialize (argv, infile, outfile);
	if (infile.empty()) usage (argv[0]);
	if (outfile.empty()) outfile = defaultOutName (infile);

	GuidoErrCode err;
	SVGSystem sys(0, 0);
	VGDevice *dev = sys.CreateDisplayDevice();
	GuidoInitDesc id = { dev, 0, 0, 0 };
	err = GuidoInit(&id);
	if (err != guidoNoErr)
        error(err);
	
	ARHandler arh;

    GuidoParser *parser = GuidoOpenParser();

    std::ifstream ifs(infile.c_str(), ios::in);
    if (!ifs)
        return 0;

    std::stringstream streamBuffer;
    streamBuffer << ifs.rdbuf();
    ifs.close();

    arh = GuidoString2AR(parser, streamBuffer.str().c_str());
	if (!arh) {
        int line, col;
		error(GuidoParserGetErrorCode (parser, line, col, 0));
	}

/*
	GuidoAR2MIDIFile operates using an ARHandler
	However, for an unknown reason, it fails to convert scores with chords when the
	AR handler has not been converted to GR. This is probably due to the AR to AR 
	transforms that take place before the AR to GR conversion.
	This is the reason of the GuidoAR2GR call below.
	Should be corrected in a future version.
	D.F. June 12 2013
*/
	GRHandler grh;
	err = GuidoAR2GR( arh, 0, &grh);
	if (err != guidoNoErr) error (err);

	cout << "converting " << infile << " to " << outfile << endl;
    err = GuidoAR2MIDIFile(arh, outfile.c_str(), 0);
	GuidoFreeGR (grh);
	GuidoFreeAR (arh);

	if (err != guidoNoErr)
        error(err);

    GuidoCloseParser(parser);

	return 0;
}


