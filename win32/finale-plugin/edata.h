/*
File Name:	EDATA.H

Copyright 1997 Coda Music Technology (all rights reserved)

Author:  	Tim Wright
Created:	02/06/97

Purpose:
	Defines tags and structures for Enigma Data.

Modification History:

**  02/07/97	TNW		Removed ETAG from EOther, DetailID.
**  02/07/97	TNW		corrected MAKEEXTAG, ed_MeasureFloat-> dt_MeasureFloat.
**  02/13/97	TNW		added ot_StaffSpec info, extra documentation for ot_MeasureSpec.
**  03/05/97	TNW		added ot_NewGroup info
**  03/05/97	TNW		took out public, private from EXNEWGROUP. for c compatibilty
**  03/05/97	TNW		fixed EDataID union declaration for c compatibility.
**  03/07/97	TNW		Changed EDataType from enum to utwobyte.
**  03/17/97	TNW		more documentation on staff, groups, measures.
**  03/21/97	TNW		added EXPERFDATA (performance data), EXINSTUSED (iulists),
**                      and EXTDIALATION (tempo tool tempo)
**  03/21/97	TNW		TDIALATION -> EXTDIALATION
**  03/31/97	TNW		added staff,score,text, shape expressions and shapes.
**  04/01/97	TNW		fixed patch type defines.
**  04/03/97	TNW		added edPrefs data type, pf_GeneralPrefs tag and
**						EXGENERALPROGOPTS; modified comment about velocity in EXPERFDATA.
**  04/04/97	TNW		note about flagging entry ef flag when it has EXPERFDATA
**  04/04/97	TNW		Massive renaming (define EXUSEOLDNAMES if really have to use the old names)
**						added text blocks.
**  04/07/97	TNW		added lyrics, text tags.
**  04/10/97	TNW		explanation of sizefont in EDTTextExpression. edt_* tags (entry details)
**						changed to ed_*
**  04/11/97	TNW		more comments about ed* defines, E*ID types, added kNewInci, kNewCmper,
**						etc. to have CreateEData get new unused value for the id fields.
**						EDTTimeDialation -> EDTTempo, started adding repeat stuff. (not ready yet)
**  04/11/97	TNW		ENIGMA_EFX_* -> FONT_EFX_*
**  04/16/97	TNW		incident->rawTextNum in EDTLyric, moved struct names, tag names, other
**						cmper/cmper2/incident explanations so they are all next to eachother.
**						(have to scroll around less to get frequently needed info). size/font
**						were flipped in explanation of sizefont in EDTTextExpression.
**						MeasureFloats, MeasureTextBlocks ids have inst in cmper1 and
**						meas in cmper2 not other way around.
**  04/17/97	TNW		added UseFloatQuantInEntryMover,AllowRedrawInterrupt,RedrawActiveWindowOnly,
**				   DottedRestsAreLegal to EDTGeneralPrefs
**  04/17/97	TNW		comments about text block cmpers, Staff, Group name id's
**  04/24/97	TNW		Finished up repeats, added enclosures. (updated Text Expression to give
**						info about Text Expression enclosures)
**  04/25/97    RES		added EDTPageSpec
**  05/05/97    TNW		added EDTEnigmaFont,EDTMeasNumberRegion,EDTMeasNumberSeparate
**  05/06/97    TNW		added comments on CreateEData for StaffSpec (takes a slot, returns an inst)
**  05/07/97    TNW		added EDTTuplet, starting to add EDTArticulation.
**  05/08/97    RES       added EDTStaffSystemSpec
**  05/08/97    RES       removed AAAA from EDTPageSpec
**  05/09/97    RES       added EDTStaffEnduction
**  05/09/97    TNW       added softEBEATs to prefs.
**  05/12/97    TNW       Corrected docu of EDTEndingRepeat, EDTTextRepeatStyle
**  05/21/97    TNW       moved preferences to EPRFDATA.H, entry details to EEDDATA.H
**  05/28/97    TNW       added EDTChordSuffix, EDTChordPlayback, EDTMidiExpression,
**						finished EDTArticulationDefinition.
**  06/02/97    TNW       changed ot_MeasNumberRegion, ot_ChordSuffix to edSpecialOther
**  06/16/97    RES       added SHAPE_EXP_NOPRINT, TEXT_EXP_NOPRINT
**  06/16/97    RES       linedeline --> lineSpacing
**  06/16/97    RES       TEXTBLOCK_LDLINEISPERCENT -->
**						TEXTBLOCK_LINESPACINGISPERCENT
**  06/16/97    RES       TEXTBLOCK_NOINTERLETR -->
**						TEXTBLOCK_NOEXPANDSINGLEWORD
**  06/16/97    TNW       moved some stuff to extypes.h
**  06/23/97    TNW       note that raw text is 1-based
**  07/03/97    TNW       removed EDTEnigmaFont from measure number typedef.
**						(don't want measure number struct to depend on EDTEnigmaFont)
**  07/07/97    RES       corrected unclosed comments
**  07/16/97    TNW       Made EDTTextExpression an even 14 bytes.
**  08/07/97    TNW       Comments about which id's are 1-based (all cmper/cmper1/cmper2 except
**						IULists, which are 0 based).  Also noted that all inci's are 0-based.
**  08/28/97    TNW      note that size of text data includes null terminating character.
**  09/05/97    RES      added MEAS_ALTNUMTSIG, MEAS_ALTDENTSIG
**  09/16/97    TNW      comments
**  10/03/97    RES       ot_TextRepeat is edOther, not edSpecialOther
**						(#909306)
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  10/10/97    TNW      added #define for IU_TEMP.
**  10/10/97     edata.h        RES       define IU_TEMP the same way it is
**						defined in globdecs.h (cast to CMPER)
**  10/10/97     edata.h        RES       after some thought, I thought it would
**						be better *not* to check whether IU_TEMP is already
**						defined before redefining it (see notes below)
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  11/25/97    TNW      comments about EDTTextRepeatStyle multiple actuators.
**  12/31/97     edata.h        RES       dt_LP --> dt_DTPERCENT
**  12/31/97     edata.h        RES       dt_FL --> dt_FLOATS
**  12/31/97     edata.h        RES       dt_ME --> dt_MIDIEXPRS
**  01/03/98     edata.h        RES       ot_MS --> ot_MSPEC
**  01/04/98     edata.h        RES       ot_IU --> ot_INSTUSED
**  01/05/98     edata.h        RES       ot_BR --> ot_BACKREPT
**  01/05/98     edata.h        RES       ot_ES --> ot_ENDSTART
**  01/05/98     edata.h        RES       ot_RU --> ot_REPSUSED
**  01/05/98     edata.h        RES       ot_RS --> ot_REPTSPEC
**  01/05/98     edata.h        RES       ot_EE --> ot_REPTENUM
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  01/12/98     edata.h        RES       ot_DY --> ot_DYNAMIC
**  01/12/98     edata.h        RES       ot_DT --> ot_DTXTSPEC
**  01/12/98     edata.h        RES       ot_DO --> ot_DOBJSPEC
**  01/12/98     edata.h        RES       ot_SD --> ot_SHAPEDEF
**  01/12/98     edata.h        RES       ot_MN --> ot_MEASNUMB
**  03/18/98     edata.h        evs       Added STAFF_FLAT_BEAMS
**  04/27/98     edata.h        RES       added dt_Gfhold, EDTgfhold, etc.
**  04/30/98     edata.h        RES       removed #ifdef __cplusplus (always C++ now)
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  07/15/98     EDATA.H        RES       removed MEASFLOAT_DONTUSEKEY,
**						MEASFLOAT_DONTUSETIME (anachronisms)
**  09/04/98     edata.h        RES       ot_PD --> ot_PLAYDUMP_TEXT
**  09/21/98     edata.h        RES       POUNDaux* --> POUND_REPLACE_*
**  09/21/98     edata.h        RES       auxdata2 --> playPass
**  11/17/98     edata.h        RES       ot_IK --> ot_CHSUFFIXPLAY
**  11/17/98     edata.h        RES       ot_IV --> ot_CHSUFFIX
**  11/17/98     edata.h        RES       INTVBANK --> CHSUFFIX
**  11/18/98     edata.h        RES       DATABLOCK --> OTHERBLOCK
**  11/18/98     edata.h        RES       OTHERBLOCK --> CHSUFFIXPLAY
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
**  02/06/99     edata.h        RES       STAFF_HIDE_REPEAT_DOTS --> STAFF_HIDE_REPEAT_TOP_DOT;
**						added STAFF_HIDE_REPEAT_BOTTOM_DOT
**  03/29/99     edata.h        RES       Removed GF_CONVERTNOTATION again. It is 
**						no longer a valid value.
**  04/20/99     edata.h        RES       FXUseInvisBeatsNotation --> FXUseBlankNotation
*//*~*/
/*
$Log: edata.h,v $
Revision 1.1.1.1  2002/12/19 19:13:11  dfober
first import

** 
** 23    8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 22    7/20/99 4:33p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 21    6/08/99 1:33p Jlenarz
** Added additional flags for altFlag of EDTStaffSpec.
** 
** 20    6/03/99 2:45p Rstokes
** added egraf.h
** 
** 19    5/28/99 11:31a Jlenarz
** Removed oops comma
** 
** 18    5/28/99 11:14a Jlenarz
** Added STAFF_ALTNOTATION
** 
** 17    5/27/99 4:47p Jlenarz
** Updated EDTStaffSpec to reflect changes to STAFFSPEC
** 
** 16    5/26/99 3:29p Jlenarz
** Updated EDTMeasureSpec to reflect changes to MSPEC
** 
** 15    5/26/99 2:27 PM Jlenarz
** Update to REPEAT_* flag values
** 
** 14    5/25/99 2:01 PM Jlenarz
** Updated EDTStaffSpec to reflect changes to STAFFSPEC. Added
** ECurrStaffSpecID which is used to load the STAFFSPEC for a given
** staff/meas/edu.
** 
** 13    5/25/99 10:23 AM Jlenarz
** Added handling of globals via existing API
** 
** 12    5/21/99 1:43p Beth
** Updated EDTInstrumentUsed to the new Finale data structures.
** 
** 11    4/21/99 12:30p Rstokes
** FXUseInvisBeatsNotation --> FXUseBlankNotation; also, last checkin was
** royally messed up, and I'm still not quite sure how it happened!
** 
** 9     3/30/99 5:10p Rstokes
** Removed GF_CONVERTNOTATION again. It is no longer a valid value.
** 
** 8     3/29/99 11:42 AM Tfischer
** GF_CONVERTNOTATION is still needed...
** 
** 7     3/26/99 5:37p Rstokes
** removed GF_CONVERTNOTATION, FXTwoBarRepeatBlankEnd
** 
** 6     3/26/99 5:19 PM Tfischer
** The last checkin caused a naming clash in Finale proper.  I'm renaming
** CONVERTNOTATION and it's components to FX...
** 
** 5     3/26/99 4:29 PM Tfischer
** Move CONVERTNOTATION enum outside of EDTGfhold so plug-ins can compile
** correctly
** 
** 4     2/18/99 12:58p Rstokes
** changed GF_CONVERTNOTATION to enum CONVERTNOTATION
**
** 3     2/08/99 4:50p Rstokes
** see comments above
 *
 * 2     1/28/99 2:41p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

/*#############################################################################*/

#ifndef _EDATA_H_
#define	_EDATA_H_

#ifndef _VERSION_H_
#include "version.h"
#endif

#ifndef _SHAPETAG_H_
#include "shapetag.h"
#endif

#ifndef _EXTYPES_H_
#include "extypes.h"
#endif

#ifndef _EGRAF_H_
#include "egraf.h"
#endif

/*#############################################################################*/

typedef utwobyte EDataType;

	/*these are also the high-level bytes of an EXTAG
	  so we can deduce the storage method given an
	  EXTAG.*/

#define edInvalidType MAKETAG('?','?')			/*type is undefined.*/

/*basic low-level types:*/

#define edOther      MAKETAG('o','t') /*stored as one other per struct.*/
#define edDetail     MAKETAG('d','t') /*stored as one detail per struct. */
#define edEntry      MAKETAG('e','t') /*stored in entry pool as entry. */
#define edText       MAKETAG('t','x') /*low-level text pool. */
#define edPrefs      MAKETAG('p','f') /*preferences. */
#define edGlobals	 MAKETAG('g','b') /*global data */

#define edEntryDetail MAKETAG('e','d')	/*detail attached to entry. */

/* # others/details/entry details stored in second char: */

#define edMultiInciOther  'O'
#define edOther2Inci  MAKETAG(edMultiInciOther,2)	/* 2 other incidences/struct */
#define edOther3Inci  MAKETAG(edMultiInciOther,3)	/* 3 other incidences/struct */
#define edOther4Inci  MAKETAG(edMultiInciOther,4)	/* 4 other incidences/struct */

#define edMultiInciDetail 'D'
#define edDetail2Inci MAKETAG(edMultiInciDetail,2)	/* 2 detail incidences/struct */
#define edDetail3Inci MAKETAG(edMultiInciDetail,3)	/* 3 detail incidences/struct */
#define edDetail4Inci MAKETAG(edMultiInciDetail,4)	/* 4 detail incidences/struct */

