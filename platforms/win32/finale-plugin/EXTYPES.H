/*
File Name:	EXTYPES.H

Copyright 1997 Coda Music Technology (all rights reserved)

Author:  Tim Wright
Created:	03/05/97

Purpose:
	Structures, types for extensions:

Modification History:

**  03/20/97  TNW  Added REBAR_STOP_* defines for stopFlags param in FX_RebarRegion
**  03/20/97  TNW  Added NOTEMOVER_* defines for action param in FX_NoteMover
**  04/24/97  TNW  took out NOTEMOVER_CROSSSTAFF, (can't do it currently through FX_NoteMover)
**  04/25/97  TNW  restored NOTEMOVER_CROSSSTAFF, added DIALOG_*
**  06/12/97  TNW  added DIALOG_MOVABLE_MODAL
**  06/18/97  TNW  added EDTEnigmaFont, EColor, EVPUFixed, EVPUFixedShort, ETextStyle
**				moved FONT_EFX_* to here, added MIDIPITCH_*
**  06/20/97  TNW  added needSelectedRegion, useGetMenuStatus to FXT_MENUSTRING_PACKET.
**  06/20/97  TNW  removed windef, quickdraw.h
**  07/03/97  TNW  EDTEnigmaFont -> EEnigmaFont, added EMeasureMetrics, EEntryMetrics
**  07/09/97  TNW  removed posi from EXENTRY (this field was always 0, so it wasn't much use
**				if you really need the position of the entry, use FX_GetEntryMetrics.
**  07/10/97  TNW  added SELECT_* (for FX_SelectionDialog)
**  08/15/97  TNW  documented EEntryMetrics a little more.
**  08/20/97  TNW  increased FXT_VERSION to 0,2
**  08/28/97  TNW  added more comments to EMeasureMetrics, EEntryMetrics.
**  09/11/97  TNW  added EProgressDataP, DLGITEM_*
**  09/11/97  TNW  EProgressDataP is a EProgressData pointer.
**  09/23/97  TNW  FXT_VERSION changed to  0x0401, 0x0380
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**	10/14/97  EVS  Added DLGITEM_3STATE_CHECK to the types of controls used for
**              FX_Get/SetDlgCntl
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**	12/15/97  TNW  moved FXT_VERSION to fxvers.h, added FXT_PARAMS, additional
**				params to FXT_MENUSTRING_PACKET
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: EXTYPES.H,v $
Revision 1.1.1.1  2002/12/19 19:13:11  dfober
first import

** 
** 9     8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 8     8/13/99 12:32p Jlenarz
** Changed extension dialog handler return value to FX_RETURNVAL
** 
** 7     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 6     6/03/99 2:59p Rstokes
** added egraf.h
** 
** 5     5/25/99 3:45 PM Jlenarz
** Moved clef into the "DO NOT EDIT" fields.
** 
** 4     5/25/99 2:03 PM Jlenarz
** Added EXMEASPOINT and EXMEASRANGE for new EDTStaffSpec field.
** 
** 3     5/25/99 8:41 AM Jlenarz
** Added 'clef' field (from TENTRY) to EXENTRY.
 * 
 * 2     1/28/99 2:43p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

/*--------------------------------------------------------------------------------*/

#ifndef _EXTYPES_H_
#define _EXTYPES_H_

#ifndef _VERSION_H_
#include "version.h"
#endif

#ifndef _NOTEFLAG_H_
#include "noteflag.h"
#endif

#ifndef _FXVERS_H_
#include "fxvers.h"
#endif

#ifndef _EGRAF_H_
#include "egraf.h"
#endif


/*--------------------------------------------------------------------------------*/

//Extension-wide info:
typedef struct
{
	ELONG version;			//version of PDK that this plugin is compatible with (use FXT_VERSION for current version)
	ELONG numSubExtensions; //number of sub-extensions (or menu items)

	tbool initOnLoad; 		//TNW 12/11/97 if 1, run init as soon as plugin is loaded
							//(only relevant when plugin is in plugins folder)  Normally
							//set this to 0, to speed launch time and save memory.
							//set to one if there is something the plugin must do at
							//program start.
	twobyte reserved;		//reserved for future expansion, and to pad struct to even # of longs.
} FXT_PARAMS;

/*--------------------------------------------------------------------------------*/

//Sub-extension (or menu item) specific info:

#define MAX_MENUSTR_LEN		64
#define MAX_MSGSTR_LEN		64
#define MAX_UNDOSTR_LEN		64

