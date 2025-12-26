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
	cerr << "usage: " << name << " -once|-loop|-fbloop -low|-high  score1 score2"  << endl;
	cerr << "       applies the pitch profile of score1 to score2"  << endl;
	cerr << "       " << scoredesc << endl;
	cerr << "       the options indicate the apply mode:"  << endl;
	cerr << "             -once  : applies the pitch profile only once"  << endl;
	cerr << "             -loop  : applies the pitch profile in a loop"  << endl;
	cerr << "             -fbloop: applies the pitch profile in a forward-backward loop"  << endl;
	cerr << "             -low   : uses chords lowest pitch value"  << endl;
	cerr << "             -high  : uses chords highest pitch value"  << endl;
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
static chordPitchMode getpmode (const char * str)
{
	if (!strcmp(str, "-low")) return kUseLowest;
	if (!strcmp(str, "-high")) return kUseHighest;
	return chordPitchMode(-1);
}

//_______________________________________________________________________________
int main(int argc, char *argv[]) 
{
	if (argc != 5) usage(argv[0]);

	TApplyMode mode = getmode(argv[1]);
	if (mode < 0) usage(argv[0]);
	chordPitchMode pmode = getpmode(argv[2]);
	if (pmode < 0) usage(argv[0]);
	
	string pitch, gmn, _stdin;
	if (!gmnVal (argv[3], pitch, _stdin)) return -1;	
	if (!gmnVal (argv[4], gmn, _stdin)) return -1;	
	
	garErr err =  guidoApplyPitch(gmn.c_str(), pitch.c_str(), mode, pmode, cout);
	if (err != kNoErr) error (err);
	return err;
}
