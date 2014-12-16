/*
File Name:	FINEXTND.H

Copyright 1996 Coda Music Technology (all rights reserved)

Author:  	Randy Stokes
Created:	01/26/96

Purpose:
	This is the include file used to build extensions.

Modification History:

**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  01/06/96  TNW  changed HWND to EWND, removed semi-colon
**			at the extern "C" end brace.
**  01/06/96  TNW  changed MAKELONG to MAKE_FOURBYTE
**  01/09/96  TNW  moved file to INC:FIN, converted C++ comments. (so straight
**			C users can use it)
**  01/24/97 TNW   added rebeam, rebar and database API's.
**  02/07/97 TNW   include edata.h, moved edata structs to edata.h.
**  02/10/97 TNW   more detailed comments on RebeamRegion, RebarRegion, took out C++
**			style comments.
**  02/13/97 TNW   added noteflag.h
**  02/18/97 TNW   converted structs to typedef's for ANSI C compatibility, make sure
**			pointers are returned in D0 for all API's
**  02/28/97 TNW   new API's: Dialog, EndDialog, Playback, SlotToInst, ScrollTo
**			ListenToMidi, FontNumToName, FontNameToNum, GetCurERegions
**  03/05/97 TNW   moved extension manager functionality to Finale, so I removed
**			the extension manager entry point prototypes
**  03/05/97 TNW   moved Finale API's from ctextend.h to here.
**  03/12/97 TNW   added FX_GetDialogItem,FX_SetDialogItem,FX_RedrawMusic,FX_CopyMidi
**			FX_PasteMidi,FX_NOTEToMidi.  Added instructions for adding new API's here.
**  03/17/97 TNW   hooked up FX_FontNameToNum.
**  03/21/97 TNW   added more comments
**  03/31/97 TNW   changed size param in EData procs to fourbyte.
**  04/03/97 TNW   FX_Get/SetDialogItem -> FX_Get/SetDlgCntlVal
**  04/15/97 TNW   added FX_NoteMover, FX_InstToSlot inst param to FX_ScrollTo, inst, meas
**				transpose params to FX_NoteToMidi
**  04/25/97 TNW   added FX_SetTimer, FX_GetKeySig, FX_SelectNote, changed params of
**				FX_NoteMover, FX_NoteToMidi, FX_Dialog
**  05/28/97 TNW   added FX_SetDlgFocus, FX_EnableDlgItem.
**  06/04/97 TNW   FX_SetTimer msInterval is a ufourbyte
**  06/18/97 TNW   added FX_MidiNoteToString, FX_StringToMidiNote,FF_GetDlgItemUser,
**				FX_SetDlgItemUser, FX_GetSyllable, FX_UpdateLayout, FX_MeasureText,
**				FinaleExtensionGetMenuStatus, FX_ParseCaretCommand
**  06/23/97 TNW  eregion.h -> eregdecs.h, added unitdecs.h
**  07/07/97 TNW  added FX_GetEntryMetrics, FX_GetTGFEntryMetrics
**  07/10/97 TNW  added FX_FontDialog, FX_SelectionDialog, FX_SymbolDialog.
**  07/11/97 TNW  comments on FX_MidiNoteToString
**  08/15/97 TNW  added FX_SyllableInfo
**  08/18/97 TNW  FX_SyllableInfo gets a tbool return type.
**  08/20/97 TNW  added flags param to FinaleExtensionInit.
**  08/25/97 TNW  corrected docu for FX_GetSyllable
**  08/27/97 RES  FX_RedrawMusic(): made pRegion const
**  09/02/97 RES  added FX_GetMeasureDuration(), FX_GetTGFDuration(), FX_ReduceEntries()
**  09/09/97 RES  corrected comments
**  09/10/97 TNW  added FX_StartMassEdit(), FX_EndMassEdit(), FX_MassEditProgress()
**				  FX_AlertMessage(), FX_Get/SetDlgItemVal(), FX_Get/SetDlgString()
**  09/11/97 RES  added hiVersion param to FinaleExtensionVersion()
**  09/22/97 TNW  alertStrID in FX_AlertMessage is an ESTRID.
**  09/23/97 TNW  added const to EData Params, removed params for FX_DeleteEData
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  10/13/97 TNW  added CONST to params in FX_FontNumToName,FX_FontNameToNum
**  10/21/97     FINEXTND.H     RES       FX_SetDlgString(): ECCSTR
**  10/21/97     FINEXTND.H     RES       FX_SetDlgCntlVal(): ECCSTR
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  12/04/97     FINEXTND.H     TNW       FX_SyllableInfo returns ending style
**					of lyric in style
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  04/17/98     FINEXTND.H     CFC       added FX_GetShowLayer()
**  04/30/98     FINEXTND.H     RES       removed #ifdef __cplusplus (always C++ now)
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  05/04/98     FINEXTND.H     CFC       experimenting with modified 
**					parameters to FX_SyllableInfo()
**  07/10/98     FINEXTND.H     CFC       checked it in
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: FINEXTND.H,v $
Revision 1.1.1.1  2002/12/19 19:13:12  dfober
first import

** 
** 11    8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 10    7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 9     6/09/99 2:08p Jlenarz
** Added const
** 
** 7     6/09/99 12:31p Jlenarz
** Added tcdTransAdj param to FX_GetKeySig()
** 
** 6     3/29/99 2:32p Rstokes
** changed FINEXPORT back to EXPORT in comments (for historical accuracy
** and tracking)
** 
** 5     3/26/99 4:54 PM Tfischer
** Make plug-ins compilable in Fin2000.  EXPORT->FINEXPORT;  other minor
** related changes
** 
** 4     3/25/99 5:02 PM Tfischer
** Add version parameter to all Finale extension hooks to allow for future
** backward compatiblity with old plugins
** 
** 3     2/04/99 3:52p Beth
** Changed EXPORT to FINEXPORT in anticipation of moving to MFC.
 * 
 * 2     1/28/99 2:43p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

/*--------------------------------------------------------------------------------*/

