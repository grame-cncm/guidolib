/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

extern int gParseErrorLine;	// defined in GUIDOEngine.cpp
extern int gParseErrorCol;


#include "guido.h"
#include "TagParameter.h"
#include "ARMusicalObject.h"
#include "ARMusicalTag.h"
#include "ARFactory.h"

//#define debugParse
#ifdef debugParse
# define debug(msg)			std::cout << msg << std::endl;
# define vdebug(msg, val)	std::cout << msg << " - " << val << std::endl;
#else
# define debug(msg)
# define vdebug(msg, val)
#endif

/* -------------------------------------------------
	APPLICATION SPECIFIC SECTIONS:

	the following part has to be adapted for the
	application which is to use the GUIDO parser module
	------------------------------------------------- */

int zaehlerSet;
int accidentals;
int ndots;


/* current note properties: */
int nt_enumSet=0; /* enum of rel.dur explicitely specified? */

/* rel. dur. of current / last note: */
long int nt_enum=0;
long int nt_denom=1;


void gd_noteInit (ARFactory *f, const char *id) {
vdebug ("gd_noteInit", id);
	accidentals = 0;
	zaehlerSet = 0;
	f->createEvent(id);
}

void gd_noteAcc (ARFactory *, int n) {
	accidentals += n;
}

void gd_noteOct (ARFactory *f, int n) {
vdebug ("gd_noteOct", n);
	f->setRegister(n);
}

void gd_noteEnum (ARFactory *f, long int n) {
vdebug ("gd_noteEnum", n);
	 // numerator
	zaehlerSet = 1;
	ndots = 0;
	f->setNumerator(n);
	f->setDenominator(1);

	nt_enum=n;
	nt_denom=1;
	nt_enumSet=1;
}

void gd_noteDenom (ARFactory *f, long int n) {
vdebug ("gd_noteDenom", n);
	if (zaehlerSet == 0)
	 {
	 //  reset to standard
	 f->setNumerator(1);
	 }
	f->setDenominator(n);
	ndots = 0; // switch off dots 

	nt_denom=n;
	if(!nt_enumSet)
	 nt_enum=1;
	zaehlerSet = 0;
}

void gd_noteDot(ARFactory *) {
debug ("gd_noteDot");
	ndots = 1;
}

void gd_noteDdot(ARFactory *) {
debug ("gd_noteDdot");
	ndots = 2;
	//f->setPoints(2);
}

void gd_noteTdot(ARFactory *) {
debug ("gd_noteDdot");
	ndots = 3;
	//f->setPoints(2);
}

void gd_seqAppendNote (ARFactory *f) {
debug ("gd_seqAppendNote");
	if (accidentals)
		f->setAccidentals(accidentals);
	if (ndots)
		f->setPoints(ndots);
	f->addEvent();

}

void gd_chordInit(ARFactory *f) {
debug ("gd_chordInit");
	f->createChord();
}

void gd_chordInitNote(ARFactory *f) {
debug ("gd_chordInitNote");
	f->initChordNote();
}

void gd_chordAppendNote(ARFactory *f) {
debug ("gd_chordAppendNote");

	// factory does the same thing as with regular Notes.
	// evtl. can be optimised

	if (accidentals)
		f->setAccidentals(accidentals);
	if (ndots)
		f->setPoints(ndots);
	f->addEvent();
}

void gd_seqAppendChord(ARFactory *f) {
debug ("gd_seqAppendChord");
	f->addChord();
	// factory does nothing?

}

static void mls2dur (int n, int& outNum, int& outDenum)
{
	int base = 1000;
	int num = n / base;
	int r = n % base;
	int denum = 4;
	while (r > 10) {	// approximate musical value (10 mls accuracy)
		denum *= 2;
		base /= 2;
		num = n / base;
		r = n % base;
	}
	outNum = num;
	outDenum = denum;
}

void gd_noteAbsDur (ARFactory *f, long int n) {
vdebug ("gd_noteAbsDur", n);
	int num, denum;
	mls2dur (n, num, denum);
	gd_noteEnum (f, num);
	gd_noteDenom (f, denum);
}

void gd_seqInit(ARFactory *f) {
debug ("gd_seqInit");

	f->createVoice();

	nt_enum=1;
	nt_denom=4;
	nt_enumSet=0;
	zaehlerSet = 0;
}

void gd_seqExit(ARFactory *f) {
debug ("gd_seqExit");
	f->addVoice();
}

void gd_segmInit(ARFactory *f) {
debug ("gd_segmInit");

	f->createMusic();
	zaehlerSet = 0;
	ndots = 0;
}

void gd_segmExit(ARFactory *) {
debug ("gd_segmExit");

	// Nothing happens to gGlobalFactory -> You can now
	// retrieve the Music by running factory->getMusic();
}

void gd_segmAppendSeq(ARFactory *) {
debug ("gd_segmAppendSeq");
  // Nothing happens to gGlobalFactory
}


/* tag handling */

typedef struct {
  char type;
  char* name;
  T_REAL d;
  long l;
  char* s;
  char* unit;
  } ARGV;

char *tag;
int argc;
ARGV *argv;

void gd_tagStart(ARFactory *f, const char* id,long int no) {
vdebug ("gd_tagStart", id);
	// skip tag without slash
	if (no<=0)
		no = -1;
	f->createTag(&(id[1]),no);
}

// will be called at end of complete tag (closing bracket)
void gd_tagEnd(ARFactory *f) {
debug ("gd_tagEnd");
	f->endTag();
}

void gd_tagIntArg(ARFactory *f, long int n) {
vdebug ("gd_tagIntArg", n);
	f->addTagParameter((int) n);
}

void gd_tagFloatArg(ARFactory *f, T_REAL val) {
vdebug ("gd_tagFloatArg", val);
	f->addTagParameter(val);
}

void gd_tagArgUnit(ARFactory *f, const char *unit)
{
vdebug ("gd_tagArgUnit", unit);
	// this gets the unit for tags...

	// this is called imediatly after a value
	// with a unit has been read in 
	// (that is after gd_tagIntArg or
	//  gd_tagFloatArg)

	f->setUnit(unit);

	// Parameter needs not to be freed!
}

void gd_tagStrArg(ARFactory *f, const char *s) {
vdebug ("gd_tagStrArg", s);
	f->addTagParameter(s);
}

void gd_tagAddArg(ARFactory *f, const char *s)
{
vdebug ("gd_tagAddArg", s);
	// is called when a tagargument is complete

	// s is the name (if the parameter had a name)
	f->setParameterName(s);
	
}

void gd_tagAdd(ARFactory *f) {
	f->addTag();
//	++ sq_num_tags;
}

// will be called after arguments have been passed
// and before range starts. Can be used for checking
// if current tag can have a range or not
void gd_tagRange(ARFactory *f) {
debug ("gd_tagRange");
	f->tagRange();
}

/** \brief Called after a parse error
*/
//int gd_error(const char *msg) 
//{
//	gParseErrorLine = lnr;
//	gParseErrorCol = cnr;
//	
//	std::cerr << "Guido error: " << msg << " (line " << lnr << ", char " <<  cnr << ")" << std::endl;
//	return 0;
//}
