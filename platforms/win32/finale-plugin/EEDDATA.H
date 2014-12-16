/*
File Name:	EEDDATA.H

Copyright 1997 Coda Music Technology (all rights reserved)

Author:  	Tim Wright
Created:	05/20/97

Purpose:
	Defines tags and structures for Enigma Entry Detail Data.

Modification History:

**  05/20/97    TNW       Moved entry detail stuff to here.
**  05/21/97    RES       added LF at end, _EENTDDATA_H_ --> _EEDDATA_H_
**  05/28/97    TNW       added EDTChord.
**  06/03/97    TNW       ot_StaffExpression -> ed_StaffExpression
**  06/23/97    TNW       note that raw text is 1-based
**  08/07/97    TNW       note that all incidents are 0-based
**  09/10/97    TNW       added EDTNoteheadMods.
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  12/31/97     EEDDATA.H      RES       dt_CN --> dt_CUEN
**  12/31/97     EEDDATA.H      RES       dt_IM --> dt_IMRK
**  12/31/97     EEDDATA.H      RES       dt_ED --> dt_EDYNAMIC
**  12/31/97     EEDDATA.H      RES       dt_CD --> dt_CROSSSTAF
**  12/31/97     EEDDATA.H      RES       dt_se --> dt_LYRDATA_Section
**  12/31/97     EEDDATA.H      RES       dt_ch --> dt_LYRDATA_Chorus
**  12/31/97     EEDDATA.H      RES       dt_ve --> dt_LYRDATA_Verse
**  01/03/98     EEDDATA.H      RES       dt_CH --> dt_CHRD
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  01/12/98     EEDDATA.H      RES       dt_TP --> dt_TUPL
**  03/14/98     EEDDATA.H      RES       added XEF_ to extended entry flags
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  10/07/98     EEDDATA.H      RES       prefixed note flags with NF_, THR
**						noteflags with THR_
**  10/07/98     EEDDATA.H      RES       corrected comments
**  10/07/98     EEDDATA.H      RES       prefixed note flags with NF_, THR
**						noteflags with THR_, entry flags with EF_
**  10/07/98     EEDDATA.H      RES       EF_TEXTDTAIL --> EF_LYRICDETAIL
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: EEDDATA.H,v $
Revision 1.1.2.1  2003/01/13 09:05:02  jkilian
added lyric support to finale plug-in

** 
** 5     8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 4     7/20/99 4:33p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 3     5/12/99 11:56 AM Jlenarz
** Changed EDTChord to match changes to CHRD
 * 
 * 2     1/28/99 2:41p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _EEDDATA_H_
#define _EEDDATA_H_

#ifndef _EDATA_H_
#include "edata.h"
#endif

/*##############################################################################*/

/* All entry detail incidents are 0-based (like all incidents), entry numbers
   are 1-based */

/*##############################################################################*/

/* EDTPerformanceData
**
** This is the performance, midi information is usually modified/read through
** the Midi Tool.
**
** Entry with the performance data must have its ef flagged with EF_PRFMDATA
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte noteID;         /* performance data belongs to this note in the entry */
	twobyte stadd;			/* EDUs to add to the 'visual' start time */
	twobyte enadd;			/* EDUs to add to the 'visual' end time */
	twobyte keyadd;         /* add this to the 'visual' midi key number */
	twobyte velocity;       /* midi velocity delta from 64 */

} EDTPerformanceData; /* ~ PERFDATA, dt_PERFDATA, DTAILDEC.H */

/*-----------------------------------------------------------------*/

#define ed_PerformanceData		MAKEEXTAG(edEntryDetail,'a','c')
/* one incident per note w/perf data, match noteID in struct w/note ID in entry)
   (inci is 0-based) */


/*##############################################################################*/

/* Cross staffing
**
** Entries with this entry data cross the specified noteID
** to instrument.
**
** Note to be cross-staved should have its flag flagged with
** NF_CROSSBIT, entry should have ef flagged w/ EF_CROSUPBIT
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte noteID;
	twobyte instrument;
	twobyte AAAA;			// unused
	twobyte BBBB;			// unused
	twobyte CCCC;			// unused

} EDTCrossStaff; /* ~ CROSSSTAF, dt_CROSSSTAF, DTAILDEC.H */