typedef struct
{
	ECHAR menuItem[ MAX_MENUSTR_LEN ];
	ECHAR msgString[ MAX_MSGSTR_LEN ];
	ECHAR undoString[ MAX_UNDOSTR_LEN ];
	tbool needSelectedRegion; //TNW 6/20/97 if 1, disable menu if no massmover selection.
	tbool useGetMenuStatus; //TNW 6/20/97 if 1, call extension's GetMenuStatus to determine status

	tbool launchable; 		//TNW 12/11/97 if 1, can be launched as a document
							//(will init and run invoke) Normally set to 1 (true)
							//so that plugin doesn't have to be in the plugins folder
							//to be applied.  initOnLoad and launchable are for
							//normal purposes, mutually exclusive.

	tbool canTerminateAfterInvoke; //TNW 12/11/97 for launchable plugins:
								  //if 1, Finale may  terminate extension after
								  //invoke (no modeless dialogs or behavior)
								  //to clean up a plugin launch.

	tbool canTerminateAfterLastDialog; //TNW 12/11/97 if 1, Finale may terminate extension after
									   //last dialog is dismissed (for modeless dialogs)
									   //This attribute is for plugins that can't be
									   //terminated after invoke because there is some modeless
									   //behavior.
} FXT_MENUSTRING_PACKET;

/*--------------------------------------------------------------------------------*/

#define TGFNN		12	/* no. notes in an entry in tgf */
#define	MAXLAYERS	4

/*--------------------------------------------------------------------------------*/

typedef struct
{
	twobyte tcd;
	twobyte alteration;
	twobyte noteID;
	FLAG_32 flag;
} EXNOTE;

/*--------------------------------------------------------------------------------*/

typedef struct
{
	twobyte channel;
	uonebyte status;	/* status byte */
	uonebyte data[2];	/* data bytes */
} EMIDIEVENT;

/*--------------------------------------------------------------------------------*/

typedef ECHAR EFONTNAME[256];

/*--------------------------------------------------------------------------------*/

typedef struct
{
	ENTNUM entnum;			/* entry number (do not change) */
	twobyte dura;			/* symbolic duration value */
	twobyte orgposi;		/* this is the position stored in the 'posi'
								element of the raw entry. It normally corresponds
								to 'manual positioning'. */
	FLAG_32 ef;				/* entry flag */
	FLAG_16 xef;			/* extended entry flag */
	twobyte numNotes;		/* number of notes in note[] */
	EXNOTE note[TGFNN];		/* array of notes */


	/* The following values are computed for convenience, but are *not* saved in SaveTGF():
		DO NOT EDIT! */

	TIME_EDU elldur;		/* elapsed duration. Time in EDUs from the
								start of the measure up to (but not
								including) this entry. (RES) */
	TIME_EDU actdur;		/* actual duration of the entry, as opposed to
								symbolic duration. Usually this is the same
								as dura. In tuplets, however, they differ.
								Ex: an eighth note has a symbolic duration
								of 512. An eighth note in a triplet, however,
								has an actual duration of 341. (RES) */
	
	twobyte clef;			// Index of clef for this entry

#if OPERATING_SYSTEM == WINDOWS
#if !defined( WIN32 )
	onebyte padding[110];	/* pad TENTRY to 256 bytes so TGFs can grow > 128K */
#endif
#endif
} EXENTRY;					/* size is currently 146 bytes for Mac, Win32; 256 bytes for Win16 */

/*--------------------------------------------------------------------------------*/

typedef struct
{
	CMPER iuList;
	twobyte staffSlot;
	twobyte inst;
	twobyte meas;
	twobyte layer;
	tbool isMirror;
	twobyte frame;

	EXENTRY *pent;
	twobyte numEntries;
} EXTGF;

/*--------------------------------------------------------------------------------*/

typedef struct
{
	twobyte beats;			/* # of beats in measure */
	twobyte divbeat;		/* # of EDUs per beat */
	FLAG_16 timeflag;		/* any combination of ALTDENTSIG, ALTNUMTSIG, and ABBRVTIME */
} ETimeSig;

/*--------------------------------------------------------------------------------*/

typedef struct
{
	twobyte font; //Enigma Font ID (FX_FontNumToName) */
	twobyte size; //Font size in points
	FLAG_16 efx;  //Font efx (FONT_EFX_*)
} EEnigmaFont;
/*--------------------------------------------------------------------------------*/

/* 26.6 fixed EVPU value (or EVPUS * 64) : */
typedef fourbyte EVPUFixed; /* ~EFIX, EFIX.H */

/* 10.6 fixed EVPU value (or EVPUS * 64) : */
typedef twobyte EVPUFixedShort; /* ~ESFIX, EFIX.H */


/*--------------------------------------------------------------------------------*/

MACCODE( typedef RGBColor EColor; )
WINCODE( typedef COLORREF EColor; )

/*--------------------------------------------------------------------------------*/

