/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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


// The global factory that creates the AR objects structure
extern ARFactory * gGlobalFactory;

int zaehlerSet;
int accidentals;
int ndots;

/* current segment properties: */
long int sg_num_voices = 0; /* voices counter */
long int sg_num_notes = 0; /* notes counter */
long int sg_num_tags = 0; /* tags counter */

/* rel. dur. of current segment: */
long int sg_enum = 0;
long int sg_denom = 1;


/* current sequence (voice) properties: */
long int sq_num_notes = 0;  /* note counter */
long int sq_num_tags = 0;   /* tag counter */
long int sq_max_voices = 0; /* max chord voices counter */

/* rel. time pos within current voice: */
long int rtp_enum = 0;
long int rtp_denom = 1;

/* current note properties: */
int nt_enumSet=0; /* enum of rel.dur explicitely specified? */

/* rel. dur. of current / last note: */
long int nt_enum=0;
long int nt_denom=1;

/* current chord properties: */
int ch_num_notes=0;	/* number notes in chord */

/* rel. dur. of current chord: */
long int ch_enum=0;
long int ch_denom=1;


/* --- specific initialization code, to be implemented by applications */
void gd_imp_init(void) {}


/* --- specific cleanup code, to be implemented by applications */
void gd_imp_exit(void){}


void gd_noteInit (const char *id) {
vdebug ("gd_noteInit", id);

	accidentals = 0;

   // kf-change!
   // ndots = 0; // This is important to switch dots off!

	zaehlerSet = 0;
	gGlobalFactory->createEvent(id);
}

void gd_noteAcc (int n) {
	accidentals += n;
}

void gd_noteOct (int n) {
vdebug ("gd_noteOct", n);
	gGlobalFactory->setRegister(n);
}

void gd_noteEnum (long int n) {
vdebug ("gd_noteEnum", n);
	 // numerator
	zaehlerSet = 1;
	ndots = 0;
	gGlobalFactory->setNumerator(n);
	gGlobalFactory->setDenominator(1);

	nt_enum=n;
	nt_denom=1;
	nt_enumSet=1;
}

void gd_noteDenom (long int n) {
vdebug ("gd_noteDenom", n);
	if (zaehlerSet == 0)
	 {
	 //  reset to standard
	 gGlobalFactory->setNumerator(1);
	 }
	gGlobalFactory->setDenominator(n);
	ndots = 0; // switch off dots 

	nt_denom=n;
	if(!nt_enumSet)
	 nt_enum=1;
	zaehlerSet = 0;
}

void gd_noteDot(void) {
debug ("gd_noteDot");
	//gGlobalFactory->setPoints(1);
	ndots = 1;
}

void gd_noteDdot(void) {
debug ("gd_noteDdot");
	ndots = 2;
	//gGlobalFactory->setPoints(2);
}

void gd_noteTdot(void) {
debug ("gd_noteDdot");
	ndots = 3;
	//gGlobalFactory->setPoints(2);
}

void gd_seqAppendNote (void) {
debug ("gd_seqAppendNote");
	if (accidentals)
		gGlobalFactory->setAccidentals(accidentals);
	if (ndots)
		gGlobalFactory->setPoints(ndots);
	gGlobalFactory->addEvent();

}

void gd_chordInit(void) {
debug ("gd_chordInit");

	// factory does nothing?
	gGlobalFactory->createChord();

	ch_enum = 0;
	ch_denom = 1;
	ch_num_notes = 0;
}

void gd_chordInitNote(void) {
debug ("gd_chordInitNote");
	gGlobalFactory->initChordNote();
}

void gd_chordAppendNote(void) {
debug ("gd_chordAppendNote");

	// factory does the same thing as with regular Notes.
	// evtl. can be optimised

	if (accidentals)
		gGlobalFactory->setAccidentals(accidentals);
	if (ndots)
		gGlobalFactory->setPoints(ndots);
	gGlobalFactory->addEvent();
}