/*-----------------------------------------------------------------*/

#define ed_CrossStaff		MAKEEXTAG(edEntryDetail,'C','D')
/* one incident per note that needs to be cross-staffed, match noteID in struct w/note ID in entry)
   (inci is 0-based)*/

/*##############################################################################*/

/* EDTStaffExpression
**
** This is the base record of staff expressions.
** Info is normally set in the staff expression assignment dialog:
**
** Entry with the staff expression must have its xef flagged with
** XEF_STAFFEXPRDETAIL;
*/

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte dynumber; /* cmper for shape (ot_ShapeExpression) or text (ot_TextExpression) expression */
	twobyte posadd;   /* horiz EVPU pos from entry */
	twobyte lineadd;  /* vertical EVPU pos from entry */
	twobyte current;  /* internal use only, set to 0 */
	FLAG_16 flag;

/*---------- flag bits: --------*/
#define STAFFEXP_ETEXT_OBJECT	0x8000 /* is text expression (not shape) */
#define STAFFEXP_NOTEBELOW		0x0800 /* goes below note (not above) */

#define STAFFEXP_EVECTORSTART	0x0100 /* start playback effect at position in measure (not beginning) */

#define STAFFEXP_CHANNELSWITCH			0x00F0
#define	STAFFEXP_NO_CHANNELSWITCH		0
#define STAFFEXP_CHANNELSWITCHtoL1		0x0010
#define STAFFEXP_CHANNELSWITCHtoL2		0x0020
#define STAFFEXP_CHANNELSWITCHtoL3		0x0030
#define STAFFEXP_CHANNELSWITCHtoL4		0x0040
#define STAFFEXP_CHANNELSWITCHtoCHORD	0x00E0
#define STAFFEXP_CHANNELSWITCHtoDYN 		0x00F0
#define	STAFFEXP_EDY_ORDER_BITS			0x0003	/* when the staff expression
										           is drawn relative to the entry: */
#define		STAFFEXP_EDY_DONT_CARE			0		/* draw when convenient (fastest) */
#define 	STAFFEXP_EDY_BEFORE_ENTRY		0x0001	/* draw expression before entry */
#define 	STAFFEXP_EDY_AFTER_ENTRY		0x0002	/* draw expression after entry */
/*------------------------------*/

} EDTStaffExpression; /* ~ EDYNAMIC, dt_EDYNAMIC, DTAILDEC.H */

/*-----------------------------------------------------------------*/

#define ed_StaffExpression		MAKEEXTAG(edEntryDetail,'E','D')

/* inci = which staff expression for this note (0-based)*/

/*##############################################################################*/

/* EDTLyric
**
** Lyrics are stored as entry details which give a syllable offset
** into a raw text record that represents a whole verse/chorus or
** section.  (See raw text documentation in  ETFSPEC.TXT)
**
** To load raw text for the lyric, use rawTextNum as the rawTextNumber, and either
** tx_VerseLyric/tx_ChorusLyric/ or tx_ChorusLyric depending
** on the tag that the EDTLyric is stored under.
**
** Entry's ef flag must have EF_LYRICDETAIL set if it has any lyrics.
*/

typedef struct
{
	twobyte rawTextNum;   /* which chorus/verse/section (1-based) */
	twobyte syll;		/* syllable of chorus/verse/section to attached to entry */
	twobyte posadd;		/* additional horiz evpu offset of lyric */
	twobyte lineadd;	/* additional vertical evpu offset of lyric */
	twobyte wext;		/* length of word extension in EVPUs */

} EDTLyric;   /* ~ LYRDATA dt_LYRDATA_Verse dt_LYRDATA_Chorus dt_LYRDATA_Section, DTAILDEC.H */

/*-----------------------------------------------------------*/

#define ed_VerseLyric		MAKEEXTAG(edEntryDetail,'v','e')
#define ed_ChorusLyric		MAKEEXTAG(edEntryDetail,'c','h')
#define ed_SectionLyric		MAKEEXTAG(edEntryDetail,'s','e')

/* inci = incident of each lyric type (verse/chorus/section,
   for each verse/section/chorus with lyric on this entry (0-based)*/