#define edMultiInciEntryDetail 'E'
#define edEntryDetail2Inci MAKETAG(edMultiInciEntryDetail,2)	/* 2 entry detail incidences/struct */
#define edEntryDetail3Inci MAKETAG(edMultiInciEntryDetail,3)	/* 3 entry detail incidences/struct */

#define edOtherString   MAKETAG('o','s') /* c-string stored as multi-incidents of others. */
#define edDetailString  MAKETAG('d','s') /* c-string stored as multi-incidents of details. */

#define edOtherNullTermArray   MAKETAG('o','n') /* null-terminated array stored as multi-incidents of others. */

//#define edDetailNullTermArray  MAKETAG('d','n') /* null-terminated array stored as multi-incidents of details. */
//#define edOtherArray   MAKETAG('o','a') /* array stored as multi-incidents of others. */
//#define edDetailArray  MAKETAG('d','a') /* array stored as multi-incidents of details. */

/* Used for class-oriented data, or data that needs to be handled specially.  */
/* In most cases, passing NULL as a databuf to LoadEData will create a default
   appropriate for the data type */

#define edSpecialOther  MAKETAG('S','O') /* struct stored in others records */
#define edSpecialDetail MAKETAG('S','D') /* struct stored in details records */
#define edSpecialEntryDetail MAKETAG('S','d') /* entry detail */

/*#############################################################################*/

/* ID Types.  The type of ID passed to the *EData APIs should match the type of data
   you are operating on (stored in the top word of the EXTAG)
*/

/*-----------------------------------------------------------*/
/* use with edOther, edOther?Inci, edOtherString, edSpecialOther types */

typedef struct
{
	CMPER cmper;
	twobyte inci;
}  EOtherID;

/*-----------------------------------------------------------*/
/* use with edDetail, edDetail?Inci, edSpecialOther, edSpecialDetail types */

typedef struct
{
	CMPER cmper1;
	CMPER cmper2;
	twobyte inci;
} EDetailID;

/*-----------------------------------------------------------*/
 /* use with edEntry types */

typedef struct
{
	ENTNUM entryNumber;
} EEntryID;

/*-----------------------------------------------------------*/
/* use with edEntryDetail types */

typedef struct
{
	ENTNUM entryNumber;
	twobyte inci;
} EEntryDetailID;

/*-----------------------------------------------------------*/
/* use with edText types */

typedef struct
{
	twobyte rawTextNumber;
} ETextID;

/*-----------------------------------------------------------*/
/* use with edPrefs types. Only supports LoadEData/SaveEData operations */

typedef struct
{
	twobyte prefNumber;
} EPrefsID;

//------------------------------------------------------------------------
// Use with edGlobals types. Only supports LoadEData/SaveEData operations

typedef struct
{
	twobyte globalsNumber;
} EGlobalsID;

//------------------------------------------------------------------------
// Use with edCurrStaffSpec types.

typedef struct
{
	twobyte  staffNum;
	twobyte  measNum;
	TIME_EDU eduPos;
} ECurrStaffSpecID;


/*#############################################################################*/

#define kNewInci		-1		/* Set inci to this in CreateEData to get new incident. */
#define kNewCmper		0xffff	/* Set cmper/cmper1 to this in CreateEData to get new cmper. */
#define kNewCmper2		0xffff	/* Set cmper2 to this in CreateEData to get new cmper2. */
#define kNewEntryNum	0		/* Set entryNumber to this in CreateEData to get new entry number. */
#define kNewRawTextNum	-1		/* Set rawTextNumber to this in CreateEData to get new entry number. */

/*
** NOTE:
** It is recommended that you always set inci to kNewInci (or 0 if it is unused for the
** data type) when calling CreateEData.
** MeasureSpec can only be created w/kNewCmper, GroupSpec cannot be created w/cmper1==kNewCmper
** (since it needs an iuList). SpecialOther, SpecialDetail types ignore inci,
** String types can only be created w/kNewInci, but use inci during LoadEData.
*/

/*#############################################################################*/

typedef union {
	EOtherID other;
	EDetailID detail;
	EEntryID entry;
	EEntryDetailID entryDetail;
	ETextID	text;
	EPrefsID prefs;
	EGlobalsID global;
	ECurrStaffSpecID staffSpec;
} EDataID;

/*#############################################################################*/

typedef ufourbyte EXTAG;

#define	MAKEEXTAG(type,c,d)	(ufourbyte)( ((ufourbyte)type<<16) | (ufourbyte)MAKETAG(c,d) )

/*#############################################################################*/

/**** General Purpose Flags *****/


/* Justification types for text blocks (how text is positioned relative to the text block): */

/* ~ JUSTIFY_LEFT..JUSTIFY_FORCED_FULL, TEXTBLOCK.H */

#define TEXT_JUSTIFY_LEFT 0
#define TEXT_JUSTIFY_RIGHT 1
#define TEXT_JUSTIFY_CENTER 2
#define TEXT_JUSTIFY_FULL 3
#define TEXT_JUSTIFY_FORCED_FULL 4


/* Alignment types for text blocks (how a text block is positioned relative to the page): */

/* ~ HALIGN_LEFT..HALIGN_CENTER, TEXTBLOCK.H */

#define TEXT_HALIGN_LEFT 0
#define TEXT_HALIGN_RIGHT 1
#define TEXT_HALIGN_CENTER 2

/* ~ VALIGN_TOP..VALIGN_CENTER, TEXTBLOCK.H */

#define TEXT_VALIGN_TOP 0
#define TEXT_VALIGN_BOTTOM 1
#define TEXT_VALIGN_CENTER 2

/* Barline types for staves and groups: */

/* ~ NOBARLINE..PARTIALBAR, OTHERDEC.H */

#define BARLINE_NOBARLINE	0		/*no barline (invisible)*/
#define	BARLINE_NORMALBAR	0x0001	/*'normal' barline*/
#define	BARLINE_DBLBAR		0x0002	/*double barline*/
#define	BARLINE_DASHBAR		0x0003	/*dashed barline*/
#define BARLINE_SOLIDBAR	0x0004	/*solid (thick) barline*/
#define	BARLINE_FINALBAR	0x0005	/*final (thin and thick) barline*/
#define	BARLINE_PARTIALBAR	0x0006	/*'tick' barline.*/

/* Bracket types for groups: */

/* ~ PLAINBRAC..REVDESKBRAC, BRACKETS.H */

#define BRACKET_NONE				0
#define BRACKET_PLAINBRAC			1
#define BRACKET_CHORUSBRAC			2
#define BRACKET_PIANOBRAC			3
#define BRACKET_REVCHORUSBRAC		4
#define BRACKET_REVPIANOBRAC		5
#define BRACKET_CURVECHORUSBRAC		6
#define BRACKET_REVCURVECHORUSBRAC	7
#define BRACKET_DESKBRAC			8
#define	BRACKET_REVDESKBRAC			9

/* Clefs: */

/* ~ ClefChart, GRUTIL.H */

#define CLEF_TREBLE				0
#define CLEF_ALTO				1
#define CLEF_TENOR				2
#define CLEF_BASS				3
#define CLEF_PERCUSSION			4
#define CLEF_TREBLE8VB			5
#define CLEF_BASS8VB			6
#define CLEF_BARITONE			7



/*####################################################################################*/

/*  Database structures/tags: Where a name is in quotes it refers to a control in the dialog
**  normally used to change the info in Finale (e.g. ot_MeasureSpec: Measure Attributes
**  dialog, ot_StaffSpec: Staff Attributes dialog.
**
**
**  All unused/undocumented bits are reserved for future, always preserve
**  in loaded data, set to zero in created data (unless otherwise specified)
*/

/*####################################################################################*/

/* Raw Text */

/* Text Tags - Access raw text using these tags (See raw text documentation
** in ETFSPEC.TXT for text format, text caret info)
**
** Use an ETextID with FX_LoadEData to load raw text.
** ETextID.rawTextNumber is obtained through record
** referencing the text. (given after each tag definition below)
**
** Text data is variable length, so LoadEData with NULL to get length first.
** Text is null-terminated, and size param of text data in FX_LoadEData, etc.
** includes the NULL terminating character.
*/

#define tx_TextBlock		MAKEEXTAG(edText, 'b', 'l') /* textID in EDTTextBlock (1-based) */
#define tx_VerseLyric		MAKEEXTAG(edText, 'v', 'e') /* incident in EDTLyric stored under ed_VerseLyric (1-based) */
#define tx_SectionLyric		MAKEEXTAG(edText, 's', 'e') /* incident in EDTLyric stored under ed_SectionLyric (1-based) */
#define tx_ChorusLyric		MAKEEXTAG(edText, 'c', 'h') /* incident in EDTLyric stored under ed_ChorusLyric (1-based) */

/*####################################################################################*/

/* EDTMeasureSpec

** Most of this information is directly related to fields in the measure attributes
** dialog (measspace, auxflag, meflag).  key correspond to the value set using the key
** signature tool and beats, divbeat to values set in the time signature tool, and
** repeat barline to values in the repeat selection dialog.
**
** Bits in auxflag, meflag all correspond to settings in Measure Attributes Dialog
** (dialog option text is in quotes after each bit)  with the exception of
** MEAS_SMARTSHAPEBIT, MEAS_MNSEPPLACE, MEAS_DYNAMBIT, MEAS_MS_ARBITMUSIC, MEAS_MEASURETEXT,
** MEAS_REPTBITS (accessed through the repeat selection dialog)
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte measpace;		/*measure width in EVPUs (measure attributes dialog)*/
	twobyte key;			/*key sig, linear keys are <16384; low byte == # sharps/flats, (key signature dialog)*/
	twobyte beats;			/*beats per measure	  (time signature dialog, and divbeat, too)*/
	twobyte divbeat;		/*EDU/beat (1024 = quarter note) (example beats=2, divbeat = 1536 means 6/8)*/
	FLAG_16 auxflag;		/*(measure attributes, repeat selection dialogs) see below:*/

/* -------- auxflag  bits: -------- */

/* ~ HIDECAUTION..BEATCHARTplusPOS, OTHERDEC.H */

#define MEAS_HIDECAUTION	0x4000	/* "Hide Cautionary Clefs/Key And Time Signatures"*/
#define MEAS_SMARTSHAPEBIT	0x2000	/* system use only (do not modify) Smart Shape starts, ends, or passes through measure*/

#define MEAS_GRP_BARLINE_OVERRIDE 0x1000 	/* "Override Group Barline Styles" */
#define MEAS_MNSEPPLACE	0x0400		/* Measure number for this measure has it's own positioning. */
#define MEAS_POSSPLIT	0x0100		/* "Allow horizontal split points" */

#define MEAS_ALTNUMTSIG	0x0080		/* if set, top of time sig is composite */
#define MEAS_ALTDENTSIG	0x0040		/* if set, bottom of time sig is composite */
#define MEAS_IGNOREKEY	0x0020		/* Key Signature Popup - "Always hide" */
#define MEAS_IGNORETIME	0x0010		/* Time Signature Popup - "Always hide" */

#define MEAS_INDIVPOSDEF	0x0008		/* "Position Notes Evenly Across Measure" */
#define MEAS_POSDEFBITS		0x0007		/* Position Notes popup - Note Positioning Mode (= one of following values) */
#define	MEAS_SCALEPOSITS	0		/* "Manually (by dragging)" */
#define	MEAS_useTIMESIG		1		/* "According to the Time Signature" */
#define	MEAS_useBEATCHART	2		/* "Use Beat Chart Spacing" */
                                    /* 3 is reserved */
#define	MEAS_POSareOFFSETS		4	/* "According to the Time Signature" plus individual positioning*/
#define	MEAS_POSareABSOLUT		5	/* Use the measure's positioning mode in ossias (clumsy!) */
#define	MEAS_BEATCHARTplusPOS	6	/* "Use Beat Chart Spacing" plus individual positioning*/

/*-----------------------------------*/

	FLAG_16 meflag;			/*""*/

/* -------- meflag  bits: -------- */

/* ~ MS_LINEBREAK..MEASURETEXT, OTHERDEC.H */

#define MEAS_MS_LINEBREAK	0x8000	/* "Begin a New Staff System" */
#define MEAS_DYNAMBIT		0x4000	/* measure has at least 1 score expression (EDTScoreExpression) */
#define	MEAS_BREAKREST		0x2000	/* "Break multi-measure rest" */
#define MEAS_RIOVERRIDE		0x1000 	/* system use only (do not modify) */

#define MEAS_DELTAKEY		0x0800	/* "Key Signature" Popup - "Always show" */
#define MEAS_DELTATIME		0x0400	/* "Time Signature" Popup - "Always show" */
#define MEAS_MS_ARBITMUSIC	0x0200	/* is a source measure for ossia */
#define MEAS_MEASURETEXT	0x0100	/* measure has at least 1 measure text-block (EDTMeasureText)*/

