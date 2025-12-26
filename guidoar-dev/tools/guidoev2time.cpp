/*

  Copyright (C) 2009-2012  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the GuidoAR Library use.
*/

#include "common.cxx"

//_______________________________________________________________________________
static void usage(char * name)
{
	cerr << "usage: " << basename(name) << " score  evIndex [voiceIndex]"  << endl;
	cerr << "       converts an event index to a time position"  << endl;
	cerr << "       " << scoredesc << endl;
	cerr << "       evIndex   : the index of the target event (1 based)"  << endl;
	cerr << "       voiceIndex: optional voice index (defaults to the first voice)"  << endl;
	exit (-1);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	if ((argc != 3) && (argc != 4)) usage(argv[0]);

	string gmn, _stdin;
	if (!gmnVal (argv[1], gmn, _stdin)) return -1;	
	int eventIndex = 0;
	if (!intVal(argv[2], eventIndex)) usage(argv[0]);
	int voiceIndex = 1;									// default voice is 1
	if ((argc == 4) && !intVal(argv[3], voiceIndex)) usage(argv[0]);

	cout << string(guidoEv2Time(gmn.c_str(), eventIndex-1, voiceIndex-1)) << endl;
	return 0;
}