typedef struct
{
	EEnigmaFont	ef;
	EColor		color;
	fourbyte	tracking;		// inter-letter spacing in "EMs"
	EVPU		baselineShift;	// offsets text from baseline, but does not affect layout
	EVPU		superscript;	// offsets text from baseline and may affect layout
} ETextStyle;

/*--------------------------------------------------------------------------------*/

/* metrics for a staff/measure intersection (frame-intersection-point or FIP */

typedef struct
{
	twobyte ssys;			/* staff system */
	CMPER iu;				/* IU list number */
	twobyte slot;			/* position in IU list ("slot") */
	fourbyte horzPercent;	// horizontal scaling percentage * 100 (was meperc)
							/* things like word extensions, 'stretchable' shapes
							   are scaled by this amount (in addition to any
							   lperc scaling) */
	twobyte mewidth;		/* width of the measure (EVPUs) including the front 	//SEVPU
							and back separations (uMUSfSEP, uMUSbSEP) (scale by percent/100 to
							get absolute EVPUS) */
	twobyte scalewidth;		/* width of the measure (EVPUs) (mewidth) minus		//SEVPU
							the front and back separations (uMUSfSEP, uMUSbSEP) */
	fourbyte percent;		/* current accumulated staff system size (%)
							(percent * 100) */
	fourbyte lperc;			/* accumulated line enduction % (current staff size:
							if this value is different from 'percent', there is a
							line enduction on this staff)(percent * 100)
							Things that scale w/the measure (lyrics, notes,
							expressions, etc. are scaled by this amount) */

	//These are all in the staff system 'perspective'
	//so scale these by percent/100 to get absolute evpus.
	EVPU ssysTop;			// EVPU top of staff system
	EVPU fipTop;			// EVPU top of FIP
	EVPU fipBottom;			// EVPU bottom of FIP
	EVPU fipTopStaffLine;	// CFC 08/01/95: EVPU top line of staff
							// Eventually rename fipTop -> fipReferenceLine
							// and fipBottom -> fipBottomStaffLine
	EVPU meposadj;			/* beginning of music portion of measure
							(after key/time and repeat bars) */
	twobyte frontRepWidth;	/* front repeat width */	//SEVPU
	twobyte backRepWidth;	/* back repeat width */		//SEVPU
	twobyte barWidth;		// 11/14/96 RES: added (required in seq_music())	//SEVPU
							// only referenced if PUSHEND is set in flag

	//Note: To get fipLeft, fipRight, use meposadj and meposadj + mewidth

	/* These are in Quickdraw or GDI coordinates
	   and are measured from the top and left of the page

	   These are native unit equivalents of fipTop, meposadj,
	   fipBottom, meposadj+mewidth, and fipTopStaffLine.

	   (Not really useful to extensions) */

	// The following top variable is also a reference line top.

	fourbyte top;			/* "top of page native" units */
	fourbyte left;
	fourbyte bottom;
	fourbyte right;
	fourbyte topStaffLine;	// CFC 08/01/95, to go with fipTopStaffLine above.
							// Eventually rename top -> referenceLine
							// and bottom -> bottomStaffLine
} EMeasureMetrics;


// -----------------------------------------------------

typedef struct
{
	fourbyte	hval;		//vertical position in EVPUS (from page bottom)
	fourbyte	noteStart;	//horizontal position in EVPUS (from page left)
	SEVPU		noteWidth;	// noteEnd = noteStart + noteWidth
	twobyte		noteID;		// from EXNOTE.
} ENoteMetrics;

// -----------------------------------------------------

typedef struct
{
	twobyte		layer;			//entry's layer
	TIME_EDU	edu;			//entry's elldur
	twobyte		symDur;			//entry's dura
	twobyte		numNotes;		//number of notes in entry.
	FLAG_32		entryFlags;		//entry's ef flags.
	ERECT		headRect;		//bounding box of all noteheads */
	ERECT		stemRect;		//bounding box of stem
	twobyte		secondsDivider; //offset from left edge of headRect to the
								//divider between main and seconds columns
	EVPU		lastDot;		//horizontal position of rightmost augmentation dot.
	EVPU		firstAcci;		//the left most edge of the accidental rectangle
								// for all accidentals on the entry.
	ENoteMetrics	noteReports[ TGFNN ];
} EEntryMetrics; /* ~ENTREPORT, IMTRKPRO.H */

// -----------------------------------------------------
// JPL 05/25/99 EXMEASPOINT and EXMEASRANGE are used in EDTStaffSpec

typedef struct
{
	twobyte meas;
	TIME_EDU edu;
} EXMEASPOINT; // MEASPOINT

// -----------------------------------------------------

typedef struct
{
	EXMEASPOINT start;
	EXMEASPOINT end;
} EXMEASRANGE; // MEASRANGE