/* ~ BARLINEBITS..REPEATS, OTHERDEC.H */

#define	MEAS_BARLINEBITS	0x00F0	/* measure's barline type ("Barline:" button) */
                                    /* (any of BARLINE_* #defines, shifted up 4 bits) */
#define MEAS_REPTBITS		0x000F	/*repeat barline type. ("Repeat Selection" Dialog)*/
#define		MEAS_FORREPBAR	0x0008	/*Measure has forward repeat barline*/
#define		MEAS_BACREPBAR	0x0004	/*Measure has backward repeat barline, measure should
									  have an EDTBackRepeat which gives parameters for back Rept */
#define		MEAS_BARENDING	0x0002	/* Measure has repeat bracket.  If MEAS_BACREPBAR set,
									   bracket is an endbracket & bracket parameters are in
									   EDTBackRepeat, otherwise  bracket is a starting bracket
									   & bracket parameters are in EDTStartRepeatEnding */
#define		MEAS_REPEATS	0x0001	/*text repeat. */
/*-----------------------------------*/

//** Possible future expansion for display time sig:
//**	additional x/y offsets
//**	space between numerator and denominator
//**	change font/size/efx on an individual basis
//**	parenthesize

	twobyte dispBeats;
	twobyte dispDivbeat;
	FLAG_16 newflag;
	twobyte custombarshape;
	twobyte customleftbarshape;
	twobyte CCCC;		// unused

// newflag contains the following defined flags:
#define	MEAS_PARENTIME	0x0001		/* parenthesize the time sig (unimplemented) */
#define	MEAS_ABBRVTIME	0x0002		/* abbreviate common or cut time (gives
									individual control over ABRV44 et al.) Note
									that this is an attribute only of a display
									time sig (could be made part of functional
									time sig as well, but we'd need another bit). */
#define MEAS_USE_DISPLAY_TIMESIG	0x0004
											// 0x0008 is available
#define MEAS_LEFT_BARLINEBITS		0x00F0  // Left Barline styles, as defined by BARLINE_STYLE
#define MEAS_DISPLAY_ALTNUMTSIG	0x0100
#define MEAS_DISPLAY_ALTDENTSIG	0x0200


}  EDTMeasureSpec; /* ~ MSPEC, ot_MSPEC, OTHERDEC.H */

#define ot_MeasureSpec MAKEEXTAG(edSpecialOther,'M','S')
/* cmper=measure (1-based), inci = 0 */

/*####################################################################################*/

/* EDTMeasureFloat (Independent Key and Time)
**
**
** If this record exists for a measure, it overrides the timesig and/or key sig in
** the EDTMeasureSpec record for the instrument. Staff Spec must have FLOATKEYS or FLOATTIME
*/

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte key;
	twobyte bts;
	twobyte div;
	twobyte AAAA;				/* unused */
	FLAG_16 flag;
/*--------flag bits:------*/

/* ~ HASKEY..ALTDENTSIG, DTAILDEC.H */

#define MEASFLOAT_HASKEY		0x0800
#define MEASFLOAT_HASTIME		0x0400
//#define MEASFLOAT_DONTUSEKEY	0x0200
//#define MEASFLOAT_DONTUSETIME	0x0100
#define MEASFLOAT_ALTNUMTSIG	0x0080
#define MEASFLOAT_ALTDENTSIG	0x0040
/*-------------------------*/

} EDTMeasureFloat; /* ~ FLOATS, dt_FLOATS, DTAILDEC.H */

/*-----------------------------------------------------------------*/

#define dt_MeasureFloat MAKEEXTAG(edDetail,'F','L')

/* cmper1= instrument (1-based), cmper2=measure (1-based),inc=0 */

/*####################################################################################*/

/* EDTStaffSpec
**
** Most of this information is directly related to fields in the Staff attributes
** dialog. (also, position full, abbrv staff name, and Staff Setup)
*/

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte botBarlineOffset;	/* "Staff Setup" Dialog: "Bottom Barline"
									(offset in EVPUs to barline bottom from bottom staff line). */
	twobyte baseyoff;			/* "Tablature" Dialog: hi-order byte is "Base Key" (midi number),
								   lo-order is "Distance from Topline in Staff" (in points)
								   (distance from the top line of staff to baseline of fret numbers) */
	FLAG_16 altFlag;			

// flag bits for altFlag

#define STAFF_ALTNOTATION				0x000F	// mask for enum FX_CONVERTNOTATION
#define STAFF_ALTLAYER					0x00F0	// layer for alt notation
#define STAFF_ALT_SHOW_DETAILS			0x0100	// show details on altered layer
#define STAFF_ALT_SHOW_OTHER_LAYERS		0x0200	// show entries on other layers
#define STAFF_ALT_SHOW_OTHER_DETAILS	0x0400	// show details on other layers

	/* set either through staff attributes or tablature dialog (if tab staff): */

	twobyte mfont;				/* font id for tablature or noteheads, (use FX_FontNumToName()/FX_FontNameToNum())*/
	twobyte sizeefx;			/* msb: font size in points; lsb: any of FONT_EFX_* */

	FLAG_16 flag;

/*------ flag bits: -------*/

/* ~ DOPERCUSSIONNOTES..NO_OPTIMIZE, DTAILDEC.H */

#define	STAFF_HIDE_REPEAT_BOTTOM_DOT	0x8000	// "Staff Setup"- "Hide Bottom Repeat Dot"
												// added 2/6/99 RES
#define STAFF_FLAT_BEAMS		0x4000   /* Controls the drawing of flat beams for the staff */
#define STAFF_DOPERCUSSIONNOTES	0x2000   /* "Notation Style" popup - "Percussion" */
#define STAFF_NEGREHEARSALMARK	0x0400	 /* not used */
#define STAFF_TABNOTES			0x0200   /* "Notation Style" popup - "Tablature" */
#define STAFF_BLANKMEASURE		0x0100   /* "Display rests in empty measures" */
#define STAFF_SF_DOSHAPENOTES 	0x0080	 /* "Notation Style" popup - "Note Shapes" */
#define	STAFF_HIDE_REPEAT_TOP_DOT		0x0040	/* "Staff Setup"- "Hide Top Repeat Dot"
												if HIDE_RPT_BARS in instflag is not set,
												just hide top repeat dot. */
												// 2/6/99: was STAFF_HIDE_REPEAT_DOTS
#define STAFF_USESMFONT 		0x0020	 /* "Independent Elements" - "Notehead Font" */
#define	STAFF_NO_OPTIMIZE		0x0001	 /* "Allow Optimization" check (inverse)
									       If set, this staff should never be optimized
									        away even when empty (like piano staves) */
/*-----------------------------*/
	twobyte clefs;		/* lo-order byte: "First Clef" (any of the CLEF_* #defines)
						   hi-order byte: "Transposition" Dialog: "Set to Clef" clef */

/* If bottom bit of topLines is 1, then topLines,botLines are
   bit masks for the staff lines (above and below the ref line)
   selected in "Staff Setup" dialog.

   However, If topLines is 0, then botLines is the
   number of staff lines.  (non-custom staff)
*/

	FLAG_16 topLines;	/* represents the top 16 staff lines above
                   		   Finales reference staff line (the one at
                   		   top + <separ from EDTInstrumentUsed for this staff> )
                   		   (from Staff Setup Dlg.) */

	FLAG_16 botLines;	/* represents the bottom 16 staff lines below and
	 					   including Finales reference staff
                   		   line (the one at top + <separ from
                   		   EDTInstrumentUsed for this staff>)
                   		   (from Staff Setup Dlg.)  */

	twobyte topBarlineOffset;   /* "Staff Setup" Dialog: "Top Barline" */
			                   	/* (EVPUs offset to barline top from top staff line) */
	twobyte transposition;		/* if 0, then no transposition, otherwise transposition is read as following: */

/*-------- transposition bits: (From "Transposition" Dialog) -------- */

#define STAFF_SETTOCLEF 	0x8000		/* "Set to Clef" - Use hi-order byte in clefs for clef */
#define STAFF_NOKEYOPT  	0x4000		/* "Simplify Key" (inverse) */
								/* (simplify key means select the representation */
								/* that minimizes the number of sharps and flats. */
								/* In western major/minor, seven flats would */
								/* be replaced by five sharps. */
								/* (used only when CHROMTRANS is clear) */

#define	STAFF_CHROMTRANS	0x2000		/* "Chromatic" if set, the transposition flag is */
								/* interpreted differently. For use when you */
								/* don't want the key sig altered to effect an */
								/* instrument transposition (non-tonal music, or */
								/* music without key sig). The key signature is */
								/* not altered, and a chromatic transposition is */
								/* held in the CHROMALT and CHROMDIA bits. */

/* these values can be set indirectly in the transposition dialog are automatically when a
transposition is selected from the popups: */

#define STAFF_INTVADJ	  	0x0FC0		/* "Interval" - used only for "Key Signature" (CHROMTRANS == NO) */
#define STAFF_KEYSIGADJ 	0x003F		/* "Key Alter" - used only for "Key Signature" (CHROMTRANS == NO) */
/* example, up a perfect fifth, add one sharp is INTVADJ==4 (4 more diatonic steps) and KEYSIGADJ==1, for add a sharp */

/* Are set directly in "Interval" dialog that comes up when "other" Chromatic Transposition is selected */
#define STAFF_CHROMALT	0x0F00		/* "Alter" - used only for "Chromatic" (CHROMTRANS == YES) */
#define STAFF_CHROMDIA	0x00FF		/* "Interval" - used only for "Chromatic" (CHROMTRANS == YES) */
/*-----------------------------*/


	FLAG_16 instflag;
/*-------- instflag bits: -------- */

/* bits for instflag: */
#define STAFF_FLOATKEYS		0x8000	/* "Independent Elements" - "Key Signature" */
#define STAFF_FLOATTIME		0x4000	/* "Independent Elements" - "Time Signature" */
#define STAFF_BLINEBREAK	0x2000	/* "Break barlines between staves" */
#define STAFF_RBARBREAK		0x1000	/* "Break repeat barlines between staves" */
#define STAFF_NEGDYNAMIC	0x0800	/* "Items to Display" - "Score Expressions" (inverse) */
#define STAFF_NEGMNUMB		0x0400	/* "Items to Display" - "Measure Numbers" (inverse) */
#define STAFF_NEGREPEAT		0x0200	/* "Items to Display" - "Endings and Text Repeats" (inverse) */
#define STAFF_NEGNAME		0x0100	/* "Items to Display" - "Staff Name" (inverse) */

/* These bits will hide the respective bars in a staff as
   long as the staff is not part of a group. Group barlines will override.
   (An open question is what happens if ALL staves in a group have hidden barlines.) */

#define STAFF_HIDE_BARLINES	0x0080	/* "Items to Display" - "Barlines" (inverse) */
#define STAFF_HIDE_RPT_BARS	0x0040	/* "Items to Display" - "Repeat Bars" (inverse) */
#define STAFF_NEGKEY		0x0020	/* "Items to Display" - "Key Signatures" (inverse) */
#define STAFF_NEGTIME		0x0010	/* "Items to Display" - "Time Signatures" (inverse) */
#define STAFF_NEGCLEF		0x0008	/* "Items to Display" - "Clefs" (inverse) */
#define STAFF_HIDESTAFF		0x0004	/* "Hide Staff" check */
#define	STAFF_NOKEY			0x0001	/* "Ignore Key Signatures" check */
/*-----------------------------*/

	/* Next three values come from the "Staff Setup" dialog */
	/* The dw_wRest, h_otherRest are for rest placement in terms of steps. */
	/* Furthermore, 0 means Finales top reference staff line, and */
	/* positive values go up, negative values down. */

	twobyte dw_wRest;		/* "Staff Setup" Dialog -  "Double Whole Rest" (lo onebyte)  and */
							/* "Whole Rest" (hi onebyte) */
	twobyte h_otherRest;  	/* "Staff Setup" Dialog - "Half Rest" (lo onebyte) and */
							/* "Other Rests" (hi onebyte)*/
	twobyte stemReversal;	/* "Staff Setup" Dialog - "Stem Reversal" */
							/* distance in harmonic levels from top line of */
							/* staff to stem reversal line; negative values */
							/* place the stem reversal line below the top of the */
							/* staff */
	twobyte fullName;		/* ot_TextBlock id for full staff name  (0==none) */
	twobyte abbrvName;		/* ot_TextBlock id for abbreviated staff name  (0==none) */
	twobyte ZZZZ;			/* unused (padding for STAFFSPEC) */
	
	// JPL 05/25/99 EDTStaffSpec needed to be updated to reflect changes to STAFFSPEC.
	// The fields above are now part of the base class STAFFSPEC_BASE, the 
	// following fields are the new ones added to STAFSPEC.

	twobyte staffID;

	// The following fields will differ according to whether this STAFFSPEC
	// references the base records for the givven STAFFSPEC or the records associated
	// with an applied style.

	ETAG shapeNoteTag;			// dt_SHAPENOTE or dt_SHAPENOTE_STYLE
	twobyte shapeNoteCmper;		// staff ID or style ID
	ETAG drumStaffTag;			// ot_DRUMSTAFF or ot_DRUMSTAFF_STYLE
	twobyte drumStaffCmper;		// staff ID or style ID
	ETAG fullNamePosTag;		// ot_NAMEPOS_Full or ot_NAMEPOS_Full_STYLE
	twobyte fullNamePosCmper;	// staff ID or style ID
	ETAG abbrvNamePosTag;		// ot_NAMEPOS_Abbrv or ot_NAMEPOS_Abbrv_STYLE
	twobyte abbrvNamePosCmper;	// staff ID or style ID

	EXMEASRANGE range;

} EDTStaffSpec; /* ~ STAFFSPEC, ot_STAFFSPEC, STFSPEC.H */