/*##############################################################################*/
/* EDTTuplet - info for tuplet, EDTTuplet is attached to the
   first entry in the tuplet (it is entry detail data)
   Entry needs EF_TUPLSTART flagged in ef if it has a tuplet.

   You may wish to load document globals to get the default
   tuplet parameters before creating a tuplet. ("Default Tuplet Visual Definition")

   These values are normally set in the Tuplet Definition dialog.
*/

typedef struct
{
	twobyte tupnum;			/* The number of members in the tuplet	*/
	twobyte tupsymdur;		/* The symbolic duration				*/
	twobyte refnum;			/* The number of reference notes		*/
	twobyte refdur;			/* The reference duration				*/
									/* example: eigth-note triplet is: 3,512,2,512 */
	twobyte endent;			//end entry, normally set to 0 (recomputes end entry)
	twobyte tupOffX;			// "Number H:"
	twobyte tupOffY;			// "Number V:"
	twobyte brackOffX;		// "Shape H:"
	twobyte brackOffY;		// "Shape V:"
	FLAG_16 flag;
/* bits for flag: ~ TUPNUM_STYLE..TUP_AUTOBRACK, DTAILDEC.H */
#define	TUPLET_NUMSTYLE	0x000F		//"Number" popup (16 styles reserved)
#define		TUPLET_NUMNONE			0x0000 //Nothing
#define		TUPLET_NUMNUMBER		0x0001 //Number
#define		TUPLET_NUMUSERATIO	0x0002 //Ratio

#define	TUPLET_POSSTYLE			0x0070	//"Placement" popup (8 styles reserved)
#define		TUPLET_POSMANUAL		0x0000	//Manual
#define		TUPLET_POSBEAMSIDE	0x0010	//Beam side
#define		TUPLET_POSNOTESIDE	0x0020
#define		TUPLET_POSABOVE		0x0030
#define		TUPLET_POSBELOW		0x0040   //

#define TUPLET_ALLOW_HORZ			0x0080	//"Allow Horizontal Drag"

#define TUPLET_IGNOREGLOFFS 		0x0100	//"Ignore Format Offset"
#define TUPLET_BREAKBRACK			0x0200   //"Break Slur or Bracket"
#define TUPLET_MATCH_HOOKS			0x0400   //"Match Length of Hooks"

#define	TUPLET_BRACKSTYLE			0x3000 //"Shape" popup 4 styles reserved
#define		TUPLET_BRACKNONE		0x0000 //Nothing
#define		TUPLET_BRACKSLUR		0x1000 //Slur
#define		TUPLET_BRACKBRACK		0x2000 //Bracket

#define TUPLET_AUTOBRACK	0x8000		//"Auto Bracket" check

	twobyte leftHookLen;		/* "Left Hook" */
	twobyte leftHookExt;		/* "Left Extension" */
	twobyte rightHookLen;	/* "Right Hook" */
	twobyte rightHookExt;	/* "Right Extension" */
	twobyte slope;				/* "Slope" */

}  EDTTuplet; /* ~ TUPL, TUPN, TUPLOFFSET, dt_TUPL, DTAILDEC.H */

#define ed_Tuplet MAKEEXTAG(edEntryDetail3Inci,'T','P')
/* inci = which tuplet on this entry (0-based, entries w/ nested tuplets have multi incidences) */


/*##############################################################################*/
/* EDTArticulation - info for articulation (entry detail data)
   This gives individual positioning info for the articulation on
   a single entry and points to a more detailed EDTArticulationDefinition
   other.

   Entry needs EF_IMRKDTAIL set in ef if it has any articulations.
*/

typedef struct
{
	twobyte imrkdef;		/* cmper of the EDTArticulationDefinition record */
	twobyte posadd;		/* Added to horizontal position	*/
	twobyte posadder;		/* Added to horz position (cloned marks only) */
	twobyte noteadd;		/* Added to vertical position. For a
									cloned mark, this is added to the
									position of the highest note to find
									the starting point. */
	twobyte lonoadd;			/* (cloned marks only) Added to the lowest
									note to find the ending point of the
									cloning operation. */
} EDTArticulation;  /* ~ IMRK, dt_IMRK, DTAILDEC.H */

#define ed_Articulation MAKEEXTAG(edEntryDetail,'I','M')
/* inci = which articulation on this entry (0-based)*/