#ifndef _FINEXTND_H_
#define	_FINEXTND_H_

#ifndef _VERSION_H_
#include "version.h"
#endif

#ifndef _EREGDECS_H_
#include "eregdecs.h"
#endif

#ifndef _UNITDECS_H_
#include "unitdecs.h"
#endif

#ifndef _ELIMITS_H_
#include "elimits.h"
#endif

#ifndef _EDATA_H_
#include "edata.h"
#endif

#ifndef _FXUPPS_H_
#include "fxupps.h"
#endif

#ifndef _FXVERS_H_
#include "fxvers.h"
#endif

/*--------------------------------------------------------------------------------

 The Mixed Mode procInfos here assume Think C-
 style calling conventions.  These conventions are used by
 Metrowerks with the exception of pointer return types, which
 in Metrowerks 68K are returned in A0, instead of the standard
 D0.
--------------------------------------------------------------------------------*/

#ifdef __MWERKS__
#ifndef powerc
#pragma pointers_in_D0
#endif
#endif

/*--------------------------------------------------------------------------------*/

//#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
//#endif

/*--------------------------------------------------------------------------------*/

/* Required entry points for any Finale Extension: */

ELONG FINEXPORT FinaleExtensionVersion( CONST ELONG hiVersion );
tbool FINEXPORT FinaleExtensionInit( CONST EWND hWnd,  FLAG_32 *flags );
tbool FINEXPORT FinaleExtensionEnumerate( CONST EWND hWnd, CONST twobyte index,
	FXT_MENUSTRING_PACKET *buf );
tbool FINEXPORT FinaleExtensionInvoke( CONST EWND hWnd, CONST twobyte index,
	EREGION *pSelRegion, EREGION *pDocRegion );
void FINEXPORT FinaleExtensionTerminate( CONST EWND hWnd );
tbool FINEXPORT FinaleExtensionGetMenuStatus( CONST twobyte index );

/*###################################################################################*/

/* API's in Finale accessible by Extensions: */

/* TGF-oriented: */

PUBLIC EXTGF * FINEXPORT FX_LoadTGF(
	CONST CMPER iuList,
	CONST twobyte staffSlot,
	CONST twobyte meas,
	CONST twobyte layer,
	CONST tbool mirrorOK,
	CONST ELONG version = FXT_VERSION );