void gd_seqAppendChord(void) {
debug ("gd_seqAppendChord");
	gGlobalFactory->addChord();
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

void gd_noteAbsDur (long int n) {
vdebug ("gd_noteAbsDur", n);
	int num, denum;
	mls2dur (n, num, denum);
	gd_noteEnum (num);
	gd_noteDenom (denum);
}

void gd_seqInit(void) {
debug ("gd_seqInit");

	gGlobalFactory->createVoice();

	rtp_enum=0;
	rtp_denom=1;

	sq_max_voices=1;
	sq_num_tags=0;

	nt_enum=1;
	nt_denom=4;
	nt_enumSet=0;
	zaehlerSet = 0;
}

void gd_seqExit(void) {
debug ("gd_seqExit");
	gGlobalFactory->addVoice();
}

void gd_segmInit(void) {
debug ("gd_segmInit");

	gGlobalFactory->createMusic();

	sg_enum = 0;
	sg_denom = 1;

	sg_num_voices = 0;
	sg_num_notes = 0;
	sg_num_tags = 0;

	zaehlerSet = 0;
	ndots = 0;
}

void gd_segmExit(void) {
debug ("gd_segmExit");

	// Nothing happens to gGlobalFactory -> You can now
	// retrieve the Music by running factory->getMusic();
}

void gd_segmAppendSeq(void) {
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

void gd_tagStart(const char* id,long int no) {
vdebug ("gd_tagStart", id);
	// skip tag without slash
	if (no<=0)
		no = -1;
	gGlobalFactory->createTag(&(id[1]),no);
}

// will be called at end of complete tag (closing bracket)
void gd_tagEnd(void) {
debug ("gd_tagEnd");
	gGlobalFactory->endTag();
}

void gd_tagIntArg(long int n) {
vdebug ("gd_tagIntArg", n);
	gGlobalFactory->addTagParameter((int) n);
}

void gd_tagFloatArg(T_REAL f) {
vdebug ("gd_tagFloatArg", f);
	gGlobalFactory->addTagParameter(f);
}

void gd_tagArgUnit(char *unit)
{
vdebug ("gd_tagArgUnit", unit);
	// this gets the unit for tags...

	// this is called imediatly after a value
	// with a unit has been read in 
	// (that is after gd_tagIntArg or
	//  gd_tagFloatArg)

	gGlobalFactory->setUnit(unit);

	// Parameter needs not to be freed!
}

void gd_tagStrArg(char *s) {
vdebug ("gd_tagStrArg", s);
	gGlobalFactory->addTagParameter(s);
}

void gd_tagAddArg(const char *s) 
{
vdebug ("gd_tagAddArg", s);
	// is called when a tagargument is complete

	// s is the name (if the parameter had a name)
	gGlobalFactory->setParameterName(s);
	
}

void gd_tagAdd(void) {
	gGlobalFactory->addTag();
	++ sq_num_tags;
}

// will be called after arguments have been passed
// and before range starts. Can be used for checking
// if current tag can have a range or not
void gd_tagRange(void) {
debug ("gd_tagRange");
	gGlobalFactory->tagRange();
}

/** \brief Called after a parse error
*/
int gd_error(const char *msg) 
{
	gParseErrorLine = lnr;
	gParseErrorCol = cnr;
	
	std::cerr << "Guido error: " << msg << " (line " << lnr << ", char " <<  cnr << ")" << std::endl;
	return 0;
}

/* Was: TODO: replace using the guido Feedback class
int gd_error(const char *msg) 
{
#ifndef GMN2GIF
//  printf("\nERROR: %s (line %li, char %li)\n", msg, lnr,cnr);
	char str[250];
	sprintf(str,"Error: %s (line %li, char %li)",
			msg, lnr,cnr);
// ATTENTION: windows specific
#ifdef _WINDOWS

	errmsg = str;

	HMODULE hmod = ::LoadLibrary("nview32.dll");
	if (DialogBox(hmod,MAKEINTRESOURCE(130),NULL,(DLGPROC) MyDlgProc) == 0)
	{
		// abort ... what do we do now?

		::FreeLibrary(hmod);
		return 1;
	}
	::FreeLibrary(hmod);
	return 0; 

#endif
#endif
	return 0;
}
*/
