/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