/*##############################################################################*/
/* EDTChord - info for chord (entry detail data)
   This info is normally edited in the "Chord Definition" dialog box.

   Learned chords use the same format, but are stored under


   Entry needs EF_CHORDBIT set in ef if it has any chords.
*/

typedef struct
{
	// Physical Incidence 1
	twobyte reg;	//scale degree of root in bottom 5 bits, flags.
//bits for reg: (~PLAYCHORD..CHRD_LOWERCASE, CHRDSPEC.H)
#define CHORD_PLAYCHORD				0x0080	//"Play Chord Suffix"
#define CHORD_SHOWROOT 				0x0040	//"Root Scale Tone" check (root is drawn if set)
#define CHORD_PLAYROOT				0x0020	//"Play Root"
#define CHORD_HIDE_FRETBOARD		0x4000	//"Hide Fretboard"
#define CHORD_SHOW_SUFFIX			0x2000	//"Chord Suffix ID" check (suffix is drawn if set)
#define CHORD_RLBITS					0x0F00	//"Alteration" (signed int)
#define CHORD_SCALENUM				0x001F	//"Root scale tone" - 1
#define CHORD_LOWERCASE				0x8000	//"Lowercase" (draw root or altbass using lowercase)

	twobyte altbass;			//scale degree of alt. bass in bottom 5 bits, flags.
//bits for altbass:
#define CHORD_DOWNSIDEBIT			0x0080	//"Put Under Root" (put altbass under suffix)
#define CHORD_SHOWALTBASS			0x0040	//"Alternate Bass" check (altbass is drawn if set)
#define CHORD_PLAYALTBASS			0x0020	//"Play Alternate Bass"
//CHORD_RLBITS, CHORD_SCALENUM, CHORD_LOWERCASE are also valid here.

	twobyte suffix;		//cmper for incidences of EDTChordSuffix records (others)
	twobyte posadd;		//horizontal offset of chord symbol from entry
	twobyte lineadd;		//vertical offset of chord symbol from entry.
	
	// Physical Incidence 2
	
	twobyte fbposadd;		// x offset of fretboard (EVPUs)
	twobyte fblineadd;		// y offset of fretboard (EVPUs)
	twobyte AAAA;			// pad
	twobyte BBBB;			// pad
	twobyte CCCC;			// pad

} EDTChord;  /* ~ CHRD, dt_CHRD, CHRDSPEC.H */

#define ed_Chord MAKEEXTAG(edEntryDetail2Inci,'C','h')

/* inci = which chord on this entry (0-based)*/

// Learned chords are EDTChord records stored under the tag dt_LearnedChord,
// with the root and alternate bass as cmpers and with a hashvalue
// stored in the posadd field.

// This hashvalue encodes the distance of each note in the chord from
// the root. Each bit represents the number of half steps from the
// root. For example, a major triad has a note 4 half steps above
// the root (the third) and another 7 half steps above the root
// (the fifth). It would be encoded as 01001000	in binary, and stored in posadd

#define dt_LearnedChord MAKEEXTAG(edDetail,'h','C')
/* cmper1==root (1-based), cmper2=alt. bass (1-based),
   inci=which learned chord w this root/alt. bass (0-based)*/

/*##############################################################################*/

/* EDTNoteheadMods
**
** Holds various note-specific custom attributes (normally
**	edited in Special Tools.
**
** If an entry has EDTNoteheadMods, its ef field should be flagged
** w/ EF_NOTEDTAIL
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte noteID;	//note that these mods belong to.
	twobyte percent;	//size percentage of notehead (100 is normal size)
	twobyte nxdisp;	//additional horizontal offset of notehead. (0 is normal)
	twobyte axdisp;	//additional horizontal offset of accidental on notehead. (0 is normal)
	twobyte altnhead;	//if not zero, slot for alternate note head (use ' ' to hide)

} EDTNoteheadMods; /* ~ CUEN, dt_CUEN, DTAILDEC.H */

/*-----------------------------------------------------------------*/

#define ed_NoteheadMods		MAKEEXTAG(edEntryDetail,'C','N')
/* one incident per note w/mods, match noteID in struct w/note ID in entry
   (inci is 0-based) */

/*##############################################################################*/

#endif
