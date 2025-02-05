/*

  Copyright (C) 2003-2012  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include "common.cxx"

//_______________________________________________________________________________
static void usage(char * name)
{
	cerr << "usage: " << basename(name) << " score midifile"  << endl;
	cerr << "       convert score to midifile"  << endl;
	cerr << "       " << scoredesc << endl;
	cerr << "       midifile: the midi file name"  << endl;
	exit (1);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	if (argc != 3) usage(argv[0]);
	
	string gmn, _stdin;
	if (!gmnVal (argv[1], gmn, _stdin)) return -1;

	const char * outfile = argv[2];	
	garErr err = guido2midifile(gmn.c_str(), outfile);
	if (err != kNoErr) error (err);
	return err;
}