/*-----------------------------------------------------------------*/

#define ot_StaffSpec MAKEEXTAG(edSpecialOther,'I','S')
#define ot_CurrStaffSpec MAKEEXTAG(edSpecialOther,'S','C')

/* cmper=instrument (1-based), inci = 0 */
/* EXCEPT FOR CreateEData:
   cmper=slot to insert new staff (0 to append), inci = 0.
   Created staff becomes new slot 'cmper' and old slot cmper becomes slot cmper+1,
   old slot cmper+1  becomes slot cmper+2, etc.  CreateEData returns new INSTRUMENT
   in cmper */

/*##############################################################################*/

/* EDTGroupSpec

** Staff Grouping.
** Most of this information is directly related to fields in the group attributes
** dialog. (and the position Full/Abbrv name dialogs accessed from this dialog).
*/

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte id;					/* bracket ID, any of the BRACKET_* #defines. */
	twobyte bracpos;			/* horz */
	twobyte bractop;			/* vert top */
	twobyte bracbot;			/* vert bottom */
	FLAG_16 flag;				/* "bracket on single staves" */

/*------------- flag bits: ------------*/
/* ~ BRACKSPEC_BRACKET_ON_SINGLE, DTAILDEC.H */

#define BRACKET_BRACKET_ON_SINGLE 0x0001

/*-------------------------------------*/

} EDTBracketSpec; /* ~ BRACKSPEC, dt_BRACKSPEC, dtaildec.h */

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte startInst;          /* starting and ending instrument in group */
	twobyte endInst;
	twobyte fullNameID;			/* ot_TextBlock ID for full group name  (0==none) */
	twobyte fullNameXadj;
	twobyte fullNameYadj;		/* delta off of default position (centered)
								   (only enabled if GROUP_FULLNAME_INDIVPOS set)
								  ("Position Full Name" dialog) */

	EDTBracketSpec bracket;        /* description of bracket for group, lower part of "Group Attributes" */

	FLAG_16 flag;

/*------------- flag bits: ------------*/
/* ~ NEWGROUP_JUSTIFY_FULL..NEWGROUP_HIDE_NAME, NEWGROUP.C */

#define GROUP_JUSTIFY_FULL 0x0007;	/* Justification for full name text */
                                    /* (any of the TEXT_JUSTIFY_* #defines) */
#define GROUP_JUSTIFY_ABRV 0x0038;	/* Justification for abbrv. name text */
                                    /* (any of the TEXT_JUSTIFY_* #defines, << 3) */

#define GROUP_BARLINEBITS 0x03C0;	/* "Use Alternate Group Barline" barline type. */
                                    /* (any of the BARLINE_* #defines, << 6) */

#define GROUP_BARLINE_STYLE		    0x0C00	/* "Draw barlines" popup in the Group Attributes dialog */
#define 	GROUP_NORMAL_BARLINES   0x0000  /* through individual staves but not connecting */
#define 	GROUP_GROUP_BARLINES	0x0400  /* barline through all staves, connecting */
#define 	GROUP_MENSURSTRICHE		0x0800	/* barlines between staves, but not through the body of the staves */

#define GROUP_OWN_BARLINE       0x1000  /* "Use Alternate Group Barline" (independent barline is found in BARLINEBITS) */
#define GROUP_FULLNAME_INDIVPOS	0x2000	/* if set, activate fullNameX/Yadj fields (check by position button in "Group Attributes") */
#define GROUP_ABRVNAME_INDIVPOS	0x4000	/* if set, activate abrvNameX/Yadj fields (check by position button in "Group Attributes") */
#define GROUP_HIDE_NAME			0x8000  /* hide group name (opposite of "Show Group Name" checkbox */

/*-------------------------------------*/

	twobyte abrvNameID;			/* ot_TextBlock ID for abbreviated group name (0==none) */
	twobyte abrvNameXadj;
	twobyte abrvNameYadj;		/* delta off of default position (centered)
								   (only enabled if GROUP_ABRVNAME_INDIVPOS set)
								   ("Position Abrv. Name" dialog) */
	FLAG_16 auxflag;

/*------------- auxflag bits: ------------*/

#define GROUP_AUX_HALIGN_FULL		0x0003 /* alignment type for full group name ("Position Full Name" dialog) */
#define GROUP_AUX_HALIGN_ABRV		0x000C /* alignment type for abbreviated group name ("Position Full Name" dialog) */
/* (any of the TEXT_HALIGN_* #defines, shifted if needed.) */

#define GROUP_AUX_EXPAND_FULL		0x8000 /* expand single words in full group name (check in "Position Full Name" dialog) */
#define GROUP_AUX_EXPAND_ABRV		0x4000 /* expand single words in abrev. group name (check in "Position Abbrv Name" dialog) */

/*-------------------------------------*/

} EDTGroupSpec; /* ~ NEWGROUP, dt_NEWGROUP, newgroup.h */

/*-----------------------------------------------------------------*/

#define dt_GroupSpec MAKEEXTAG(edSpecialDetail,'N','G')

/* cmper=iuList (0-based), cmper2 = groupID (1-based), inci =0 */

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte clef;
	FLAG_16 flag;
	twobyte frame[ MAXLAYERS ];

/*------------- flag bits: ------------*/
#define GF_CLEFISLIST		0x0400
//#define GF_LAYERISLIST	0x0200

// (NOTE 7/26/95 RES): there currently isn't a way in Finale to set shape notes
// for a single frame, so GF_DOSHAPENOTES can't be set
#define GF_DOSHAPENOTES		0x0080
#define GF_MIRRORFRAME 		0x0040
//#define GF_USEMFONT 		0x0020			// RES 4/20/98: retired (never used)
#define	GF_HIDECLEF			0x0010			// RES 3/26/98

//#define GF_CONVERTNOTATION	0x000F

} EDTGfhold; /* ~ GFHOLD, dt_GFHOLD, gfhold.h */

enum FX_CONVERTNOTATION
{
	FXUseNormalNotation		= 0,	// RES 10/26/95
	FXUseSlashBeatsNotation	= 1,
	FXUseRhythmicNotation		= 2,
	FXUseOneBarRepeatSymbol	= 3,
	FXUseTwoBarRepeatSymbol	= 4,
//	FXTwoBarRepeatBlankEnd	= 5,	// 3/26/99 RES: obsolete
	FXUseBlankNotation		= 6		// added 7/18/90 -- RES
									// 4/20/99 RES: FXUseInvisBeatsNotation --> FXUseBlankNotation
};
/*-----------------------------------------------------------------*/

#define dt_Gfhold MAKEEXTAG(edSpecialDetail,'G','F')

/* cmper=inst (1-based), cmper2 = meas (1-based), inci =0 */

/*##############################################################################*/

/* EDTInstrumentUsed
**
** All the incidents under a single cmper define an instrument list (staff set).
** EDTInstrumentUsed indicates which staves are in the staff set, and the distances
** between them.  Each incident represents  a slot in the iulist.
** (slot == incident + 1, since slots are 1-based).
**
** Creating instrument lists:
**
** To be safe only create temporary iu lists (list of IU others) using the
** temp iu cmper, 65529.  You may need to create a temporary iu list, for
** example to play a certain selection of staves using FX_Playback
**
** If you create an iu list at another cmper, it may be overwritten by
** Finale.
*/

//#ifndef IU_TEMP
// 10/10/97 RES: After some thought, I thought it would be better *not* to
// check whether IU_TEMP is already defined before redefining it. Doing so
// would run the risk of defining it differently. Redefining it identically
// without checking first might generate a compiler warning at worst, but
// ensures that we don't redefine differently, which would generate a much
// stronger warning, if not an error. (IU_TEMP is also defined in GLOBDECS.H,
// which is not accessible to plug-ins. A better long-range solution might to
// have defines like this in a common place.)
#define IU_TEMP	((CMPER) 65529)
//#endif

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte inst;		/* instrument number for this slot */
	twobyte AAAA;		/* unused */
	twobyte BBBB;		/* unused */
	twobyte CCCC;		/* unused */
	fourbyte separ;		/* distance between top staff line of this slot and last in EVPUs */
//	MEASRANGE range;    /*  start meas of 0 means beginning of piece */
						/*  end meas of 0 means end of piece */

	// The following fields make up the data memebers of the MEASRANGE class above.
	twobyte		measstart_meas;  
	TIME_EDU	measstart_edu;
	twobyte		measend_meas;
	TIME_EDU	measend_edu;

} EDTInstrumentUsed; /* ~ INSTUSED, ot_INSTUSED, OTHERDEC.H */

/*-----------------------------------------------------------------*/

#define ot_InstrumentUsed	MAKEEXTAG(edSpecialOther,'I','u')

/* cmper = IUList (0-based), inci = (slot - 1, slots are 1-based, inci is 0-based)*/

/*##############################################################################*/

/* EDTTempo
**
** This is the time dialation info that you normally modify
** with the Tempo Tool. The correlation between the dialog and the record is:
**
** "Measure": (records are created under cmpers for each specified measure)
** "Unit": incident
** "Start Time in Measure": eldur translated to a beat
** "Set To": flag == TDIL_ABSOLUTE, ratio == beats per minute as EDUS per RTU, unit == RTU.
** "Change By": flag == TDIL_RELATIVE, ratio == %
*/

/*-----------------------------------------------------------------*/

typedef struct
{
	TIME_EDU eldur;		/* elapsed duration from start of measure to
						start of time dilation */
	fourbyte ratio;		/* if flag == relative, this is a percentage * 1000.
						Ex: for 50% reduction, this will be 500.
						if flag == absolute, this is the ratio of EDUs to
						RTUs, expressed as a 16 bit fixed point number. For
						the Mac, at 1000 RTUs/sec, a ratio of 60 bpm (a quarter note
						per second) would be ( 1024 << 16 ) / 1000. (You would store
						1000 in the unit field) */
	twobyte unit;		/* RTUs = hardware ticks/sec */
	FLAG_16 flag;		/* relative or absolute (only bottom bit is currently used) */

/*---------- flag bits: --------*/
#define	TIMEDIAL_RELATIVE	1
#define	TIMEDIAL_ABSOLUTE	0
/*------------------------------*/

} EDTTempo; /* ~ TDIALATION, ot_AC, OTHERDEC.H */

/*-----------------------------------------------------------------*/

#define ot_Tempo	MAKEEXTAG(edOther,'A','C')

/* cmper = measure (1-based), incident =  which tempo record for this measure (0-based),
should be in chronological order.*/

/*##############################################################################*/

/* EDTScoreExpression
**
** This is the base record of score expressions.
** Info is normally set in the score expression assignment dialog:
**
** EDTMeasureSpec for the measure should have its meflag flagged with
** MEAS_DYNAMBIT;
*/

/*-----------------------------------------------------------------*/

typedef struct   /* DY */
{
	twobyte dynumber;   /* cmper for shape (ot_ShapeExpression) or text (ot_TextExpression) expression */
	twobyte posadd;		/* horiz EVPU pos from left of measure */
	twobyte lineadd;	/* vertical EVPU pos from top of measure */
	twobyte IndivPos;   /* if SEPRPLACE set, identifies ot_DynamSeparatePlacement record
						   for this expression (matches  measure in EDTSeparates, needs
						   to be unique for all EDTScoreExpressions in a particular measure */
	twobyte current;    /* internal use only, set to 0 */
	FLAG_16 flag;

/*---------- flag bits: --------*/

#define	SCOREEXP_OBJECT_TYPE		0x6000
#define 	SCOREEXP_TEXT_OBJECT	0x0000 /* text expression */
#define		SCOREEXP_SHAPE_OBJECT	0x2000 /* shape expression */

#define SCOREEXP_LISTINST	0x1000		/* if set, LISTNUM contains an instrument num */
								        /*  else it is a staff list cmper (ot_InstrumentUsed) */
#define SCOREEXP_SEPRPLACE	0x0800      /* allow separate placement for each staff, placement offsets
								          are in ot_DynamSeparatePlacement. Loop through EDTSeparates
								          stored under this measure, and match inst w/instno, and
								          IndivPos in EDTScoreExpression w/measure in EDTSeparates*/

#define SCOREEXP_VECTORSTART	0x0400   /* if set, start playback at position in measure,
											else start at beginning */
#define SCOREEXP_LISTNUM		0x03FF   /* If LISTINST is set: the instrument number the
								  			 shape is attached to. (Smart shapes or other
								   			expressions with "This Staff Only" selected).
								   			If LISTINST is not set: cmper for a staff list
								   			(which is a set of ot_STAFFLIST, ot_DC, ot_dc,
								   			ot_IO, and ot_io records all with the same
								  			 cmper) or 0 if no staff list */
/*------------------------------*/

} EDTScoreExpression; /* ~ DYNAMIC, ot_DYNAMIC, OTHERDEC.H */

