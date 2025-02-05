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
	cerr << "usage: " << basename(name) << " score time [voiceIndex]"  << endl;
	cerr << "       converts a score time position to an event index"  << endl;
	cerr << "       " << scoredesc << endl;
	cerr << "       time      : a time position expressed as a rational (i.e. 'n/d') where 1 is a whole note."  << endl;
	cerr << "       voiceIndex: optional voice index (defaults to the first voice)"  << endl;
	exit (1);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	if ((argc != 3) && (argc != 4)) usage(argv[0]);

	string gmn, _stdin;
	if (!gmnVal (argv[1], gmn, _stdin)) return -1;
	
	rational date;
	if (!rationalVal(argv[2], date)) usage(argv[0]);
	int voiceIndex = 1;									// default voice is 1
	if ((argc == 4) && !intVal(argv[3], voiceIndex)) usage(argv[0]);

	cout << guidoTime2Ev(gmn.c_str(), date, voiceIndex-1) << endl;
	return 0;
}
