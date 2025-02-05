/*

  Copyright (C) 2012  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include "common.cxx"

//_______________________________________________________________________________
static void usage(char * name)
{
	cerr << "usage: " << basename(name) << " score  " << endl;
	cerr << "       check score syntax"  << endl;
	cerr << "       " << scoredesc << endl;
	exit (-1);
}

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
	if (argc != 2) usage(argv[0]);	
	string gmn, _stdin;
	if (!gmnVal (argv[1], gmn, _stdin)) return -1;
	if (!guidocheck (gmn.c_str())) {
		cerr << "invalid gmn code" << endl;
		return 1;
	}
	return 0;
}