/* For headRect, stemRect: top,bottom are distance from page bottom,
   left,right are distance from page right */
/*--------------------------------------------------------------------------------*/

//Dummy struct don't rely on length/field of this
//its just here to get a typed pointer to private
//info in Finale.

struct EProgressData
{
	long priv;  //Private to Finale
};

typedef EProgressData* EProgressDataP;

/*--------------------------------------------------------------------------------*/

/* Font effects: */

/* ~ ENIGMA_EFX_BOLD..ENIGMA_EFX_ABSOLUTE, EFONTDEC.H */

#define FONT_EFX_PLAIN			0x00
#define FONT_EFX_BOLD			0x01
#define FONT_EFX_ITALIC			0x02
#define FONT_EFX_UNDERLINE		0x04
#define FONT_EFX_OUTLINE		0x08
#define FONT_EFX_SHADOW			0x10
#define FONT_EFX_STRIKEOUT		0x20
#define FONT_EFX_ABSOLUTE		0x40

/*--------------------------------------------------------------------------------*/
/* value for EDTGeneralPrefs.PitchRepresentationValue or midiPitchRepType in FX_MidiNoteToString */

#define MIDIPITCH_NUMERIC 0
#define MIDIPITCH_STRING 1

/*--------------------------------------------------------------------------------*/

/* codes for stopFlags in FX_RebarRegion ~REBAR_NO_STOP..REBAR_STOP_EMPTYFRAME,MASSDECS.H*/

#define	REBAR_NO_STOP               0x0000 /* stop at end of piece */
#define	REBAR_STOP_KEYCHANGE		0x0001 /* stop at a key change */
#define	REBAR_STOP_TIMECHANGE		0x0002 /* stop at time signature at a key change */
#define	REBAR_STOP_ENDOFREGION		0x0004 /* stop at end of region */
#define	REBAR_STOP_SPECIALBARLINE	0x0008 /* stop at non-normal bar line */
#define	REBAR_STOP_EMPTYFRAME		0x0020 /* stop at empty frame */

/*--------------------------------------------------------------------------------*/

/* codes for action in FX_NoteMover ~EM_OP_COPREPL..EM_OP_DELETE,FINEMOVE.H*/

#define NOTEMOVER_COPREPL		1		/* copy and replace notes */
#define NOTEMOVER_COPMERGE		2		/* copy and merge notes */
#define NOTEMOVER_DELREPL		3		/* delete after replacing notes */
#define NOTEMOVER_DELMERGE		4		/* delete after merging notes */
#define NOTEMOVER_CROSSSTAFF	5		/* cross-staff selected notes */
#define NOTEMOVER_PREFIX		6		/* insert at front of target measure */
#define NOTEMOVER_APPEND		7		/* append at end of target measure */
#define NOTEMOVER_DELETE		8		/* delete notes */

/*--------------------------------------------------------------------------------*/

/* selection types for FX_SelectionDialog */

#define SELECT_ARTICULATIONS		1
#define SELECT_CHORDSUFFIXES		2
#define SELECT_SHAPES				3
#define SELECT_STAFFEXPRESSIONS		4
#define SELECT_SCOREEXPRESSIONS		5
#define SELECT_EXECUTABLESHAPES		6

/*--------------------------------------------------------------------------------*/

/* dlgtype for FX_Dialog */

#define DIALOG_MODELESS				0		/* modeless dialog (show it) */
#define DIALOG_MODAL				1		/* modal dialog */
#define DIALOG_INVISIBLE_MODELESS	2		/* invisible modeless dialog (don't show it)
												for faceless background processing in
												conjunction with FX_SetTimer
												Mac: also set visible in resource to NO */
#define DIALOG_MOVABLE_MODAL	    3		/* Mac movable modeless dialog (on windows this is
												the same as DIALOG_MODAL) */

/*--------------------------------------------------------------------------------*/

/* dlg control types for FX_Set/GetDlgCntl*/

#define DLGITEM_MENU		 0		/* combo box (win) or popup menu (Mac) */
#define DLGITEM_CHECK		 1		/* check box */
#define DLGITEM_RADIOBTN	 2		/* radio button */
#define DLGITEM_NUMTEXT		 3		/* numerical value in static/edit text */

#if OPERATING_SYSTEM == MAC_OS
#define DLGITEM_MISC		 4		/* Misc standard control on Mac (uses Set/GetControlValue) */
#endif

#define DLGITEM_3STATE_CHECK 5      /* 3 state checkbox */

/*--------------------------------------------------------------------------------*/

WINCODE( typedef LRESULT FX_RETURNVAL; )
MACCODE( typedef tbool FX_RETURNVAL; )

#endif