/*-----------------------------------------------------------------*/

#define ot_ScoreExpression	MAKEEXTAG(edOther,'D','Y')

/* cmper = measure (1-based), incident =  which score expression for this measure (0-based) */

/*##############################################################################*/

/* EDTSeparatePlacement
**
** This record specifies separate placement info for
** a score expression or repeat (gives extra offset for a particular
** instrument.  Each instrument that has separate placement
** will have one of these records.
*/

/*-----------------------------------------------------------------*/

typedef struct
{
	twobyte instno;  /* info is for this instrument */
	twobyte measure; /* needs to match IndivPos when used w/score expression,
						needs to match measure when used w/repeats,
						(so we know which record this is for) */
	twobyte x1add;   /* extra offset from location in score expression record, */
	twobyte y1add;	 /*  or repeat bracket top left, or repeat text */
	twobyte x2add;   /* extra offset from repeat bracket bottom right */
	twobyte y2add;   /* (not used w/score expressions, repeat text) */

} EDTSeparatePlacement;		/* ~ SEPERATES, ot_DI, ot_BI, ot_EI,OTHERDEC.H */

/*-----------------------------------------------------------------*/

#define ot_SeparateScoreExpressionPlacement		MAKEEXTAG(edOther,'D','I')
#define ot_SeparateBackwardRepeatPlacement		MAKEEXTAG(edOther,'B','I')
#define ot_SeparateEndingRepeatPlacement		MAKEEXTAG(edOther,'E','I')
#define ot_SeparateEndingRepeatTextPlacement	MAKEEXTAG(edOther,'L','I')

/* cmper == measure number of score expression/repeat in staff/score (1-based)
 incident == which separate placement info for this measure (0-based)
  */


/*##############################################################################*/

/* EDTPlayDump
**
** This record holds arbitrary midi data for a staff/score expression
** with a playback type of Midi Dump (see EDTTextExpression)
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte dataitems;
	twobyte data[5];  /* storage for bytes to dump.
						(dumps data[0 to 5] hi order byte,
						then lo-order */

} EDTPlayDump; /* ~ PLAYDUMP, ot_PLAYDUMP_TEXT, OTHERDEC.H */

/*-----------------------------------------------------------*/

#define ot_PlayDump		MAKEEXTAG(edOther,'P','D')

/* cmper == value in text/shape expression (1-based), inci = 0 */

/*##############################################################################*/

/* EDTTextExpression
**
** This record specifies text expression related info
** and hangs off of a staff or score expressions.
** Info is normally set in the text expression designer dialog:
**
** text for the text expression	is variable length string
** at the end of the struct (Use LoadEData w/NULL buffer to
** get length of struct+string)
**
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte sizefont;  /* msb: font size in points (unsigned); lsb: font id (unsigned) */
	FLAG_16 efx;       /* any of FONT_EFX_* */
	twobyte value;     /* executable shape id, or constant value (for playback) */
	twobyte auxdata1;  /* used only if USEauxDATA in flags is set, meaning depends on playback type */
	twobyte playPass;  /* play only on pass playPass (if != 0) */
	FLAG_16 flag;

/*---------- flag bits: --------*/

/* ~ POUNDaux1..DYTYPEBITS, NEWENCLOSURE, OTHERDEC.H */
// 9/21/98 RES: renamed POUNDaux* --> POUND_REPLACE_*
#define	TEXT_EXP_POUND_REPLACE_BITS		0xC000
#define	TEXT_EXP_POUND_REPLACE_NONE		0xC000	// no pound sign replacement
#define	TEXT_EXP_POUND_REPLACE_VALUE	0x0000	// replace pound sign w/set-to-value number
#define	TEXT_EXP_POUND_REPLACE_CTRLR	0x8000	// replace pound sign in text w/controller
#define	TEXT_EXP_POUND_REPLACE_PASS		0x4000	// replace pound sign in text w/pass number
//#define TEXT_EXP_POUNDaux1		0x8000   /* replace pound sign in text w/controller */
//#define TEXT_EXP_POUNDaux2		0x4000   /* replace pound sign in text w/pass number */
//								         /* neither == replace pound sign w/set-to-value number */
#define TEXT_EXP_NEWENCLOSURE	0x0800   /* enclose text expression using info in ot_TextExpEnclosure,
											(same cmper as this ot_TextExpression) */
#define	TEXT_EXP_NOPRINT		0x0200	 /* if set, expression does not print (screen only) */

/* flags below are also used in shape expressions: */

#define EXP_VALUEisEXEC		0x2000   /* value is an executable shape id, else it's a constant */
#define EXP_USEauxDATA		0x1000	 /* playback type needs auxdata1 to fully specify its parameters */
#define EXP_DYTYPEBITS		0x00FF	 /* playback type: */

/* ~ TIME..STOPTDIAL, PLAYDECS.H */

#define 	EXP_TIME			1		/* Tempo, value == pulses/minute, auxdata1 = EDUs in pulse (EXP_USEauxDATA) */
#define 	EXP_AMPLITUDE		2		/* Key Velocity, value = velocity */
#define		EXP_TRANSPOSE		3		/* Note Transposition, value = 1/2 steps to transpose */
#define 	EXP_DUMP			4		/* Packet Dump to Midi, value == id of ot_PlayDump */
#define 	EXP_CHANNEL			5		/* Output Route to Midi Channel Mapping, value== channel to switch to. */
#define 	EXP_REKEY			6		/* Restrike any held notes, value == executable shape. */
#define 	EXP_STARTTDIAL		7		/* Start recognizing Tempo tool tempo maps */
#define 	EXP_STOPTDIAL		8		/* Stop recognizing Tempo tool tempo maps */

/* ~ MIDI_CONTROLLER..MIDI_PATCH_CHANGE, MIDIDECS.H */

#define		EXP_MIDI_CONTROLLER		(MIDIBYTE)0xB0 /* Controller, value == controller value, auxdata1 = midi controller (EXP_USEauxDATA) */
#define		EXP_MIDI_PATCH_CHANGE	(MIDIBYTE)0xC0 /* patch change, auxdata1 = patch # (EXP_USEauxDATA) */

/* value, msb: any of EXP_PATCH_* #defines below. Determines how the patch is done (how auxdata1 is interpreted),
   value, lsb: the patch number

   The bank switch values are in auxdata1, one both or none of these values are used depending on program change type:
   auxdata1,msb: controller 0 value for bank switch.
   auxdata1,lsb: controller 32 value for bank switch (except for PATCH_PROG_CHANGE_X2,
    	in which case it is the 2nd patch number)

   If a byte is not used by the program change type, it should be set to 0.
*/

/* ~ PATCH_XX_XXX_PC..PATCH_XX_PC__PC, MPATCH.H */

#define			EXP_PATCH_PROG_CHANGE 0			/* standard patch change, auxdata1 == 0 */
#define			EXP_PATCH_C0_C32_PROG_CHANGE 1	/* bank sent as controller 0,32; then program change (lsb */
#define			EXP_PATCH_C0_PROG_CHANGE 2		/* bank sent as controller 0; then program change */
#define			EXP_PATCH_C32_PROG_CHANGE 3		/* bank sent as controller 32; then program change */
#define			EXP_PATCH_PROG_CHANGE_X2 4		/* 2 program changes. */

/* ~ MIDI_PRESSURE..MIDI_PITCHWHEEL, MIDIDECS.H */

#define		EXP_MIDI_PRESSURE		(MIDIBYTE)0xD0 /* channel pressure/aftertouch, value == channel pressure 0-127 */
#define		EXP_MIDI_PITCHWHEEL		(MIDIBYTE)0xE0 /* pitchwheel, value == (0-16384,8192==rest) */

	char text[2]; /* variable array of null-terminated text */

} EDTTextExpression; /* ~ DTXTSPEC, ot_DTXTSPEC, OTHERDEC.H */

/*-----------------------------------------------------------*/

#define ot_TextExpression		MAKEEXTAG(edSpecialOther,'D','T')

/* cmper == dynumber in staff/score expression, 1-based, inci = 0 */

/*##############################################################################*/

/*
** EDTShapeExpression
**
** This record specifies shape expression-related info
** and hangs off staff and score expressions.
** Info is normally set in the shape expression designer dialog:
*/

/*-----------------------------------------------------------*/

typedef struct
{
	CMPER shapedef;			/* shape id */
	twobyte AAAA;			/* unused */
	twobyte value;			/* executable shape id, or constant value */
	twobyte auxdata1;       /* used only if USEauxDATA in flags is set, meaning depends on playback type */
	twobyte playPass;		/* play only on pass playPass (if != 0) */
	FLAG_16 flag;

/*---------- flag bits: --------*/

/* ~ MASTERSHAPE..LOCKDOWN, OTHERDEC.H */

#define SHAPE_EXP_MASTERSHAPE		0x0800      /* This shape expression is originally created,
										and not a duplicate, display this shape in the dialog.
										When in doubt leave this bit alone, or set for brand
										new shape expressions. */

#define SHAPE_EXP_objBRACKET	0x0200  /* system use only */
#define SHAPE_EXP_LOCKDOWN		0x0100  /* don't allow horizontal stretching (inverse of the checkbox) */
#define	SHAPE_EXP_NOPRINT		0x0400	/* if set, expression does not print (screen only) */

/* EXP_* flags for text expression are also valid here */

/*------------------------------*/

} EDTShapeExpression; /* ~ DOBJSPEC, ot_DOBJSPEC, OTHERDEC.H */

/*-----------------------------------------------------------*/

#define ot_ShapeExpression		MAKEEXTAG(edOther,'D','O')

/* cmper == dynumber in staff/score expression (1-based), inci = 0 */

/*##############################################################################*/

/* EDTShape
**
** This record specifies a shape definition (used in shape expression,
** executable shapes, etc.) related info see shapetag.h for instruction
** types.
** Info is normally constructed in the shape designer dialog:
*/

/*-----------------------------------------------------------*/

typedef struct
{
	float x;
	float y;
} EDTFPPoint; /* FPOINT, PATHPROT.H */

/*-----------------------------------------------------------*/

typedef struct
{
	tbool 		isFloat;			/* if YES, PATHDATA elements are floats
									where appropriate (some things like
									font/size/efx, character, etc. are
									never float even if isFloat is YES.) */
	EDTFPPoint	curpnt;				/* always float, regardless of isFloat,
									in absolute coords */
	EDTFPPoint	startPoint;			/* the absolute starting point for the
									entire path. Set by pathcompiler().
									Always float, regardless of isFloat. */
	EDTFPPoint	startObject;		/* the absolute starting point for the
									current object. Always float, regardless
									of isFloat. */
	ERECT		boundRect;			/* the bounding rectangle for the path
									(accumulated during pathcompiler()),
									in absolute coordinates */
	fourbyte 	horzPerc;			/* records horz scaling in effect when
									boundRect accumulated (percent * 100) */
	fourbyte 	vertPerc;			/* records vert scaling in effect when
									boundRect accumulated (percent * 100) */

	fourbyte	instrucOff;			/* byte offset for array of EDTPathInst for shape
									  (from start of struct) */
	fourbyte	dataOff;			/* byte offset for array of EDTPathData for shape
									   (from start of struct)  This is the required data
									   for the instructions in the EDTPathInst array */
	fourbyte	instrucLen;			/* number of EDTPathInst elements */
	fourbyte	dataLen;			/* number of EDTPathData elements */

	/* shape instructions and data follow (variable length)
		use EDTPathInst *instPtr = (EDTPathInst *)((char *)exShapePtr + instrucOff)
		or EDTPathData *pathPtr = (EDTPathData *)((char *)exShapePtr + dataOff) */

} EDTShape; /* ~ SHAPEDEF, ot_SHAPEDEF; SHAPELIST, ot_SL; SHAPEDATA, ot_SB; OTHERDEC.H */

/*------------------------------------------------------------------------*/

#define ot_Shape		MAKEEXTAG(edSpecialOther,'S','D')

/* cmper == shapedef in shape expression (1-based), inci = 0 */

/*------------------------------------------------------------------------*/

typedef union  /* data for shape, specify either f or l depending on isFloat,
					(some things are always fourbyte, like font ids) */
{
	float f;
	fourbyte l;

} EDTPathData; /* PATHDATA, PATHPROT.H */

/*------------------------------------------------------------------------*/

typedef struct 					/* Path Instruction */
{
	ETAG tag;					/* any of st_* shape tags in shapetag.h */
	uonebyte revnum;			/* revision number. set to 2 */
	uonebyte numdata;			/* number of data items for this instruction */
	twobyte dataOff;			/* this instruction's data offset from beginning of shape's EXPATHDATA array */
	FLAG_16 flag;				/* (system use only, used by the shape designer while editing) */

} EDTPathInst;  /* PATHINST, PATHPROT.H */

