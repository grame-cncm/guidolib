/*

  Copyright (C) 2012  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include "common.cxx"

#include "guidoelement.h"
#include "AROthers.h"
#include "guidoparser.h"

//_______________________________________________________________________________
static void usage(char * name)
{
	cerr << "usage: " << basename(name) << " score  " << endl;
	cerr << "       read and print score"  << endl;
	cerr << "       " << scoredesc << endl;
	exit (-1);
}

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
	if (argc != 2) usage(argv[0]);	
	string gmn, _stdin;
	if (!gmnVal (argv[1], gmn, _stdin)) return -1;

	guidoparser r;
	Sguidoelement elt = r.parseString(gmn.c_str());

	if (elt) cout << elt << endl;
	else {
		cerr << "invalid gmn code" << endl;
		return 1;
	}
	
	return 0;
}

