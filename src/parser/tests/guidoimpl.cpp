/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "guido.h"

using namespace std;


/* --- specific initialization code, to be implemented by applications */
void gd_imp_init(void) {}

/* --- specific cleanup code, to be implemented by applications */
void gd_imp_exit(void) {}

int gd_error(const char *msg)		{}

void gd_noteInit (const char *id)	{}
void gd_noteAcc (int n)				{}
void gd_noteOct (int n)				{}
void gd_noteEnum (long int n)		{}
void gd_noteDenom (long int n)		{}
void gd_noteDot(void)				{}
void gd_noteDdot(void)				{}
void gd_noteTdot(void)				{}
void gd_noteAbsDur (long int n)		{}
void gd_seqAppendNote (void)		{}

void gd_chordInit(void)				{}
void gd_chordInitNote(void)			{}
void gd_chordAppendNote(void)		{}
void gd_seqAppendChord(void)		{}

void gd_seqInit(void)				{}
void gd_seqExit(void)				{}

void gd_segmInit(void)				{}
void gd_segmExit(void)				{}
void gd_segmAppendSeq(void)			{}

void gd_tagStart(const char* id, long int no)	{}
void gd_tagIntArg(long int n)		{}
void gd_tagFloatArg(T_REAL r)		{}
void gd_tagArgUnit(char* unit)		{}
void gd_tagStrArg(char *s)			{}
void gd_tagAdd(void)				{}
void gd_tagAddArg(const char *s)	{}
void gd_tagEnd(void)				{}

void gd_tagRange()					{}