PUBLIC tbool FINEXPORT FX_SaveTGF( EXTGF *pTgf, CONST ELONG version  = FXT_VERSION );
PUBLIC EXTGF * FINEXPORT FX_FreeTGF( EXTGF *pTgf, CONST ELONG version  = FXT_VERSION );
PUBLIC tbool FINEXPORT FX_InsertEXENTRY(
	EXTGF *pTgf,
	CONST twobyte offset,
	CONST twobyte numToAdd,
	CONST ELONG version  = FXT_VERSION );
PUBLIC tbool FINEXPORT FX_DeleteEXENTRY(
	EXTGF *pTgf,
	CONST twobyte offset,
	CONST twobyte numToDelete,
	CONST ELONG version  = FXT_VERSION );

/* Gets the duration of the passed EXTGF, accounting for voice 2 */

PUBLIC TIME_EDU FINEXPORT FX_GetTGFDuration( const EXTGF *ptgf, CONST ELONG version  = FXT_VERSION ); // 9/2/97 RES: added --

/* Breaks or combines the entries in ptgf for the current time signature
	for the measure based on the rules of standard notation. */

PUBLIC void FINEXPORT FX_ReduceEntries( EXTGF *ptgf, CONST ELONG version  = FXT_VERSION ); // 9/2/97 RES: added --

/*------------------------------------------------------*/
/* EREGION-oriented:*/