/* flags for text expression are also valid here */

/*##############################################################################*/

/* EDTTextBlock
**
** This record specifies layout info for a text block, it hangs
** off a staff or group record (for staff group names) or a
** Page or Measure Text Block.
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte textID;			/* ID for raw text block data (rawTextNumber for tx_TextBlock, 1-based)
							   See Raw Text Documentation in ETFSPEC.TXT) */
							/* Next two are Used when there is no layout shape, 0 means unbounded in that direction */
	twobyte width;			/* Width of text block in evpus */
	twobyte height;			/* Height of text block in evpus */
	twobyte shapeID;		/* shapeid of layout shape (0 if there is none) */
	twobyte lineSpacing;	/* line spacing, if TEXTBLOCK_LINESPACINGISPERCENT set, it is a percent of the font size */
	twobyte xadd;			/* EVPU offset of text from origin (used with custom frames) */
	twobyte yadd;			/* EVPU offset of text from origin (used with custom frames) */
	FLAG_16 flag;

/*---------- flag bits: --------*/

/* ~ TB_JUSTIBITS..LINESPACINGISPERCENT, TEXTBLOK.C */

#define TEXTBLOCK_JUSTIBITS 0x0007	/* Justification type. any of TEXT_JUSTIFY_* #defines */

#define TEXTBLOCK_NEWPOS 0x0008	/* NO: positioning modes in PTXTTIE, MTXTTIE are applied to the
									origin of the layout (if any), the upper left corner of the text
									if no layout.
								   YES: positioning modes above are applied to the bounding rectangle of
									the object, with or without layout. This flag also governs how
									the default font is used to compute the baseline. It will ordinarily
									be set for 3.7 text blocks and above. */

#define TEXTBLOCK_USERIGHTPOS			0x0010	/* use the right page offsets for right (odd) pages */
#define TEXTBLOCK_SHOWMAINSHAPE			0x0200	/* Draw the main shape. (shapeID) */
#define TEXTBLOCK_NOEXPANDSINGLEWORD	0x0400	/* "Expand Single Word" effects the behaviour of the
												full justification modes, JUSTIFY_FULL and JUSTIFY_FORCED_FULL */
#define TEXTBLOCK_WORDWRAP				0x0800	/* automatically wrap words */

#define TEXTBLOCK_LINESPACINGISPERCENT  0x1000	/* If set and lineSpacing is not zero,
											   the lineSpacing field is a percentage
											   of the font size. */

/*-------------------------------*/


						  /* inset and stdline are store as: (hi: most sig. word, lo: least sig. word)  */
	twobyte insethi;	  /* EVPUS * 64 (EVPUFixed) for inset of text from frame (shape)*/
	twobyte insetlo;

	twobyte stdlinehi;	  /* EVPUS * 64 (EVPUFixed) for line width of standard frame when it is outlined (stdlinehi<<16 + stdlinelo) */
	twobyte stdlinelo;

	twobyte unused[12];

} EDTTextBlock; /* ~ NEWTEXTBLOCK, TEXTBLOK.H */

/*-----------------------------------------------------------*/

#define ot_TextBlock			MAKEEXTAG(edSpecialOther,'T','X')

/* cmper == text block id (1-based), incident == 0
   text block id's are stored in Staff, Group Records; and Page
   and Measure-Attached Text Records*/

/*##############################################################################*/
/*
** EDTPageText
**
** Attaches a EDTTextBlock to a page or range of pages
*/

/*-----------------------------------------------------------*/

typedef struct
{
	CMPER block;				/* ot_TextBlock cmper */
	twobyte xdisp;              /* EVPU offset from reference point (determined by bits in flag) */
	twobyte ydisp;
	twobyte startPage;			/* first page text appears on */
	twobyte endPage;			/* last page text appears on, 0 means to end of document */
	FLAG_16 flag;

/*---------- flag bits: --------*/
/* ~ PTXT_OE_BITS..PTXT_INDRP_POS, TEXTBLOK.C */

#define PAGETEXT_OE_BITS           0x0003	/* 2 bits, 3 modes */

/* ~ PTXT_ASSIGN_ALL..PTXT_ASSIGN_EVEN, TEXTBLOCK.H */

#define 	PAGETEXT_ASSIGN_ALL 0
#define 	PAGETEXT_ASSIGN_ODD 1
#define 	PAGETEXT_ASSIGN_EVEN 2

#define PAGETEXT_HPOS_LP_BITS      0x000C	/* 2 bits, 3 modes (for left pages or all)) (TEXT_HALIGN_*) */
#define PAGETEXT_HPOS_RP_BITS      0x0030	/* 2 bits, 3 modes (for right pages) (TEXT_HALIGN_*) */
#define PAGETEXT_HPOS_PAGE_EDGE    0x0040	/* if set, horz pos is relative to
											   page edges, else it's relative to margins */
#define PAGETEXT_ANNOTATION        0x0080	/* show on screen only, don't print */
#define PAGETEXT_VPOS_BITS         0x0300	/* 2 bits, 3 modes (for left pages or all)) (TEXT_VALIGN_*) */
#define PAGETEXT_VPOS_PAGE_EDGE    0x0400	/* if set, vert pos is relative to
											   page edges, else it's relative to margins */
#define PAGETEXT_INDRP_POS         0x0800	/* if set, use different pos for right/left pages */

/*-------------------------------*/

	twobyte rightpgxdisp; 		 /* offset from reference point on right pages (determined by bits in flag) */
	twobyte rightpgydisp;

	twobyte unused[4];

} EDTPageText;			/* ~ PTXTTIE, ot_PTXTTIE, TEXTBLOK.H */

/*-----------------------------------------------------------*/

#define ot_PageText		MAKEEXTAG(edOther2Inci,'p','T')

/* cmper == page, for single page text blocks (1-based), == 0 for multi-page text blocks,
incident == which text block on the page (0-based). */

/*##############################################################################*/

/*
** EDTMeasureText attaches a EDTTextBlock to a measure
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte block;				/* ot_TextBlock cmper */
	twobyte xdisp;				/* EVPU offset from measure top, left */
	twobyte ydisp;
	twobyte AAAA;				/* unused */
	FLAG_16 flag;

/*---------- flag bits: --------*/
/* ~ MTXT_POSTIT, TEXTBLOK.C */

#define MEASURETEXT_ANNOTATION 0x0001   /* show on screen only, don't print */
/*-------------------------------*/

} EDTMeasureText; /* ~ MTXTTIE, dt_MTXTTIE; TEXTBLOK.H */

/*-----------------------------------------------------------*/

#define dt_MeasureTextBlock		MAKEEXTAG(edDetail,'m','t')

/* cmper == instrument (1-based), cmper2 == measure (1-based),
  inci == which text block on the measure(0-based) */


/*##############################################################################*/

/* Bits for flag in EDTBackwardRepeat, EDTEndingRepeat, EDTTextRepeat:

 (These are set in the Backward Repeat Assignment, Repeat Assignment, Ending RepeatBar
 Assignment dialogs:)

 REPEAT_isMARKER,REPEAT_JMPtoMARK are only used by EDTTextRepeat (text repeats,
 "Repeat Assignment" dialog)

 REPEAT_MULTACTUATE,REPEAT_JMPIGNORE are not used by EDTBackwardRepeat

 REPEAT_REPuntilACT,REPEAT_JMPALWAYS,REPEAT_JMPonACTU,REPEAT_COUNTPASSES
 REPEAT_CLRonCHANGE are not used by EDTEndingRepeat.
*/

#define REPEAT_isMARKER		0x4000
#define REPEAT_JMPIGNORE	0x2000	/* "Jump if Ignoring Repeats"
										To effectively "ignore" repeats, you sometime
										have to take them!  For example, given a first ending/second ending
										situation, the user expects that ignoring repeats
										will take the second ending and skip the first.
										Our current implementation (3.0), however, takes
										the first ending, then jumps directly to the second!
										The JMPIGNORE flag means "always jump if ignoring
										repeats", and should be all we need to handle this
										situation. */
#define REPEAT_JMPRELATIVE	0x1000	/*"Relative Jump" - if set, target is a relative
									 offset from the current measure rather than
									 an actual measure number (ignored if isMARKER
									 is set  */
									 
// JPL 05/26/99: The repeat action within Finale is now controlled by the combination of
// the two bits in *_ACTION ( 0x0C00 ) via nice access routines. For the plugins we need to
// 'manually' set the bits.

#define REPEAT_JMPALWAYS	0x0000 /* "Always Jump" */
#define REPEAT_JMPonACTU	0x0400 /* "Jump on total passes" */
#define REPEAT_REPuntilACT	0x0800  /* "Repeat until total passes" */


#define REPEAT_JMPtoMARK	0x0200

// REPEAT_COUNTPASSES is no longer used (it is implied by repeat action;
// only REPEAT_JMPALWAYS does not bother to count).
//#define REPEAT_COUNTPASSES	0x0080  /* set if using REPEAT_JMPonACTU or REPEAT_REPuntilACT */
#define REPEAT_CLRonCHANGE	0x0040  /* "Reset on Repeat Action" */
#define REPEAT_MULTACTUATE	0x0008  /* "Multiple..." (repeat has multiple total passes) */
#define REPEAT_INDIVPLAC	0x0001  /* "Individual Positioning"
										Bracket individually placed for each staff.
										See repeat structs to see how indiv positioning
										is stored: */

/*################################################################################*/

/* Used for a "Backward Rrepeat", or "Backward Repeat w/Bracket" */
/* These values are set in the "Backwards Repeat Bar Assignment" Dialog

    if REPEAT_INDIVPLAC set in flag separate placement is fount in
	EDTSeparatePlacement record w/ tag ot_SeparateBackwardRepeatPlacement

	Flag measure's meflag with MEAS_BACREPBAR, to get the repeat to show
	up, and MEAS_BACREPBAR|MEAS_BARENDING to make a bracket on the backward
	repeat.
 */

typedef struct
{
	twobyte current;	/* internal counter, set to zero */
	twobyte actuate;    /* "Total Passes" */
	twobyte target;		/* "Target Measure" */

	/* pos1,line1; pos2,line2 are used when the backward bar
	  is connected to a repeat ending (MEAS_BARENDING set in measure)
	  These values do not include additional global offsets set in the Repeat
	  Endings Dialog. */

	twobyte pos1;	/* EVPU offsets of the right bottom of bracket from measure's right top */
	twobyte line1;
	FLAG_16 flag;	/* see REPEAT_* flags above */

	twobyte AAAA;	/* unused */
	twobyte BBBB;	/* unused */
	twobyte CCCC;	/* unused */
	twobyte pos2;	/* EVPU offsets of the left top of bracket from measure's left top */
	twobyte line2;
	twobyte DDDD;	/* unused */

} EDTBackwardRepeat; /* ~ ot_BACKREPT, BACKREPT, OTHERDEC.H */

#define ot_BackwardRepeat MAKEEXTAG(edOther2Inci,'B','R')

/* cmper == measure (1-based), incident == 0 */

/*################################################################################*/

/* Repeat ending: for a "Multiple Ending Repeat"
	(bracket w/optional text)
	These values are normally set in the "Ending Repeat Assignment" Dialog
	Flag measure's meflag with MEAS_BARENDING, to get the repeat to show up


    if REPEAT_INDIVPLAC set in flag separate placement for the bracket is fount in
	EDTSeparatePlacement record w/ tag ot_SeparateEndingRepeatPlacement,
	and separate placement for the text is found in ot_SeparateEndingRepeatTextPlacement

	The text for the text repeat is stored as a string under the
	ot_EndingRepeatText tag.
*/

typedef struct
{
	/* pos1,line1; pos2,line2 textpos,textline;
		do not include additional global
	   offsets set in the Repeat Endings Dialog. */

	twobyte current; /* internal counter, set to zero */
	twobyte nextend; /* "Target Measure" */
	twobyte textpos; /* horiz. EVPU offset of the repeat text from measure's left */

	twobyte pos;   /* EVPU offsets of the left bottom of bracket from measure's left bottom */
	twobyte line;
	FLAG_16 flag;	/* see REPEAT_* flags above */

	twobyte AAAA;	/* unused */
	twobyte endline; /* unused */
	twobyte textline;  /* vertical EVPU offset of the repeat text from measure's top */
	twobyte pos2;	/* EVPU offsets of the right top of bracket from measure's right top */
	twobyte line2;
	twobyte CCCC;	/* unused */


/* The "Total Passes" field or the Multiple passes entered in the
"Multiple total passes" dialog box are stored as a array of twobytes
(these are passes when the measure should be played)   If
Multiple passes are not checked, this array contains just one value, the
"Total Passes" value in the Ending Repeat dialog is found */

	twobyte numTotalPasses;
	twobyte multiTotalPasses[1]; /* variable length array ~ ot_REPTENUM, REPTENUM, OTHERDEC.H */

} EDTEndingRepeat; /* ~ ot_ENDSTART, ENDSTART, ENDSTART1, OTHERDEC.H */

#define ot_EndingRepeat MAKEEXTAG(edSpecialOther,'E','S')
/* cmper == measure (1-based), incident == 0 */

/* The text for the Ending Repeat is stored as a string in ot_EndingRepeatText: */

#define ot_EndingRepeatText MAKEEXTAG(edOtherString,'E','T')
/* cmper == measure (1-based), incident == 0 */


/*################################################################################*/

/* Text Repeat : for an individually placed Text Repeat

	These values are normally set in the "Repeat Assignment" Dialog
	Flag measure's meflag with MEAS_REPEATS, to get the repeat to show up
*/

typedef struct
{
	twobyte current; /* internal counter, set to zero */
	twobyte actuate; /* "Total Passes" (if "Multiple..." is checked, total passes are in
					     multiTotalPasses[] in EDTTextRepeatStyle) */
	twobyte target;  /* "Target Measure" */
	twobyte repnum;  /* cmper number of EDTTextRepeatStyle for this text repeat */
	twobyte AAAA;	 /* unused */
	FLAG_16 flag; /* see REPEAT_* flags above */

} EDTTextRepeat; /* ~ ot_REPSUSED, REPSUSED, OTHERDEC.H */

//#define ot_TextRepeat MAKEEXTAG(edSpecialOther,'R','U')
#define ot_TextRepeat MAKEEXTAG(edOther,'R','U')
/* cmper == measure (1-based), incident == which text repeat for the measure (can be multiples, 0-based) */

/*################################################################################*/

/*  Text Repeat Style (these are the repeats that go in the scrolling list
	in the "Repeat Selection" Dialog box)

	These values are normally set in the "Repeat Designer" Dialog
	Flag measure's meflag with MEAS_REPEATS, to get the repeat to show up
	(Note that the multiple passes information is edited in the
	"Repeat Assignment" Dialog even though it belongs to this structure.)

	The text for the text repeat is stored as a string under the
	ot_TextRepeatStyleText tag.
*/

typedef struct
{
	twobyte posadd;  /* EVPU offsets from top left of measure */
	twobyte lineadd;
	twobyte font;	/* font id (use FX_FontNumToName) */
	twobyte size;	/* font size in points */
	FLAG_16 efx;	/* font efx (any of FONT_EFX_* #defines */
	FLAG_16 flag;
/* bits for flag: */
#define TEXT_REPEAT_NEWENCLOSURE	0x0800 /* Enclose repeat using info in ot_RepeatEnclosure/w cmper
											  of the EDTTextRepeatStyle */
#define TEXT_REPEAT_POUNDMEAS		0x0080 /* replace '#' in text with measure */
#define TEXT_REPEAT_POUNDMARK		0x0040 /* replace '#' in text with mark target's text */
#define TEXT_REPEAT_USETHISFONT		0x0020 /* "Use this Font" (use struct's font instead of
											  mark's font for drawing mark's text when
											  using TEXT_REPEAT_POUNDMARK ) */

#define TEXT_REPEAT_JUSTIBITS		0x0003  /* justification of text with measure */

#define 	TEXT_REPEAT_LEFTJUST	0x0000
#define		TEXT_REPEAT_RITEJUST	0x0001
#define		TEXT_REPEAT_CENTERED	0x0002
#define		TEXT_REPEAT_FULLJUST	0x0003


	//This info is only relevant if REPEAT_MULTACTUATE is set in the flags of
	//the corresponding EDTTextRepeat record.  If REPEAT_MULTACTUATE is not set,
	//these values will/should be 0, and actuate in the corresponding EDTTextRepeat
	//should be used as the single totalPasses value.

	twobyte numTotalPasses;		 //number of twobytes in following array:
	twobyte multiTotalPasses[1]; //the various multiple passes from the "Repeat Assignment" Dialog
								 //(variable length array)

} EDTTextRepeatStyle; /* ~ ot_REPTSPEC, REPTSPEC, OTHERDEC.H */

