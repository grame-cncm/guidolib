/*

  Copyright (C) 2009-2012  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the GuidoAR Library use.
  
*/

#include "common.cxx"

//_______________________________________________________________________________
static void usage(const char * name)
{
	cerr << "usage: " << name << " -once|-loop|-fbloop  score1 score2"  << endl;
	cerr << "       applies the rythm of score1 to score2"  << endl;
	cerr << "       " << scoredesc << endl;
	cerr << "       the options indicate the apply mode:"  << endl;
	cerr << "             -once applies the rythmic structure only once"  << endl;
	cerr << "             -loop applies the rythmic structure in a loop"  << endl;
	cerr << "             -fbloop applies the rythmic structure in a forward-backward loop"  << endl;
	exit (1);
}

//_______________________________________________________________________________
static TApplyMode getmode (const char * str)
{
	if (!strcmp(str, "-once")) return kApplyOnce;
	if (!strcmp(str, "-loop")) return kApplyForwardLoop;
	if (!strcmp(str, "-fbloop")) return kApplyForwardBackwardLoop;
	return TApplyMode(-1);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	if (argc != 4) usage(argv[0]);

	TApplyMode mode = getmode(argv[1]);
	if (mode < 0) usage(argv[0]);

	string rythm, gmn, _stdin;
	if (!gmnVal (argv[2], rythm, _stdin)) return -1;	
	if (!gmnVal (argv[3], gmn, _stdin)) return -1;	

	garErr err = guidoApplyRythm(gmn.c_str(), rythm.c_str(), mode, cout);
	if (err != kNoErr) error (err);
	return err;
}