PUBLIC tbool FINEXPORT FX_RebeamRegion(
	CONST EREGION * pRegion,        /*region to rebeam */
	CONST ETimeSig * pExtTimeSig,	/* may be NULLP to beam to existing time sigs */
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_RebarRegion(
	CONST EREGION *pRegion,		/*region to rebar*/
	CONST FLAG_16 stopFlags,    /* see EXTYPES.H REBAR_STOP_* #defines */
	CONST tbool reBeam,         /* rebeam to time sig? */
	CONST tbool padWithRests,	/* pad empty frames with rests */
	CONST tbool checkAccis,		/* defaults to NO (1/7/97 RES)*/
	CONST ELONG version  = FXT_VERSION );

/*------------------------------------------------------*/
/* database-oriented: */

PUBLIC tbool FINEXPORT FX_LoadEData(
	CONST EXTAG extag,		/* type of data*/
	CONST EDataID *id,		/* Identifies data to load*/
	void *data,				/* place to put tag-specific data. If null, just return size.*/
	fourbyte *size,			/* size of data buffer (so if size of info changes in future versions old extensions will not crash)*/
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_SaveEData(
	CONST EXTAG extag,		/* type of data*/
	CONST EDataID *id,		/* Identifies data to save*/
	CONST void *data,		/* tag-specific data to save*/
	CONST fourbyte size,	/* size of data buffer (so if size of info changes in future versions old extensions will not crash)*/
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_CreateEData(
	CONST EXTAG extag,		/* type of data*/
	EDataID *id,			/* Identifies data to create*/
	CONST void *data,		/* tag-specific data to create*/
	CONST fourbyte size,	/* size of data buffer (so if size of info changes in future versions old extensions will not crash)*/
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_DeleteEData(
	CONST EXTAG extag,		/* type of data*/
	CONST EDataID *id,		/* Identifies data to delete*/
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_PreviousEData(
	CONST EXTAG extag,		/* type of data*/
	EDataID *id,			/* get data before this id*/
	void *data,				/* place for tag-specific data to load*/
	CONST fourbyte size,	/* size of data buffer (so if size of info changes in future versions old extensions will not crash)*/
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_NextEData(
	CONST EXTAG extag,		/* type of data*/
	EDataID *id,			/* get data after this id*/
	void *data,				/* place for tag-specific data to save*/
	CONST fourbyte size,	/* size of data buffer (so if size of info changes in future versions old extensions will not crash)*/
	CONST ELONG version  = FXT_VERSION );

/*------------------------------------------------------*/
/* Dialog Related: */

PUBLIC EWND FINEXPORT FX_Dialog(
	twobyte nDlgID,						/* id for dialog template.*/
	FF_DialogHandlerUPP lpCallBack,		/* dialog handler routine */
	EWND hParent,						/* parent dialog */
	twobyte dlgType,					/* DIALOG_MODAL, DIALOG_MODELESS, DIALOG_INVISIBLEMODELESS*/
	void *data,							/* user data (passed back to lpCallBack) */
	CONST ELONG version  = FXT_VERSION );



PUBLIC void FINEXPORT FX_EndDialog( EWND hDlg, CONST ELONG version  = FXT_VERSION ); /*destroy a modeless dialog.*/

/* Get/Set a dialog item (works with checks, radio buttons, edit text,
   combo boxes on windows, resource controls  on mac.

    Get returns 1 or 0 for checks, radio buttons, number for edit text (0 or value of text), index for combo
    boxes and GetCtlValue for Mac resource controls

    Set accepts the same values that Get returns. (if an edit control specify either val or textVal
 */

PUBLIC ELONG FINEXPORT FX_GetDlgCntlVal(  EWND hDlg, ECNTLID dialogItem, ECSTR textVal, twobyte maxTextValLen, CONST ELONG version  = FXT_VERSION );
PUBLIC void FINEXPORT FX_SetDlgCntlVal(  EWND hDlg, ECNTLID dialogItem, ECCSTR textVal, ELONG val, CONST ELONG version  = FXT_VERSION );


/* Send a WM_TIMER message to hDlg, with id timerID, every msInterval milliseconds */

PUBLIC void FINEXPORT FX_SetTimer( EWND hDlg, twobyte timerID, ufourbyte msInterval, CONST ELONG version  = FXT_VERSION );


/* Enable/Disable the dialog item */

PUBLIC void FINEXPORT FX_EnableDlgItem(  EWND hDlg, ECNTLID dialogItem, tbool enable, CONST ELONG version  = FXT_VERSION );

/* Set the dlg's focus item (item with blinking cursor, item that gets keys) */

PUBLIC void FINEXPORT FX_SetDlgFocus(  EWND hDlg, ECNTLID dialogItem, /* 0 == kill focus */ CONST ELONG version  = FXT_VERSION );


/* get val in units (UNITS_* in UNITSDEC.H) from dialog edit text, store in val */
PUBLIC void FINEXPORT FX_GetDlgItemUser( EWND hDlg, ECNTLID dlgItem, twobyte units, double *val, CONST ELONG version  = FXT_VERSION );

/* set val in units (UNITS_* in UNITSDEC.H) in dialog edit text */

PUBLIC void FINEXPORT FX_SetDlgItemUser( EWND hDlg, ECNTLID dlgItem, twobyte units, double val, CONST ELONG version  = FXT_VERSION );


/* present an alert, using string resource, alertStrID, if non-zero, else use alertStr,
   wType is one of MB_* flags in EVENTDECS.H, alertStrID id is ESTRID (see version.h
   for explanation of ESTRID format). If alertStrID is 0, then alertStr is used instead.
   button clicked (IDOK, IDCANCEL, IDYES, IDNO is returned) */
PUBLIC twobyte FINEXPORT FX_AlertMessage( EWND hParent, CONST EWORD wType , CONST ESTRID alertStrID, ECCSTR alertStr, CONST ELONG version  = FXT_VERSION );


/* Get/Set numerical (or on/off)  value value of a control according to type  */
PUBLIC ELONG FINEXPORT FX_GetDlgItemVal( EWND hDlg, ECNTLID dialogItem, twobyte cntlType, CONST ELONG version  = FXT_VERSION );
PUBLIC void FINEXPORT FX_SetDlgItemVal( EWND hDlg, ECNTLID dialogItem, twobyte cntlType, ELONG val, CONST ELONG version  = FXT_VERSION );

/* Get/Set string value of a static/edit text dialog item */
PUBLIC void FINEXPORT FX_GetDlgString( EWND hDlg, ECNTLID dialogItem, ECSTR textVal, twobyte maxTextValLen, CONST ELONG version  = FXT_VERSION );
PUBLIC void FINEXPORT FX_SetDlgString( EWND hDlg, ECNTLID dialogItem, ECCSTR str, CONST ELONG version  = FXT_VERSION );


/*------------------------------------------------------*/
/* Midi related: */

PUBLIC void FINEXPORT  FX_Playback(
	twobyte startmeas,		/*start playing at this measure*/
	twobyte endmeas,		/*stop playing at this measure (0 = all measures)*/
	twobyte inst,			/*play this inst only (0 = all instruments)*/
	CMPER IU_list,			/*instrument list*/
	CONST ELONG version  = FXT_VERSION );

PUBLIC tbool FINEXPORT FX_ListenToMidi( /*return true if got midi event, false if canceled*/
	EWND hWnd,				/*parent window of listen to midi dialog (can be NULL)*/
	EMIDIEVENT *event,		/*store listened-to event here.*/
	CONST ELONG version  = FXT_VERSION );


/* Paste/Copy midi data (not hooked up yet) */
PUBLIC tbool FINEXPORT FX_CopyMidi( EREGION *pRegion, void *data, fourbyte *size, CONST ELONG version  = FXT_VERSION );
PUBLIC tbool FINEXPORT FX_PasteMidi( EREGION *pRegion, void *data, fourbyte size, CONST ELONG version  = FXT_VERSION );

/* Translate exNote  into a midi key number, using key
(usually comes from FX_GetKeySig) */
PUBLIC twobyte FINEXPORT FX_NoteToMidi( EXNOTE * exNote, twobyte key, CONST ELONG version  = FXT_VERSION );

/* translate Midi note into a string */
/* midiPitchRepType = MIDIPITCH_NUMERIC (e.g. "61") or MIDIPITCH_STRING (e.g. "C#4") */

PUBLIC void FINEXPORT FX_MidiNoteToString( twobyte midiKeyNum, ECSTR str, twobyte maxChars, twobyte midiPitchRepType, CONST ELONG version  = FXT_VERSION );

/* translate string, e.g. "C#4" or "61", back to midi note. */
PUBLIC twobyte FINEXPORT FX_StringToMidiNote( ECSTR str, CONST ELONG version  = FXT_VERSION );

/*------------------------------------------------------*/
/* Note Mover: */

/* Do note mover-type operations, see EXTYPES.H for action values (NOTEMOVER_*),
   source,dest represent measures (duraPos ignored), allNotes==move all notes in measure,
   layer==-1 for all layers,
   WARNING: Moving selected notes that are not in source will result in undefined behavior... */

PUBLIC void FINEXPORT FX_NoteMover (EMUSICPOINT *source, EMUSICPOINT *dest, twobyte action, twobyte layer, tbool allNotes, CONST ELONG version  = FXT_VERSION );

/* Choose a note in an entry for FX_NoteMover (noteoff is offset of note in entry).
   entry == NULLP clears all selections, noteoff == -1 selects all notes in an entry */

PUBLIC void FINEXPORT FX_SelectNote ( EXENTRY *entry, twobyte noteoff, CONST ELONG version  = FXT_VERSION );

/*------------------------------------------------------*/
/* Miscellaneous Utilities: */

PUBLIC twobyte FINEXPORT FX_SlotToInst( twobyte slot, CMPER iuList, CONST ELONG version  = FXT_VERSION ); /*get inst for slot in iuList*/
PUBLIC twobyte FINEXPORT FX_InstToSlot( twobyte inst, CMPER iuList, CONST ELONG version  = FXT_VERSION ); /*get slot in iuList for inst*/

PUBLIC void FINEXPORT  FX_ScrollTo( twobyte meas, twobyte inst, CONST ELONG version  = FXT_VERSION ); /*bring this measure into view*/
																/* if inst>0, bring that inst into view*/

/* Translate to and from enigma font ids (twobyte, stored in file structs that specify a font)
   and font names (e.g. Petrucci) */
PUBLIC tbool FINEXPORT FX_FontNumToName( CONST twobyte fontnum, EFONTNAME *fontname, CONST ELONG version  = FXT_VERSION );
PUBLIC twobyte FINEXPORT FX_FontNameToNum(  CONST EFONTNAME *fontname, CONST ELONG version  = FXT_VERSION );

/* Return currently selected region in pSelRegion, main document window in hWnd (null for Mac), and
   region for entire document in pDocRegion */
PUBLIC tbool FINEXPORT FX_GetCurERegions( EWND *hWnd, EREGION *pSelRegion, EREGION *pDocRegion, CONST ELONG version  = FXT_VERSION );

/* Redraw current document */
PUBLIC void FINEXPORT FX_RedrawMusic ( const EREGION *pRegion, CONST ELONG version  = FXT_VERSION );

/* Get key signature for meas, inst (see key format in ETFSPEC.TXT)
   If transpose is true, then return "transposed" key signature
   (as it appears in the score, not the way it sounds)
   (finds floating key sigs) */

PUBLIC twobyte FINEXPORT FX_GetKeySig ( twobyte meas, twobyte inst, tbool transpose, 
										twobyte * const tcdTransAdj = NULLP,	// adjustment to notes' tcd values
									    CONST ELONG version  = FXT_VERSION );


/* perform an update layout: from page (as if page were the current page),
   2nd param matches functionality of shift key */

PUBLIC void FINEXPORT FX_UpdateLayout( twobyte page, tbool overrideRetainFrozMeas, CONST ELONG version  = FXT_VERSION );


//Gather information about lyric syllable, returns true if syllable found.

//tag, lyricTextNum (raw text num of lyric text), syll (lyric syllable number)
//come from EDTLyric (lyric entry detail),  Syllable is returned as a c string
//in syllStr (syllStr is a string maxChars long).  Ending style of lyric
//is returned in style, actual number of chars in syllable is returned in
//sylLen, if syllable ends in hyphen, isHyphen is true, syllable width
//in EVPUs is returned in width

//Any of syllStr, syllLen, style, isHypen, and width can all be NULL,
//if this information is not required, to speed up the process

//example: if you just need to know if a syllable exists,
//use FX_SyllableInfo(tag, num, syll, NULL, 0, NULL, NULL, NULL, NULL, NULL)

#if 1
PUBLIC tbool FINEXPORT FX_SyllableInfo(  EXTAG tag, twobyte lyricTextNum, twobyte syllNum, //from EDTLyric
									ECSTR syllStr, twobyte maxChars, twobyte *syllLen,
									ETextStyle *style, tbool *isHyphen,
									EVPU *width, CONST ELONG version  = FXT_VERSION );
#else
// CFC 07/10/98: This is what would most likely be needed to address the problems
// with the word extensions plugin that arose because of the addition of the new
// alignment and justification options.
PUBLIC tbool FINEXPORT FX_SyllableInfo( EXTAG tag, twobyte lyricTextNum, twobyte syllNum, //from EDTLyric
									ECSTR syllStr, twobyte maxChars, twobyte *syllLen,
									ETextStyle *style, tbool *isHyphen,
									twobyte inst, twobyte meas, ENTNUM lyEntnum,
									EVPU *lyStartPos, EVPU *lyEndPos, CONST ELONG version  = FXT_VERSION );
#endif

/* return pointer to syllth syllable in str (NULL if syllable doesn't exist), tag
   is the tag that the text is stored under. */
/* Upon return style contains the text style of the syllable requested.
   style->size should be set to 0 when str is at the beginning of a raw text stream,
   otherwise it needs to match str (it needs to contain str's style)
   As syllables are gotten, the caret text is interpreted and style is updated

   FX_GetSyllable(str,1,tag,style), returns the next syllable
   in str after the one pointed to by str. FX_GetSyllable(str,2,tag,style) gets
   the syllable after that, etc. FX_GetSyllable(str,0,tag,style) will
   return str itself. */

PUBLIC ECCSTR FINEXPORT FX_GetSyllable( ECCSTR str, fourbyte syll, EXTAG tag, ETextStyle *style, CONST ELONG version  = FXT_VERSION );

/* Caret command parser:
   If first char in str is a caret (^), This routine parses the caret command, and updates
   style according to the caret command, a pointer to the first non-caret command
   is returned. If caret command is '^^' (print a single caret), then TRUE is returned
   in singlecaret. */

PUBLIC ECCSTR FINEXPORT FX_ParseCaretCommand( ECCSTR str, tbool *singleCaret, EXTAG tag, ETextStyle *style, CONST ELONG version  = FXT_VERSION );

/* Retrieve metrics info for a single entry.
   NOTE: every time you measure an entry on a different page, there
   will be a delay (quite long if page is complicated) while Finale
   calculates layout info for that page.
   Therefore you should try to make all the measurements for
   a single page at once.

   inst, meas are the inst and measure of the entry, entryNum is its entry number,
   pEntryMetrics points to a EEntryMetrics struct, pMeasureMetrics is an optional
   pointer to measure-wide  information/metrics. (may be null if you don't need this info)
   set rebuild to YES to recalculate the layout info (normally this should be NO
   for speed, but if you modify things that effect the layout (adding entries, measures,
   staves, etc.) you will need to set this to YES to get accurate results)

   returns YES if entryNum was found, else NO. */

PUBLIC tbool FINEXPORT FX_GetEntryMetrics(twobyte inst, twobyte meas, ENTNUM entryNum,
	EEntryMetrics *pEntryMetrics, EMeasureMetrics *pMeasureMetrics, tbool rebuild, CONST ELONG version  = FXT_VERSION );

/* Retrieve metrics info for all the entries in specified EXTGF.
   Similar to FX_GetEntryMetrics.  pEntryMetrics needs to point
   to an array of EEntryMetrics structs, one for each entry in
   pExTGF.

   returns YES if pExTGF was found, else NO. */

PUBLIC tbool FINEXPORT FX_GetTGFEntryMetrics(
	EXTGF *pExTGF, EEntryMetrics *pEntryMetrics, EMeasureMetrics *pMeasureMetrics,
	tbool rebuild, CONST ELONG version  = FXT_VERSION );

/* query user for a font, return info in pFont, set noSize/noEfx to disable
   size or efx controls. returns NO if canceled.*/

PUBLIC tbool FINEXPORT FX_FontDialog(
	CONST EWND hParent, EEnigmaFont *pFont,
	ECCSTR pString, CONST tbool noSize, CONST tbool noEfx, CONST ELONG version  = FXT_VERSION );

/* query user for a character in a particular font, return NO if canceled. */

PUBLIC tbool FINEXPORT FX_SymbolDialog(
	CONST EWND hParent, ECHAR *pChar, CONST EEnigmaFont *pFont, CONST ELONG version  = FXT_VERSION );

/* query user for an id of a particular data types (shapes,expressions,etc.)
   cmper for item returned in cmper (cmper also contains item to be selected
   in the dialog initially), type is one of SELECTION_* in extypes.h,
   upon return, update is set to true if items were created or deleted
   (necessitating a redraw). returns NO if canceled. */

PUBLIC tbool FINEXPORT FX_SelectionDialog(
	CONST EWND hParent, twobyte type, CMPER *cmper, tbool *update, CONST ELONG version  = FXT_VERSION );


/* gets the total duration of the specified measure according to its time
	signature. Accounts for independent/composite time signatures. */

PUBLIC TIME_EDU FINEXPORT FX_GetMeasureDuration( const twobyte inst, const twobyte meas, CONST ELONG version  = FXT_VERSION );

/* Start a long edit, and indicate progress during the edit, checking for cancel */

PUBLIC EProgressDataP FINEXPORT FX_StartMassEdit(CONST EREGION *pRegion, CONST fourbyte jobSize, CONST ELONG version  = FXT_VERSION );
PUBLIC void FINEXPORT FX_EndMassEdit( EProgressDataP progressData, tbool canceled, CONST ELONG version  = FXT_VERSION );
PUBLIC tbool FINEXPORT FX_MassEditProgress( EProgressDataP p, CONST twobyte increment,
						CONST twobyte inst, CONST twobyte meas, CONST twobyte layer, CONST ELONG version  = FXT_VERSION );

// Return true if a given layer is currently showing.

PUBLIC tbool FINEXPORT FX_GetShowLayer( CONST twobyte layer, CONST ELONG version  = FXT_VERSION );

/*###################################################################################*/

//#ifdef __cplusplus
}	/* extern "C" */
//#endif

/*--------------------------------------------------------------------------------*/

#ifdef __MWERKS__
#ifndef powerc
#pragma pointers_in_A0
#endif
#endif
/*--------------------------------------------------------------------------------*/

#endif	/*	_FINEXTND_H_ */