#define ot_TextRepeatStyle MAKEEXTAG(edSpecialOther,'R','S')
/* cmper == value stored in EDTTextRepeat.repnum (1-based), incident == 0 */

#define ot_TextRepeatStyleText MAKEEXTAG(edOtherString,'R','T')
/* cmper == value stored in EDTTextRepeat.repnum (1-based), incident == 0 */

/*##############################################################################*/
/* Enclosure definition: Used by Text Repeats, Staff, Score Expressions,
	Measure Numbers */

/* This information, along with a bounding box specifies the parameters
   needed to draw an enclosure  This information is normally edited in
   the "Enclosure Designer" dialog box: */

typedef struct
{
	twobyte        xAdd;	   /*  "Center X,Y" - offset text from center by this much */
	twobyte        yAdd;
	twobyte        xMargin;	/*  "Width" / 2  - extra space on left, right sides */
	twobyte        yMargin;	/* "Height" / 2  -  extra space on top, bottom sides */
	EVPUFixedShort lineWidth;  /* "Line Thickness", in 64ths of an EVPU */
	FLAG_16        flag;
/* bits for flag: */
#define	NEWSIDES		0x000F	/* number of sides
									0 == no enclosure 	(special case)
									1 == rectangle		(special case)
									2 == ellipse		(special case)
									3 == triangle
									4 == diamond
									5 == pentagon
									6 == hexagon
									7 == septagon
									8..15 == polygon */

#define	NOT_TALL		0x1000	/* "Enforce Minimum Width" don't let shape get taller than it is wide */
#define	LWIDTH_IS_SHAPE	0x2000	/* lwidth field is a shape ID */
#define	EQUAL_ASPECT	0x4000	/* "Match Height and Width" keep width and height equal */
#define	FIXED_SIZE		0x0800	/* fixed size (ignore text bounding box) */

} EDTEnclosure;  /* ~ ot_Rx, ot_De, ENCLSPEC, OTHERDEC.H */

#define ot_RepeatEnclosure MAKEEXTAG(edOther,'R','x')
/* cmper == value stored in EDTTextRepeat.repnum (1-based), incident == 0 */

#define ot_TextExpEnclosure MAKEEXTAG(edOther,'D','e')
/* cmper == dynumber in Score or staff expression (same cmper as Text Expression, 1-based), incident == 0 */

/*####################################################################################*/
/* EDTPageSpec

There is exactly one PS record for every page in the piece.

Page specs are stored over 2 incidences.

Most of this information is directly related to fields in the page layout
dialog.
*/

/*-----------------------------------------------------------*/

typedef struct
{
	fourbyte height;		/* page height in EVPUs */
	fourbyte width;			/* page width in EVPUs */
	twobyte stavestr;		/* ID for first staff system on this page */
							/* (EDTStaffSystemSpec); 0 means recompute */
							/* the first system, -1 means leave page blank */
	FLAG_16 pageflag;		/* see bits below */
	twobyte margTop;		/* top margin in EVPUs */
	twobyte margLeft;		/* left margin in EVPUs */
	twobyte margBottom;		/* bottom margin in EVPUs */
	twobyte margRight;		/* right margin in EVPUs */
	twobyte percent;		/* page reduction percentage */
//	twobyte AAAA;			/* (unused) */

/* pageflag bits: */

#define PAGESPEC_OSSIA			0x0001	/* a page based ossia is attached to this page */
#define PAGESPEC_MARGSCOPING   	0x0002	/* page was resized with "hold margins" (contents
										of page are affected by resize, but not the
										physical page size) */

}  EDTPageSpec; /* ~ PAGESPEC, ot_PAGESPEC, PAGESPEC.H */

#define ot_PageSpec MAKEEXTAG(edSpecialOther,'P','S')
/* cmper=page number (1-based), inci = 0 */

/*##############################################################################*/
/* EDTStaffSystemSpec

There is exactly one SSPEC record for every staff system in the piece.

Staff system specs are stored over two incidences.

Most of this information is directly related to fields in the page layout
dialog.
*/

/*-----------------------------------------------------------*/

typedef struct
{
	// staff system margins:
	twobyte top;
	twobyte left;
	twobyte right;
	twobyte bottom;

	twobyte mestart;		// first measure on this system
	twobyte mend;			// first measure on next system; if 0, recompute
							// the number of measures on this system
	fourbyte horzPercent;	// horizontal stretch percentage (hundredths of a percent)
	twobyte ssysPercent;	// percent reduction for this system
	FLAG_16 staveflag;		// see bits defined below

// bits for staveflag:

#define SSPEC_LINEBREAK		0x8000	// force this system to start a page
									// (implemented in Enigma but not hooked up)
#define SSPEC_INSTLIST		0x4000	// staff system has its own IU list (the
									// system is "optimized")
#define SSPEC_NONAMES		0x2000	// suppresses drawing of staff names (hooked
									// up in Enigma but not accessible)

#define SSPEC_LINEPERC		0x0400	// at least one of the staves in the staff
									// system has an enlargment/reduction
									// ("enduction"). See EDTStaffEnduction.
#define SSPEC_DOSPLITFREEZE	0x0004	// indicates the existence of measure split
									// information (not documented at this time)
#define SSPEC_RESIZE_VERT	0x0002	// if set, space between systems will be
									// reduced as well (aka SS_SPACESCOPING)
#define SSPEC_HOLD_MARGINS	0x0001	// if set, the horizontal extent of the
									// system will not be reduced (aka SS_MARGSCOPING)

}  EDTStaffSystemSpec; /* ~ SSPEC, ot_SSPEC, SSPEC.H */

#define ot_StaffSystemSpec MAKEEXTAG(edSpecialOther,'S','S')
/* cmper=system number (1-based), inci = 0 */

/*##############################################################################*/
/** EDTStaffEnduction contains information about an individual staff enlargement
** or reduction ("enduction") within a specified staff system.
*/

/*-----------------------------------------------------------*/

typedef struct
{
	twobyte staffPercent;
	twobyte AAAA;				/* unused */
	twobyte BBBB;				/* unused */
	twobyte CCCC;				/* unused */
	twobyte DDDD;				/* unused */
} EDTStaffEnduction; /* ~ DTPERCENT, dt_DTPERCENT; DTAILDEC.H */

/*-----------------------------------------------------------*/

#define dt_StaffEnduction		MAKEEXTAG(edDetail,'L','P')
/* cmper1 == staff system (1-based), cmper2 == inst (1-based), inci == 0 */

/*##############################################################################*/
/* EDTMeasNumberRegion - specifies  info for a measure number region.
 Normally edited, created with Measure Number dialog.
*/

/* ~ MAX_MNUMB_PREFIX,MAX_MNUMB_SUFFIX, MEASNUMB.H */

#define	MEASNUM_MAX_PREFIX	24
#define	MEASNUM_MAX_SUFFIX	24

typedef struct
{
	//font of measure numbers. ("Set Font...")

	twobyte font; //Enigma Font ID (FX_FontNumToName) */
	twobyte size; //Font size in points
	FLAG_16 efx;  //Font efx (FONT_EFX_*)

	twobyte startMeas;		//"Includes Measure"
	twobyte endMeas;		//"Through"  - 1 (end of region, not inclusive),
	twobyte incidence;		//"Show Every ___ Measures"
	twobyte startchar;		//"Starting Character..." in "Measure Numbering Style"  dlg
	twobyte base;			//"Numbering Base" in  "Measure Numbering Style"  dlg
							//(e.g. 10=numbers, 26=letters)
	twobyte offset;			//"First Measure in Region" - 1
	twobyte xdisp;			//"Postion..." (Distance between measnum & top left of measure)
	twobyte ydisp;
	ECHAR prefix[ MEASNUM_MAX_PREFIX ]; //"Prefix"
	ECHAR suffix[ MEASNUM_MAX_SUFFIX ]; //"Suffix"
	EDTEnclosure defEnclosure;			// "Edit Enclosure..." - default enclosure for measure numbers
	FLAG_16 flag;
	// bits for flag field (~ STARTOFLINE..SHOWONBOTTOM, MEASNUMB.H):
		#define MEASNUMB_STARTOFLINE 	0x8000  //"Show measures at start of staff system"
		#define MEASNUMB_MULTIPLEOF		0x4000  //"Show every __ measures starting with measure __"
		#define MEASNUMB_AUTOENCLS		0x2000  //"Show Enclosure on Every Number"
												//(!"Show Enclosures on Selected Numbers")
		#define	MEASNUMB_EXCEPTFIRSTMEAS 0x1000	//"Hide First Measure Number in Region"

		//These represent the check boxes in the "Measure Numbering Style" dialog
		#define MEASNUMB_CNTFROMONE		0x0080  //"Count from One"
		#define MEASNUMB_NOZERO			0x0040  //"No Zeros"
		#define MEASNUMB_DOUBLEUP		0x0020  //"Double Up"

		#define MEASNUMB_SHOWONTOP		0x0008	//"Always show on Top Staff"
		#define MEASNUMB_SHOWONBOTTOM	0x0004	//"Always show on Bottom Staff"



	twobyte startWith;		// "Beginning with Measure ____" - 1
							// To get 5, 10, 15... for example, set startWith to 4.
							// (MEASNUMB_MULTIPLEOF needs to be set)
	twobyte region;			// a unique 1-based identifier independent
							// of the comparator that the EDTMeasNumberRegion
							// is stored under; region is used to link to
							// EDTMeasNumSeparate record.  The important thing
							// is that the cmper for EDTMeasNumberRegion can change
							// through sorting, editing, etc., but region will
							// not change, and therefore changing the cmper does
							// not have to affect EDTMeasNumberSeparate et al.
							// (Set by Finale upon creation, do not modify)

}  EDTMeasNumberRegion; /* ~ MEASNUMB, ot_MEASNUMB, MEASNUMB.H */


