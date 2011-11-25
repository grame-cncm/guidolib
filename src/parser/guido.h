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

/* guido.h */

#ifndef GUIDO_H
#define GUIDO_H

#include "GuidoDefs.h"

#ifdef DEBUG_ON
  #define DEBUG(x) x
#else
  #define DEBUG(x)
#endif


/* --- platform / compiler dependent declarations --- */
/* standard settings */

/*#define CODEWARRIOR_SPECIFIC*/
/* uncomment previous line when compiling with metrowerks codewarrior */

//#define STANDARD
/* comment out previous line when using specific options 

#ifdef STANDARD
  #define T_REAL double
  #define const const
#endif

#ifdef CODEWARRIOR_SPECIFIC
  #define T_REAL float
  #define const      
#endif
*/

typedef double T_REAL; 		/* per default, use double precision */


/* --- initialisation, has to be called before any other gd_* function is called --- */
void gd_init(void);

/* --- specific initialization code, to be implemented by applications */
void gd_imp_init(void);

/* --- cleanup, should be called when parser is not used any longer --- */
void gd_exit(void);

/* --- specific cleanup code, to be implemented by applications */
void gd_imp_exit(void);

const char * gd_pc2noteName(int pitch);

#define PARSE(f)	parse(f)

//#define CINC { if (yyleng > 1) cnr+=yyleng; else cnr++; } /* gl_sc_charcount+= yyleng; */
#define CINC { if (guidoleng > 1) cnr+=guidoleng; else cnr++; } /* gl_sc_charcount+= yyleng; */
#define LINC { lnr++; cnr=0; } /* gl_sc_linecount++;gl_sc_rellinecount++; */

#define ERROR(s)	gd_error(s);

/* gd-na parser modes, determine which tags/events are filtered out */
#define PARSE_MODE_ALL        0
#define PARSE_MODE_PHASE_1_1  11
#define PARSE_MODE_PHASE_1_3  13
#define PARSE_MODE_PHASE_2    2

extern int parse_mode;

extern long int lnr;
extern long int cnr;
extern long int cmnt_level;

int gd_error(const char *msg);
int gd_parse(const char *filename, int mode);
int gd_parse_buffer(const char *buffer);

/* Old:
#define NOTE_C 0
#define NOTE_CIS 1
#define NOTE_D 2
#define NOTE_DIS 3
#define NOTE_E 4
#define NOTE_F 5
#define NOTE_FIS 6
#define NOTE_G 7
#define NOTE_GIS 8
#define NOTE_A 9
#define NOTE_AIS 10
#define NOTE_H 11
#define REST -1
#define EMPTY -2
*/

#define SHARP +1
#define FLAT (-1)

#define GD_INIT_SEGM	gd_segmInit();
#define GD_EXIT_SEGM	gd_segmExit();
#define GD_APP_SEQ   	gd_segmAppendSeq();

#define GD_INIT_SEQ  	gd_seqInit();
#define GD_EXIT_SEQ    	gd_seqExit();

#define GD_NT(x)    	gd_noteInit(x);
#define GD_SH_NT    	gd_noteAcc(SHARP);
#define GD_FL_NT    	gd_noteAcc(FLAT);
#define GD_OCT_NT(x)    gd_noteOct(x);
#define GD_ENUM_NT(x) 	gd_noteEnum(x);
#define GD_DENOM_NT(x) 	gd_noteDenom(x);
#define GD_DOT_NT      	gd_noteDot();
#define GD_DDOT_NT     	gd_noteDdot();
#define GD_TDOT_NT     	gd_noteTdot();
#define GD_ABSDUR_NT(x) gd_noteAbsDur(x);

#define GD_APP_NT    	gd_seqAppendNote();

#define GD_INIT_CH   	gd_chordInit();
#define GD_CH_INIT_NT	gd_chordInitNote();
#define GD_CH_APP_NT 	gd_chordAppendNote();
#define GD_APP_CH    	gd_seqAppendChord();

#define GD_TAG_START(x,i) gd_tagStart(x,i);
#define GD_TAG_END      gd_tagEnd();
#define GD_TAG_NARG(x)  gd_tagIntArg(x);
#define GD_TAG_RARG(x)  gd_tagFloatArg(x);
#define GD_TAG_SARG(x)  gd_tagStrArg(x);
#define GD_TAG_TARG(x)  /* don't use this! */
#define GD_TAG_ADD_ARG(x) gd_tagAddArg(x);
#define GD_TAG_ADD      gd_tagAdd();
#define GD_TAG_ARG_UNIT(x) gd_tagArgUnit(x);
#define GD_TAG_RANGE    gd_tagRange();


void gd_fracNorm(long int *a, long int *b);
  /* normalises fraction a/b */
void gd_fracAdd(long int *a, long int *b, const long int c, const long int d);
  /* sets a/b := a/b+c/d, normalised */
  /* normalise a/b, c/d before using this function */
int gd_fracCmp(long int a, long int b, long int c, long int d);
  /* returns +1 if a/b > c/d, 0 if a/b=c/d, -1 else */
  /* normalise a/b, c/d before using this function */

long int gd_tagStackPop(void);
void gd_tagStackClear(void);
void gd_tagStackPush(long int tagno);

extern long int tagno;

int gd_noteName2pc (const char *name);

void gd_noteInit (const char *id);
void gd_noteAcc (int n);
void gd_noteOct (int n);
void gd_noteEnum (long int n);
void gd_noteDenom (long int n);
void gd_noteDot(void);
void gd_noteDdot(void);
void gd_noteTdot(void);
void gd_noteAbsDur (long int n);
void gd_seqAppendNote (void);

void gd_chordInit(void);
void gd_chordInitNote(void);
void gd_chordAppendNote(void);
void gd_seqAppendChord(void);

void gd_seqInit(void);
void gd_seqExit(void);

void gd_segmInit(void);
void gd_segmExit(void);
void gd_segmAppendSeq(void);

void gd_tagStart(const char* id, long int no);
void gd_tagIntArg(long int n);
void gd_tagFloatArg(T_REAL r);
void gd_tagArgUnit(char* unit);
void gd_tagStrArg(char *s);
void gd_tagAdd(void);
void gd_tagAddArg(const char *s);
void gd_tagEnd(void);

/* (JB) moved to Gmn2MidiParser.h
char gd_getTagArgType(int n);
char* gd_getTagArgName(int n);
long int gd_getTagArgInt (int n);
T_REAL gd_getTagArgFloat (int n);
char* gd_getTagArgUnit (int n);
char* gd_getTagArgStr (int n);
*/
void gd_tagRange();


/* rel. time pos in current seq (voice) = rtp_en/rtp_denom */
extern long int rtp_en;
extern long int rtp_dn;

/* turns automatic increase of rel. time pos after notes on, off default is ON */
extern void gd_durIncrOn(void);
extern void gd_durIncrOff(void);

#endif