//Standard numbering schemes and corresponding base, startchar and flag:

//							base	startchar	CNTFROMONE	NOZERO	DOUBLEUP
//							-------------------------------------------------
//"1, 2, 3, 4"				10		'0'			YES			NO		NO
//"a, b, c...aa, bb, cc"	26		'a'			NO			NO		YES
//"A, B, C...AA, BB, CC"	26		'A'			NO			NO		YES
//"a, b, c...aa, ab, ac"	26		'a'			NO			YES		NO
//"A, B, C...AA, AB, AC"	26		'A'			NO			YES		NO

#define ot_MeasNumberRegion MAKEEXTAG(edSpecialOther,'M','N')

/* cmper=which measure number region (1-based), inci = 0 */

/*##############################################################################*/
/* EDTMeasNumberSeparate - specifies staff-specific placement
  for measure number.  EDTMeasNumberSeparate belong to the
  EDTMeasureRegion record with matching region fields...
*/


typedef struct
{
	twobyte region;		// Measure number region (1 based); matches region
						// field in associated EDTMeasNumberRegion record
	twobyte x1add;		// Additional offset of measure number
	twobyte y1add;
	twobyte x2add;		// may not be needed
	FLAG_16 flag;
	/* bits for flag ~ FORCEHIDE_BITS..USE_ENCL, MEASNUMB.H */
		#define MEASNUM_HIDE_MNUM		0x0001 /* override region settings and hide measure number */
		#define MEASNUM_FORCE_MNUM		0x0002 /* override region settings and show measure number */
		#define MEASNUM_USE_ENCL		0x0004 /* enclose measure numbers w/ encl */


	EDTEnclosure encl;	// Enclosure used if MEASNUM_USE_ENCL is set

	twobyte AAAA;		// unused (padded it out to three detail incidences)
	twobyte BBBB;
	twobyte CCCC;
	twobyte DDDD;
}  EDTMeasNumberSeparate; /* ~ MNSEPARATE, dt_MNSEPARATE, MEASNUMB.H */

#define dt_MeasNumberSeparate MAKEEXTAG(edDetail3Inci,'M','I')
/* cmper1=inst (1-based), cmper2=meas (1-based), inci = 0 */

/*##############################################################################*/
/* EDTArticulationDefinition
**
** Definition for an Articulation type (this record is referenced by
** an EDTArticulation entry detail's imrkdef field).  These values
** are normally edited in the "Articulation Designer" dialog box.
*/

typedef struct
{
	FLAG_16 efxsymbolMain;	//"Main Symbol": efx in msb (FONT_EFX_*), char in lsb
	twobyte sizefontMain;	//"Main Symbol": size in msb, enigma font id in lsb
	twobyte AAAA;		// unused
	twobyte BBBB;		// unused
	twobyte hhock;		//system use only (set to zero in new records)
	FLAG_16 flag;
/* bits for flag: ~ HORZITERANT..IMRKDEF_NOPRINT, OTHERDEC.H */

#define ARTIC_HORZITERANT 	0x0020 //1 == "Copy Main Symbol Horizontally" 0 == "Vertically"
									//(also need ARTIC_IMRKITERANT)
#define ARTIC_IMRKITERANT 	0x0040 //"Copy the Main Symbol"
#define ARTIC_USETOPNOTE	0x0080 //"Attach to top note"
#define	ARTIC_AUTOHORZ		0x0010 //"Center Horizontally"
#define	ARTIC_AUTOVERT		0x0008 //0 == "Position Manually" 1== use ARTIC_AUTOVRTMODE
#define	ARTIC_AUTOVRTMODE	0x0007 //Vertical autoposition mode: ("Position" popup)
#define	ARTIC_NOTESIDE			 0 //"Position On note side"
#define	ARTIC_STEMSIDE			 1 //"Position On stem side"
#define	ARTIC_ABOVEENTRY		 2 //"Position above entry"
#define	ARTIC_BELOWENTRY		 3 //"Position below entry"
#define	ARTIC_OUTSIDESTAFF	0x1000	//"Always Place Outside Staff"
#define	ARTIC_ABOVE_SYMBOL	0x2000	/*"When placed above a note":
								   0 == use efxsymbol, 1 == use efxsymbol2 */
#define	ARTIC_BELOW_SYMBOL	0x4000	/* "When placed below a note":
								   0 == use efxsymbol, 1 == use efxsymbol2 */
//#define ARTIC_NOPRINT		0x0100 //don't print (not officially supported)



	FLAG_16 efxsymbolFlipped;	//"Flipped Symbol": efx in msb (FONT_EFX_*), char in lsb
	twobyte sizefontFlipped;	//"Flipped Symbol": size in msb, enigma font id in lsb
	twobyte Xoffset;		    //"Handle Positioning" for "Main Symbol"
	twobyte Yoffset;
	twobyte defVertPos;		//"Default Vertical Position"
	FLAG_16 flag2;
/* bits for flag2: ~ AVOID_STAFF_LINES..IMRK_PLAY, OTHERDEC.H */

#define	ARTIC_AVOID_STAFF_LINES	0x0001 //"Avoid Staff Lines"

#define	ARTIC_MAIN_IS_SHAPE		0x0002	 //1==Main symbol is shape (use mainShape);
										 //0== Main symbol is a character (use efxSymbolMain)
#define	ARTIC_FLIPPED_IS_SHAPE	0x0004	 //1==Flipped symbol is shape (use flipShape);
										 // 0== Flipped symbol is a character (use efxSymbolFlipped)
#define	ARTIC_AMP_IS_PERCENT	0x0008	 //"Values are percentages" ("Playback Effect" is "Change Key Velocity")
#define	ARTIC_START_IS_PERCENT	0x0010	 //"Values are percentages" ("Playback Effect" is "Change Attack")
#define	ARTIC_DUR_IS_PERCENT	0x0020	 //"Values are percentages" ("Playback Effect" is "Change Duration")
#define	ARTIC_PLAY			0x0040		//0=="Playback Effect" is "None" 1==one of
										//("Change Key Velocity", "Change Attack","Change Duration")

	twobyte CCCC;				//unused
	twobyte DDDD;				//unused
	twobyte X2offset;			//"Handle Positioning" for "Flipped Symbol"
	twobyte Y2offset;
	twobyte mainShape;			//shape id of main symbol if ARTIC_MAIN_IS_SHAPE
	twobyte flipShape;			//shape id of flipped symbol if ARTIC_FLIPPED_IS_SHAPE

//Playback effect is defined by either setting a non-zero value in one
//of the TopNote/BotNote pairs below, or by setting an ARTIC_*_IS_PERCENT
//flag (the corresponding pair of values is then considered a percent).
	twobyte startTopNote;	//"Top Note Value" when "Playback Effect" is "Change Attack"
	twobyte startBotNote;	//"Bottom Note Value" when "Playback Effect" is "Change Attack"
	twobyte durTopNote;		//"Top Note Value" when "Playback Effect" is "Change Duration"
	twobyte durBotNote;		//"Bottom Note Value" when "Playback Effect" is "Change Attack"
	twobyte ampTopNote;		//"Top Note Value" when "Playback Effect" is "Change Key Velocity"
	twobyte ampBotNote;		//"Bottom Note Value" when "Playback Effect" is "Change Key Velocity"

} EDTArticulationDefinition;  /* ~ IMRKDEF,IMRKDEF2,IMRKDEF3,IMRKDEF4, OTHERDEC.H */

#define ot_ArticulationDefinition MAKEEXTAG(edOther4Inci,'I','X')
/* cmper1=which articulation (1-based), inci = 0 */

/*##############################################################################*/
/* EDTMidiExpression - specifies measure specific midi expression
  (continous data captured in Midi transcription or hyperscribe,
  or non-entry specific data created/edited in Midi Tool)
*/

typedef struct
{
	TIME_EDU eldur; //time offset from beginning of measure.
	twobyte status; //expression type:

/* ~ MIDI_CONTROLLER..MIDI_PITCHWHEEL, MIDIDECS.H */

#define MIDIEXP_CONTROLLER		(MIDIBYTE)0xB0
#define MIDIEXP_PATCH_CHANGE	(MIDIBYTE)0xC0
#define MIDIEXP_PRESSURE		(MIDIBYTE)0xD0		// channel pressure/aftertouch
#define MIDIEXP_PITCHWHEEL		(MIDIBYTE)0xE0

	twobyte data1; //expression data
	twobyte data2;

//MIDIEXP_CONTROLLER: data1 = controller number, data2 = controller value
//MIDIEXP_PATCH_CHANGE: data1 = patch and PatchType, data2 = bank Data. (same as EXP_MIDI_PATCH_CHANGE's value, auxdata)
//MIDIEXP_PRESSURE: data1 = pressure value, data2 = 0
//MIDIEXP_PITCHWHEEL: data1 = pitch wheel value lsb, data2 = pitch wheel value msb.

}  EDTMidiExpression; /* ~ MIDIEXPRS, dt_MIDIEXPRS, DTAILDEC.H */

#define dt_MidiExpression MAKEEXTAG(edDetail,'M','E')
/* cmper1=inst (1-based), cmper2=meas (1-based), inci = which midi expression in the measure
   (should be in chronological order) */

/*##############################################################################*/
/* EDTChordSuffix - specifies the suffix letters in a chord symbol
   (normally edited in the chord suffix editor.  These 'others'
   are attached to EDTChord and EDTLearnedChord records

   A complete suffix is defined by a sequence of EDTChordSuffix
   (ordered by incident) under the same cmper.
*/

typedef struct
{
	twobyte symbol; //suffix character
	twobyte xdisp;	//horizontal offset from base symbol
	twobyte ydisp;	//horizontal offset from base symbol
	twobyte sizefont; //lsb: enigma font id, msb: size in points
	FLAG_16 efx; //any of FONT_EFX_* #defines
	FLAG_16 flag;
//bits for flag: (isNUMBER..anyCHpres, OTHERDEC.H */
#define CHRDSUFX_isNUMBER 	0x0800 //"Number"
#define CHRDSUFX_preFLAT 	0x0080 //"Prefix with Flat"
#define CHRDSUFX_preSHARP 	0x0040 //"Prefix with Sharp"
#define CHRDSUFX_prePLUS 	0x0020 //"Prefix with Plus"
#define CHRDSUFX_preMINUS 	0x0010 //"Prefix with Minus"

}  EDTChordSuffix; /* ~ CHSUFFIX, ot_CHSUFFIX, OTHERDEC.H */

#define ot_ChordSuffix MAKEEXTAG(edSpecialOther,'I','V')
/* cmper1=suffix id (1-based), inci = which letter in the suffix sequence (0-based)*/

/*##############################################################################*/
/* EDTChordPlayback - specifies the playback for a chord suffix.
   stored as an array of twobytes, one twobyte per midi note in chord.

   cmper matches cmper of EDTChordSuffix.
 */

typedef struct
{
	twobyte keys[6]; //variable length array of twobytes, each twobyte is a
					 //midi note in chord,
}  EDTChordPlayback; /* ~ CHSUFFIXPLAY, ot_CHSUFFIXPLAY, OTHERDEC.H */

#define ot_ChordPlayback MAKEEXTAG(edOtherNullTermArray,'I','K')
/* cmper1=suffix id (1-based), inci = 0 */

//##############################################################################*

// EDTClefDef specifies a clef

typedef struct
{
	twobyte adjust;			// The number of harmonic levels from the top line of
							// the staff to middle C. Negative numbers place middle
							// C below the top line of the staff.

	twobyte clefchar;		// The clef character

	twobyte clefydisp;		// The number of harmonic levels from the top line of the
							// staff to the baseline of the clef. The baseline of the 
							// clef is the line it is pinned to through enlargments 
							// and reductions. ( For the treble clef it is the G line; 
							// for the bass clef, the F line, and so on. ) Negative 
							// numbers place the baseline below the top line of the 
							// staff.
							
	twobyte altcyadjust;	// The difference between the musical baseline of the clef 
							// (such as the G line for treble clef) and the typographic
							// baseline, in harmonic levels. Petrucci and Maestro do
							// not need this; Sonata does.  (only used if useCYADJ is YES)
							
} EDTClefDef; // ~ ClefChart[], PACKGLOB.C

#define gb_ClefDef		MAKEEXTAG(edGlobals,'c','f')
// globalsNumber value ( in EGlobasID ) is the clef index ( 0..7 )

//##############################################################################*

#endif	/*	_EDATA_H_ */

