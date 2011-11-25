/*-----------------------------------------------------------------------------

	File:		FXUPPS.H

	Purpose:	definitions for Finale Plugin Functions.
				Allows us to write cross-platform handling of function
				pointers in Windows/68K Mac/PPC Mac

				Prototypes/typedefs for all functions in Finale that
				are called from extensions (and vice-versa) are contained here.

	Copyright © 1989-1997 Coda Music Software.  All rights reserved.
	
	3/25/99 TSF:  NOTE:  Even though this looks like it is a Mac-only file, it's really 
		a cross-platform Mac/Windows file.  GENERATINGCFM will not be defined in Windows
		(or 68k Mac) so it will be compatible...

-------------------------------------------------------------------------------
**
**  02/07/97	TNW		added procInfos APIs for rebar, rebeam, enigma data
**  02/19/97	TNW		return pointers in d0 (for external functions)
**  02/25/97	TNW		more extension api's (dialog, playback, listen, etc.)
**  03/05/97	TNW		use pointers in DO, extern C for UPPs.
**  03/05/97	TNW		comment about MW 68K
**  03/05/97	TNW		corrected syntax error in header inclusion.
**  03/07/97	TNW		moved FXPluginFuncs to here.  Added EXPORT to
**						UPP definitions for extension entry points
**						(important for windows)
**  03/12/97	TNW		added FF_GetDialogItemUPP, FF_SetDialogItemUPP
** 						FF_RedrawMusicUPP, FF_CopyMidiUPP, FF_PasteMidiUPP
** 						FF_NOTEToMidiUPP
**  03/12/97	TNW		added GLOBAL to Finale entry point function types.
**  03/19/97	TNW		use EGLOBAL calling conventions for non-EXPORT functions.
**  03/19/97	TNW		EGLOBAL is FARMEM STDCALL
**  03/31/97	TNW		changed size in EData procs to fourbyte.
**  04/03/97	TNW		Get/SetDialogItemUPP -> Get/SetDlgCntlValUPP
**  04/10/97	TNW		Made FF_DialogHandlerUPP EXPORT instead of EGLOBAL, removed EGLOBAL.
**  04/15/97	TNW		added FF_NoteMoverUPP, inst param to FF_ScrollToUPP, inst, meas param to NoteToMidi
**  04/25/97	TNW		added FF_SelectNoteUPP, FF_GetKeySigUPP, FF_SetTimerUPP, changed params of
**						FF_NoteMoverUPP, FF_NoteToMidiUPP, FF_DialogUPP
**  04/27/97	TNW		FF_GetKeySigUPP needed twobyte return val
**  05/01/97	TNW		params in FF_NoteMover were swapped.
**  05/07/97	TNW		changed second param in DialogHandler from UINT to ufourbyte.
**  05/28/97	TNW		added FF_SetDlgFocusUPP, FF_EnableDlgItemUPP
**  06/03/97	TNW		changed msg param in DialogHandler to utwobyte (to match windows)
**  					removed cast from non-PPC New_FF macros (to get some type-checking)
**  06/18/97	TNW		added FF_UpdateLayoutUPP, FF_GetSyllableUPP,
**						FF_SetDlgItemUserUPP, FF_GetDlgItemUserUPP, FF_StringToMidiNoteUPP,
**						FF_MidiNoteToStringUPP, FF_ParseCaretCommand
**  06/23/97	TNW		eregion.h -> eregdecs.h
**  07/07/97	TNW		added FX_GetEntryMetrics, FX_GetTGFEntryMetrics
**  07/09/97	TNW		added FX_FontDialog, FX_SymbolDialog, FX_SelectionDialog
**  08/15/97	TNW		added FX_SyllableInfo
**  08/18/97	TNW		FX_SyllableInfo gets a tbool return type.
**  08/19/97	TNW		added attributes flags param to FX_InitUPP.
**  08/20/97	TNW		fixed return type in PPC CallFF_SyllableInfoUPP
**  08/27/97    RES     FF_RedrawMusicUPP: made pRegion const
**  09/02/97    RES     added FX_GetMeasureDuration(), FX_GetTGFDuration(), FX_ReduceEntries()
**  09/03/97    TNW     fixed syntax err in FF_ReduceEntriesUPP typedef.
**  09/03/97    TNW     fixed syntax err in CallFF_GetMeasureDurationProc
**  09/10/97	TNW		changed msg param in DialogHandler back to ufourbyte
**						(to accomodate Win32)
**  09/10/97    TNW     added FX_StartMassEdit(), FX_EndMassEdit(), FX_MassEditProgress()
**						added FX_AlertMessage(), FX_Get/SetDlgItemVal(), FX_Get/SetDlgString()
**  09/11/97    RES     added hiVersion param to FinaleExtensionVersion()
**  09/22/97	TNW		alertStrID in FX_AlertMessage is an ESTRID.
**  09/23/97	TNW		added const to FX_*Edata, added FF_Save/Create/Delete/EnumEDataUPP's
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  10/13/97	TNW		added CONST to params in FX_FontNumToName,FX_FontNameToNum
**  10/21/97     fxupps.h       RES       FF_SetDlgStringUPP: ECCSTR
**  10/21/97     fxupps.h       RES       FF_SetDlgCntlValUPP: ECCSTR
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  04/17/98     fxupps.h       CFC       added FX_GetShowLayer()
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: FXUPPS.H,v $
Revision 1.1.1.1  2002/12/19 19:13:12  dfober
first import

** 
** 12    8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 11    8/13/99 12:32p Jlenarz
** Changed extension dialog handler return value to FX_RETURNVAL
** 
** 10    7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 9     6/9/99 3:06 PM Jlenarz
** Additional changes for FX_GetKeySig()
** 
** 8     6/09/99 12:44p Jlenarz
** Added tcdTransAdj to FX_GetKeySig()
** 
** 7     3/29/99 2:32p Rstokes
** changed FINEXPORT back to EXPORT in comments (for historical accuracy
** and tracking)
** 
** 6     3/26/99 4:54 PM Tfischer
** Make plug-ins compilable in Fin2000.  EXPORT->FINEXPORT;  other minor
** related changes
** 
** 5     3/26/99 10:23a Tfischer
** Fix compile error on non-cfm version
** 
** 4     3/25/99 4:56 PM Tfischer
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

#ifndef _FXUPPS_H_
#define _FXUPPS_H_

#ifndef _VERSION_H_
#include "version.h"
#endif

#ifndef _EXTYPES_H_
#include "extypes.h"
#endif

#ifndef _EREGDECS_H_
#include "eregdecs.h"
#endif

#ifndef _EDATA_H_
#include "edata.h"
#endif

#ifndef GENERATINGCFM
#define GENERATINGCFM 0
#endif

/******************************************************************************************
   plug-in function table macros.  Always use corresponding Call macro when
   calling a plugin or finale function.  On the Mac, the functions are Universal
   Proc Ptrs and cannot be called directly, this provides some platform independence.
 *******************************************************************************************/

/*--------------------------------------------------------------------------------*/

/*
** The Mixed Mode procInfos here assume Think C-
** style calling conventions.  These conventions are used by
** Metrowerks with the exception of pointer return types, which
** in Metrowerks 68K are returned in A0, instead of the standard
** D0.
*/

#ifdef __MWERKS__
#ifndef powerc
#pragma pointers_in_D0
#endif
#endif


#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif

/* FX_VersionUPP */

#if GENERATINGCFM
typedef UniversalProcPtr FX_VersionUPP;

enum {
	uppFX_VersionProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(ELONG)))
};

#define NewFX_VersionProc(FUNC)		\
		(FX_VersionUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFX_VersionProcInfo, GetCurrentArchitecture())
#define CallFX_VersionProc(FUNC,hiVersion)		\
		(ELONG)CallUniversalProc((UniversalProcPtr)(FUNC), uppFX_VersionProcInfo, (hiVersion))

#else

typedef ELONG (FINEXPORT *FX_VersionUPP)( CONST ELONG hiVersion );
#define NewFX_VersionProc(FUNC)		\
		((FX_VersionUPP) (FUNC))
#define CallFX_VersionProc(FUNC, hiVersion)		\
		(*(FUNC))((hiVersion))

#endif

/*--------------------------------------------------------------------------------*/

/* FX_InitUPP */

#if GENERATINGCFM
typedef UniversalProcPtr FX_InitUPP;

enum {
	uppFX_InitProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(FLAG_32 *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};

#define NewFX_InitProc(FUNC)		\
		(FX_InitUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFX_InitProcInfo, GetCurrentArchitecture())
#define CallFX_InitProc(FUNC,hWnd,flags)		\
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFX_InitProcInfo,(hWnd),(flags))

#else

typedef tbool (FINEXPORT *FX_InitUPP)( CONST EWND hWnd, FLAG_32 *flags );
#define NewFX_InitProc(FUNC)		\
		((FX_InitUPP) (FUNC))
#define CallFX_InitProc(FUNC,hWnd,flags)		\
		(*(FUNC))((hWnd),(flags))

#endif

/*--------------------------------------------------------------------------------*/
/* FX_EnumerateUPP */

#if GENERATINGCFM
typedef UniversalProcPtr FX_EnumerateUPP;

enum {
	uppFX_EnumerateProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(FXT_MENUSTRING_PACKET *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};

#define NewFX_EnumerateProc(FUNC)		\
		(FX_EnumerateUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFX_EnumerateProcInfo, GetCurrentArchitecture())
#define CallFX_EnumerateProc(FUNC, hWnd, index, buf) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFX_EnumerateProcInfo, (hWnd), (index), (buf))
#else

typedef tbool (FINEXPORT *FX_EnumerateUPP)( CONST EWND hWnd, CONST twobyte index, FXT_MENUSTRING_PACKET *buf );
#define NewFX_EnumerateProc(FUNC)		\
		((FX_EnumerateUPP) (FUNC))
#define CallFX_EnumerateProc(FUNC, hWnd, index, buf)		\
		(*(FUNC))((hWnd), (index), (buf))

#endif


/*--------------------------------------------------------------------------------*/
/* FX_InvokeUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FX_InvokeUPP;
enum {
	uppFX_InvokeProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(EREGION *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFX_InvokeProc(FUNC)		\
		(FX_InvokeUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFX_InvokeProcInfo, GetCurrentArchitecture())
#define CallFX_InvokeProc(FUNC, hWnd, index, pSelRegion, pDocRegion)		\
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFX_InvokeProcInfo, (hWnd), (index), (pSelRegion), (pDocRegion))
#else

typedef tbool (FINEXPORT *FX_InvokeUPP)( CONST EWND hWnd, CONST twobyte index,
	EREGION *pSelRegion, EREGION *pDocRegion );
#define NewFX_InvokeProc(FUNC)		\
		((FX_InvokeUPP) (FUNC))
#define CallFX_InvokeProc(FUNC, hWnd, index, pSelRegion, pDocRegion)		\
		(*(FUNC))((hWnd), (index), (pSelRegion), (pDocRegion))
#endif


/*--------------------------------------------------------------------------------*/
/* FX_TerminateUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FX_TerminateUPP;
enum {
	uppFX_TerminateProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| RESULT_SIZE(SIZE_CODE(0))
};
#define NewFX_TerminateProc(FUNC)		\
		(FX_TerminateUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFX_TerminateProcInfo, GetCurrentArchitecture())
#define CallFX_TerminateProc(FUNC, hWnd)		\
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFX_TerminateProcInfo, (hWnd))
#else

typedef void (FINEXPORT *FX_TerminateUPP)( CONST EWND hWnd );
#define NewFX_TerminateProc(FUNC)		\
		((FX_TerminateUPP) (FUNC))
#define CallFX_TerminateProc(FUNC, hWnd)		\
		(*(FUNC))((hWnd))

#endif


/*--------------------------------------------------------------------------------*/
/* FX_GetMenuStatusUPP */

#if GENERATINGCFM
typedef UniversalProcPtr FX_GetMenuStatusUPP;

enum {
	uppFX_GetMenuStatusProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};

#define NewFX_GetMenuStatusProc(FUNC)		\
		(FX_GetMenuStatusUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFX_GetMenuStatusProcInfo, GetCurrentArchitecture())
#define CallFX_GetMenuStatusProc(FUNC, index) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFX_GetMenuStatusProcInfo, (index))
#else

typedef tbool (FINEXPORT *FX_GetMenuStatusUPP)( CONST twobyte index);
#define NewFX_GetMenuStatusProc(FUNC)		\
		((FX_GetMenuStatusUPP) (FUNC))
#define CallFX_GetMenuStatusProc(FUNC, index)		\
		(*(FUNC))((index))

#endif

/*--------------------------------------------------------------------------------*/
/*
 *  Finale entry points
 */
/*--------------------------------------------------------------------------------*/

/* FF_LoadTGFUPP */

typedef  EXTGF *EXTGFPTR;

#if GENERATINGCFM

typedef UniversalProcPtr FF_LoadTGFUPP;
enum {
	uppFF_LoadTGFProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CMPER)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(EXTGF *)))
};
#define NewFF_LoadTGFProc(FUNC)		\
		(FF_LoadTGFUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_LoadTGFProcInfo, GetCurrentArchitecture())
#define CallFF_LoadTGFProc(FUNC, iuList, staffSlot, meas, layer, mirrorOK, FXversion) \
		(EXTGFPTR)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_LoadTGFProcInfo, (iuList), (staffSlot), (meas), (layer), (mirrorOK), (FXversion))
#else


typedef EXTGFPTR	(FINEXPORT *FF_LoadTGFUPP)( CONST CMPER iuList, CONST twobyte staffSlot,
	CONST twobyte meas, CONST twobyte layer, CONST tbool mirrorOK, CONST ELONG FXversion );
#define NewFF_LoadTGFProc(FUNC)		\
		(/*(FF_LoadTGFUPP)*/ (FUNC))
#define CallFF_LoadTGFProc(FUNC, iuList, staffSlot, meas, layer, mirrorOK, FXversion)		\
		(*(FUNC))((iuList), (staffSlot), (meas), (layer), (mirrorOK), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_InsertEntryUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_InsertEntryUPP;
enum {
	uppFF_InsertEntryProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_InsertEntryProc(FUNC)		\
		(FF_InsertEntryUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_InsertEntryProcInfo, GetCurrentArchitecture())
#define CallFF_InsertEntryProc(FUNC, pTgf, offset, numToAdd, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_InsertEntryProcInfo, (pTgf), (offset), (numToAdd), (FXversion))
#else

typedef tbool (FINEXPORT *FF_InsertEntryUPP)( EXTGF *pTgf, CONST twobyte offset, CONST twobyte numToAdd, CONST ELONG FXversion );
#define NewFF_InsertEntryProc(FUNC)		\
		(/*(FF_InsertEntryUPP)*/ (FUNC))
#define CallFF_InsertEntryProc(FUNC, pTgf, offset, numToAdd, FXversion)		\
		(*(FUNC))((pTgf), (offset), (numToAdd), (FXversion))
#endif


/*--------------------------------------------------------------------------------*/
/* FF_DeleteEntryUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_DeleteEntryUPP;
enum {
	uppFF_DeleteEntryProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_DeleteEntryProc(FUNC)		\
		(FF_DeleteEntryUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_DeleteEntryProcInfo, GetCurrentArchitecture())
#define CallFF_DeleteEntryProc(FUNC, pTgf, offset, numToDelete, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_DeleteEntryProcInfo, (pTgf), (offset), (numToDelete), (FXversion))
#else

typedef tbool (FINEXPORT *FF_DeleteEntryUPP)( EXTGF *pTgf, CONST twobyte offset, CONST twobyte numToDelete, CONST ELONG FXversion );
#define NewFF_DeleteEntryProc(FUNC)		\
		(/*(FF_DeleteEntryUPP)*/ (FUNC))
#define CallFF_DeleteEntryProc(FUNC, pTgf, offset, numToDelete, FXversion) \
		(*(FUNC))((pTgf), (offset), (numToDelete), (FXversion))
#endif


/*--------------------------------------------------------------------------------*/
/* FF_SaveTGFUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SaveTGFUPP;
enum {
	uppFF_SaveTGFProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_SaveTGFProc(FUNC)		\
		(FF_SaveTGFUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SaveTGFProcInfo, GetCurrentArchitecture())
#define CallFF_SaveTGFProc(FUNC, pTgf, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SaveTGFProcInfo, (pTgf), (FXversion))
#else

typedef tbool	(FINEXPORT *FF_SaveTGFUPP)( EXTGF *pTgf, CONST ELONG FXversion );
#define NewFF_SaveTGFProc(FUNC)		\
		(/*(FF_SaveTGFUPP)*/ (FUNC))
#define CallFF_SaveTGFProc(FUNC, pTgf, FXversion)		\
		(*(FUNC))((pTgf), (FXversion))
#endif


/*--------------------------------------------------------------------------------*/
/* FF_FreeTGFUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_FreeTGFUPP;
enum {
	uppFF_FreeTGFProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(EXTGF *)))
};
#define NewFF_FreeTGFProc(FUNC)		\
		(FF_FreeTGFUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_FreeTGFProcInfo, GetCurrentArchitecture())
#define CallFF_FreeTGFProc(FUNC, pTgf, FXversion) \
		(EXTGFPTR)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_FreeTGFProcInfo, (pTgf), (FXversion))
#else

typedef EXTGFPTR (FINEXPORT *FF_FreeTGFUPP)(EXTGF *pTgf, CONST ELONG FXversion);
#define NewFF_FreeTGFProc(FUNC)		\
		(/*(FF_FreeTGFUPP)*/ (FUNC))
#define CallFF_FreeTGFProc(FUNC, pTgf, FXversion) \
		(*(FUNC))((pTgf), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_RebarRegionUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_RebarRegionUPP;
enum {
	uppFF_RebarRegionProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(FLAG_16)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_RebarRegionProc(FUNC)		\
		(FF_RebarRegionUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_RebarRegionProcInfo, GetCurrentArchitecture())
#define CallFF_RebarRegionProc(FUNC, pRegion, stopFlags, reBeam, padWithRests, checkAccis, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_RebarRegionProcInfo, (pRegion),(stopFlags),(reBeam),(padWithRests),(checkAccis),(FXversion))
#else

typedef tbool (FINEXPORT *FF_RebarRegionUPP)(
	CONST EREGION *pRegion, CONST FLAG_16 stopFlags, CONST tbool reBeam,
	CONST tbool padWithRests, CONST tbool checkAccis, CONST ELONG FXversion );
#define NewFF_RebarRegionProc(FUNC)		\
		(/*(FF_RebarRegionUPP)*/ (FUNC))
#define CallFF_RebarRegionProc(FUNC, pRegion, stopFlags, reBeam, padWithRests, checkAccis, FXversion) \
		(*(FUNC))( (pRegion),(stopFlags),(reBeam),(padWithRests),(checkAccis),(FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_RebeamRegionUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_RebeamRegionUPP;
enum {
	uppFF_RebeamRegionProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ETimeSig *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_RebeamRegionProc(FUNC)		\
		(FF_RebeamRegionUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_RebeamRegionProcInfo, GetCurrentArchitecture())
#define CallFF_RebeamRegionProc(FUNC,  pRegion, pExtTimeSig, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_RebeamRegionProcInfo, (pRegion), (pExtTimeSig), (FXversion))
#else

typedef tbool (FINEXPORT *FF_RebeamRegionUPP)( CONST EREGION * pRegion, CONST ETimeSig * pExtTimeSig, CONST ELONG FXversion );
#define NewFF_RebeamRegionProc(FUNC)		\
		(/*(FF_RebeamRegionUPP)*/ (FUNC))
#define CallFF_RebeamRegionProc(FUNC, pRegion, pExtTimeSig, FXversion ) \
		(*(FUNC))((pRegion), (pExtTimeSig), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_LoadEDataUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_LoadEDataUPP;
enum {
	uppFF_LoadEDataProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EDataID *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(fourbyte *)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_LoadEDataProc(FUNC)		\
		(FF_LoadEDataUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_LoadEDataProcInfo, GetCurrentArchitecture())
#define CallFF_LoadEDataProc(FUNC, extag, id, data, size, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_LoadEDataProcInfo, (extag), (id), (data), (size), (FXversion))
#else

typedef tbool (FINEXPORT *FF_LoadEDataUPP)( CONST EXTAG extag, CONST EDataID *id, void *data, fourbyte *size, CONST ELONG FXversion);
#define NewFF_LoadEDataProc(FUNC)		\
		(/*(FF_LoadEDataUPP)*/ (FUNC))
#define CallFF_LoadEDataProc(FUNC, extag, id, data, size, FXversion) \
		(*(FUNC))((extag), (id), (data), (size), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_DeleteEDataUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_DeleteEDataUPP;
enum {
	uppFF_DeleteEDataProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EDataID *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_DeleteEDataProc(FUNC)		\
		(FF_DeleteEDataUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_DeleteEDataProcInfo, GetCurrentArchitecture())
#define CallFF_DeleteEDataProc(FUNC, extag, id, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_DeleteEDataProcInfo, (extag), (id), (FXversion))
#else

typedef tbool (FINEXPORT *FF_DeleteEDataUPP)( CONST EXTAG extag, CONST EDataID *id, CONST ELONG FXversion);
#define NewFF_DeleteEDataProc(FUNC)		\
		(/*(FF_DeleteEDataUPP)*/ (FUNC))
#define CallFF_DeleteEDataProc(FUNC, extag, id, FXversion) \
		(*(FUNC))((extag), (id), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_SaveEDataUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SaveEDataUPP;
enum {
	uppFF_SaveEDataProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EDataID *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(fourbyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_SaveEDataProc(FUNC)		\
		(FF_SaveEDataUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SaveEDataProcInfo, GetCurrentArchitecture())
#define CallFF_SaveEDataProc(FUNC, extag, id, data, size, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SaveEDataProcInfo, (extag), (id), (data), (size), (FXversion))
#else

typedef tbool (FINEXPORT *FF_SaveEDataUPP)( CONST EXTAG extag, CONST EDataID *id, CONST void *data, CONST fourbyte size, CONST ELONG FXversion);
#define NewFF_SaveEDataProc(FUNC)		\
		(/*(FF_SaveEDataUPP)*/ (FUNC))
#define CallFF_SaveEDataProc(FUNC, extag, id, data, size, FXversion) \
		(*(FUNC))((extag), (id), (data), (size), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_CreateEDataUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_CreateEDataUPP;
enum {
	uppFF_CreateEDataProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EDataID *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(fourbyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_CreateEDataProc(FUNC)		\
		(FF_CreateEDataUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_CreateEDataProcInfo, GetCurrentArchitecture())
#define CallFF_CreateEDataProc(FUNC, extag, id, data, size, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_CreateEDataProcInfo, (extag), (id), (data), (size), (FXversion))
#else

typedef tbool (FINEXPORT *FF_CreateEDataUPP)( CONST EXTAG extag, EDataID *id, CONST void *data, CONST fourbyte size, CONST ELONG FXversion);
#define NewFF_CreateEDataProc(FUNC)		\
		(/*(FF_CreateEDataUPP)*/ (FUNC))
#define CallFF_CreateEDataProc(FUNC, extag, id, data, size, FXversion) \
		(*(FUNC))((extag), (id), (data), (size), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_EnumEDataUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_EnumEDataUPP;
enum {
	uppFF_EnumEDataProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EDataID *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(fourbyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_EnumEDataProc(FUNC)		\
		(FF_EnumEDataUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_EnumEDataProcInfo, GetCurrentArchitecture())
#define CallFF_EnumEDataProc(FUNC, extag, id, data, size, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_EnumEDataProcInfo, (extag), (id), (data), (size), (FXversion))
#else

typedef tbool (FINEXPORT *FF_EnumEDataUPP)( CONST EXTAG extag, EDataID *id, void *data, CONST fourbyte size, CONST ELONG FXversion);
#define NewFF_EnumEDataProc(FUNC)		\
		(/*(FF_EnumEDataUPP)*/ (FUNC))
#define CallFF_EnumEDataProc(FUNC, extag, id, data, size, FXversion) \
		(*(FUNC))((extag), (id), (data), (size), (FXversion))
#endif


/*--------------------------------------------------------------------------------*/
/* FF_DialogHandlerUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_DialogHandlerUPP;
enum {
	uppFF_DialogHandlerProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ufourbyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(WPARAM)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(LPARAM)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(void *)))
		| RESULT_SIZE(SIZE_CODE(sizeof(FX_RETURNVAL)))
};
#define NewFF_DialogHandlerProc(FUNC)		\
		(FF_DialogHandlerUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_DialogHandlerProcInfo, GetCurrentArchitecture())
#define CallFF_DialogHandlerProc(FUNC, hDlg, msg, wParam, lParam, data) \
		(FX_RETURNVAL)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_DialogHandlerProcInfo, (hDlg), (msg), (wParam), (lParam), (data))
#else

typedef FX_RETURNVAL (FINEXPORT *FF_DialogHandlerUPP)( EWND hDlg, ufourbyte msg, WPARAM wParam, LPARAM lParam, void *data );
#define NewFF_DialogHandlerProc(FUNC)		\
		(/*(FF_DialogHandlerUPP)*/ (FUNC))
#define CallFF_DialogHandlerProc(FUNC, hDlg, msg, wParam, lParam, data) \
		(*(FUNC))((hDlg), (msg), (wParam), (lParam), (data))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_DialogUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_DialogUPP;
enum {
	uppFF_DialogProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(FF_DialogHandlerUPP)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(EWND)))
};
#define NewFF_DialogProc(FUNC)		\
		(FF_DialogUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_DialogProcInfo, GetCurrentArchitecture())
#define CallFF_DialogProc( FUNC, nDlgID, lpCallBack, hParent, type, data, FXversion) \
		(EWND)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_DialogProcInfo, (nDlgID), (lpCallBack), (hParent), (type), (data), (FXversion))
#else

typedef EWND (FINEXPORT *FF_DialogUPP)( twobyte nDlgID, FF_DialogHandlerUPP lpCallBack, EWND hParent, twobyte type, void *data, CONST ELONG FXversion);
#define NewFF_DialogProc(FUNC)		\
		(/*(FF_DialogUPP)*/ (FUNC))
#define CallFF_DialogProc( FUNC, nDlgID, lpCallBack, hParent, type, data, FXversion) \
		(*(FUNC))((nDlgID), (lpCallBack), (hParent), (type), (data), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_EndDialogUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_EndDialogUPP;
enum {
	uppFF_EndDialogProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_EndDialogProc(FUNC)		\
		(FF_EndDialogUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_EndDialogProcInfo, GetCurrentArchitecture())
#define CallFF_EndDialogProc( FUNC, hDlg, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_EndDialogProcInfo, (hDlg), (FXversion))
#else

typedef void (FINEXPORT *FF_EndDialogUPP)(EWND hDlg, CONST ELONG FXversion);
#define NewFF_EndDialogProc(FUNC)		\
		(/*(FF_EndDialogUPP)*/ (FUNC))
#define CallFF_EndDialogProc( FUNC, hDlg, FXversion ) \
		(*(FUNC))((hDlg), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_EnableDlgItemUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_EnableDlgItemUPP;
enum {
	uppFF_EnableDlgItemProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_EnableDlgItemProc(FUNC)		\
		(FF_EnableDlgItemUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_EnableDlgItemProcInfo, GetCurrentArchitecture())
#define CallFF_EnableDlgItemProc(FUNC, hDlg, dialogItem, enable, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_EnableDlgItemProcInfo, (hDlg), (dialogItem), (enable), (FXversion))
#else

typedef void (FINEXPORT *FF_EnableDlgItemUPP)( EWND hDlg, ECNTLID dialogItem, tbool enable, CONST ELONG FXversion );
#define NewFF_EnableDlgItemProc(FUNC)		\
		(/*(FF_EnableDlgItemUPP)*/ (FUNC))
#define CallFF_EnableDlgItemProc(FUNC, hDlg, dialogItem, enable, FXversion ) \
		(*(FUNC))((hDlg), (dialogItem), (enable), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SetDlgFocusUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SetDlgFocusUPP;
enum {
	uppFF_SetDlgFocusProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SetDlgFocusProc(FUNC)		\
		(FF_SetDlgFocusUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SetDlgFocusProcInfo, GetCurrentArchitecture())
#define CallFF_SetDlgFocusProc(FUNC, hDlg, dialogItem, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SetDlgFocusProcInfo, (hDlg), (dialogItem), (FXversion))
#else

typedef void (FINEXPORT *FF_SetDlgFocusUPP)( EWND hDlg, ECNTLID dialogItem, CONST ELONG FXversion);
#define NewFF_SetDlgFocusProc(FUNC)		\
		(/*(FF_SetDlgFocusUPP)*/ (FUNC))
#define CallFF_SetDlgFocusProc(FUNC, hDlg, dialogItem, FXversion) \
		(*(FUNC))((hDlg), (dialogItem), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetDlgCntlValUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetDlgCntlValUPP;
enum {
	uppFF_GetDlgCntlValProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ECSTR)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_GetDlgCntlValProc(FUNC)		\
		(FF_GetDlgCntlValUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetDlgCntlValProcInfo, GetCurrentArchitecture())
#define CallFF_GetDlgCntlValProc(FUNC, hDlg, dialogItem, textVal, maxTextValLen, FXversion) \
		(ELONG)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetDlgCntlValProcInfo, (hDlg), (dialogItem), (textVal), (maxTextValLen), (FXversion))
#else

typedef ELONG (FINEXPORT *FF_GetDlgCntlValUPP)( EWND hDlg, ECNTLID dialogItem, ECSTR textVal, twobyte maxTextValLen, CONST ELONG FXversion );
#define NewFF_GetDlgCntlValProc(FUNC)		\
		(/*(FF_GetDlgCntlValUPP)*/ (FUNC))
#define CallFF_GetDlgCntlValProc(FUNC, hDlg, dialogItem, textVal, maxTextValLen, FXversion ) \
		(*(FUNC))((hDlg), (dialogItem), (textVal),(maxTextValLen), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SetDlgCntlValUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SetDlgCntlValUPP;
enum {
	uppFF_SetDlgCntlValProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ECCSTR)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SetDlgCntlValProc(FUNC)		\
		(FF_SetDlgCntlValUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SetDlgCntlValProcInfo, GetCurrentArchitecture())
#define CallFF_SetDlgCntlValProc(FUNC, hDlg, dialogItem, textVal, val, FXversion ) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SetDlgCntlValProcInfo, (hDlg), (dialogItem), (textVal), (val), (FXversion))
#else

typedef void (FINEXPORT *FF_SetDlgCntlValUPP)( EWND hDlg, ECNTLID dialogItem, ECCSTR textVal, ELONG val, CONST ELONG FXversion );
#define NewFF_SetDlgCntlValProc(FUNC)		\
		(/*(FF_SetDlgCntlValUPP)*/ (FUNC))
#define CallFF_SetDlgCntlValProc(FUNC, hDlg, dialogItem, textVal, val, FXversion ) \
		(*(FUNC))((hDlg), (dialogItem), (textVal), (val), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_SlotToInstUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SlotToInstUPP;
enum {
	uppFF_SlotToInstProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(CMPER)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(twobyte)))
};
#define NewFF_SlotToInstProc(FUNC)		\
		(FF_SlotToInstUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SlotToInstProcInfo, GetCurrentArchitecture())
#define CallFF_SlotToInstProc(FUNC, slot, iuList, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SlotToInstProcInfo, (slot), (iuList), (FXversion))
#else

typedef tbool (FINEXPORT *FF_SlotToInstUPP)( twobyte slot, CMPER iuList, CONST ELONG FXversion );
#define NewFF_SlotToInstProc(FUNC)		\
		(/*(FF_SlotToInstUPP)*/ (FUNC))
#define CallFF_SlotToInstProc(FUNC, slot, iuList, FXversion) \
		(*(FUNC))((slot), (iuList), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_ScrollToUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_ScrollToUPP;
enum {
	uppFF_ScrollToProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_ScrollToProc(FUNC)		\
		(FF_ScrollToUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_ScrollToProcInfo, GetCurrentArchitecture())
#define CallFF_ScrollToProc(FUNC, meas,inst, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_ScrollToProcInfo, (meas),(inst), (FXversion))
#else

typedef void (FINEXPORT *FF_ScrollToUPP)( twobyte meas, twobyte inst, CONST ELONG FXversion );
#define NewFF_ScrollToProc(FUNC)		\
		(/*(FF_ScrollToUPP)*/ (FUNC))
#define CallFF_ScrollToProc(FUNC, meas,inst, FXversion) \
		(*(FUNC))((meas),(inst),(FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_PlaybackUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_PlaybackUPP;
enum {
	uppFF_PlaybackProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(CMPER)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_PlaybackProc(FUNC)		\
		(FF_PlaybackUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_PlaybackProcInfo, GetCurrentArchitecture())
#define CallFF_PlaybackProc(FUNC, start, end, staff, iulist, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_PlaybackProcInfo, (start), (end), (staff), (iulist), (FXversion))
#else

typedef void (FINEXPORT *FF_PlaybackUPP)( twobyte start, twobyte end, twobyte staff, CMPER iulist, CONST ELONG FXversion );
#define NewFF_PlaybackProc(FUNC)		\
		(/*(FF_PlaybackUPP)*/ (FUNC))
#define CallFF_PlaybackProc(FUNC, start, end, staff, iulist, FXversion) \
		(*(FUNC))((start), (end), (staff), (iulist), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_ListenToMidiUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_ListenToMidiUPP;
enum {
	uppFF_ListenToMidiProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EMIDIEVENT *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_ListenToMidiProc(FUNC)		\
		(FF_ListenToMidiUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_ListenToMidiProcInfo, GetCurrentArchitecture())
#define CallFF_ListenToMidiProc(FUNC, hWnd, event, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_ListenToMidiProcInfo, (hWnd), (event), (FXversion))
#else

typedef tbool (FINEXPORT *FF_ListenToMidiUPP)( EWND hWnd, EMIDIEVENT *event, CONST ELONG FXversion ); 
#define NewFF_ListenToMidiProc(FUNC)		\
		(/*(FF_ListenToMidiUPP)*/ (FUNC))
#define CallFF_ListenToMidiProc(FUNC, hWnd, event, FXversion) \
		(*(FUNC))((hWnd), (event), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_FontNumToNameUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_FontNumToNameUPP;
enum {
	uppFF_FontNumToNameProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EFONTNAME *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_FontNumToNameProc(FUNC)		\
		(FF_FontNumToNameUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_FontNumToNameProcInfo, GetCurrentArchitecture())
#define CallFF_FontNumToNameProc(FUNC, fontnum, fontname, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_FontNumToNameProcInfo, (fontnum), (fontname), (FXversion))
#else

typedef tbool (FINEXPORT *FF_FontNumToNameUPP)( CONST twobyte fontnum, EFONTNAME *fontname, CONST ELONG FXversion );
#define NewFF_FontNumToNameProc(FUNC)		\
		(/*(FF_FontNumToNameUPP)*/ (FUNC))
#define CallFF_FontNumToNameProc(FUNC, fontnum, fontname, FXversion) \
		(*(FUNC))((fontnum), (fontname), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/
/* FF_FontNameToNumUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_FontNameToNumUPP;
enum {
	uppFF_FontNameToNumProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EFONTNAME *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(twobyte)))
};
#define NewFF_FontNameToNumProc(FUNC)		\
		(FF_FontNameToNumUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_FontNameToNumProcInfo, GetCurrentArchitecture())
#define CallFF_FontNameToNumProc(FUNC, fontname, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_FontNameToNumProcInfo, (fontname), (FXversion))
#else

typedef twobyte (FINEXPORT *FF_FontNameToNumUPP)( CONST EFONTNAME *fontname, CONST ELONG FXversion );
#define NewFF_FontNameToNumProc(FUNC)		\
		(/*(FF_FontNameToNumUPP)*/ (FUNC))
#define CallFF_FontNameToNumProc(FUNC, fontname, FXversion) \
		(*(FUNC))((fontname),(FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetCurERegionsUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetCurERegionsUPP;
enum {
	uppFF_GetCurERegionsProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_GetCurERegionsProc(FUNC)		\
		(FF_GetCurERegionsUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetCurERegionsProcInfo, GetCurrentArchitecture())
#define CallFF_GetCurERegionsProc(FUNC, hWnd, pSelRegion, pDocRegion, FXversion ) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetCurERegionsProcInfo, (hWnd), (pSelRegion), (pDocRegion), (FXversion))
#else

typedef tbool (FINEXPORT *FF_GetCurERegionsUPP)( EWND *hWnd, EREGION *pSelRegion, EREGION *pDocRegion, CONST ELONG FXversion );
#define NewFF_GetCurERegionsProc(FUNC)		\
		(/*(FF_GetCurERegionsUPP)*/ (FUNC))
#define CallFF_GetCurERegionsProc(FUNC, hWnd, pSelRegion, pDocRegion, FXversion) \
		(*(FUNC))((hWnd), (pSelRegion), (pDocRegion), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_RedrawMusicUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_RedrawMusicUPP;
enum {
	uppFF_RedrawMusicProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_RedrawMusicProc(FUNC)		\
		(FF_RedrawMusicUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_RedrawMusicProcInfo, GetCurrentArchitecture())
#define CallFF_RedrawMusicProc(FUNC, pRegion, FXversion) \
	(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_RedrawMusicProcInfo, (pRegion), (FXversion))
#else

typedef void (FINEXPORT *FF_RedrawMusicUPP)( const EREGION *pRegion, const ELONG FXversion );
#define NewFF_RedrawMusicProc(FUNC)		\
		(/*(FF_RedrawMusicUPP)*/ (FUNC))
#define CallFF_RedrawMusicProc(FUNC, pRegion, FXversion) \
		(*(FUNC))((pRegion), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_CopyMidiUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_CopyMidiUPP;
enum {
	uppFF_CopyMidiProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(fourbyte *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_CopyMidiProc(FUNC)		\
		(FF_CopyMidiUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_CopyMidiProcInfo, GetCurrentArchitecture())
#define CallFF_CopyMidiProc(FUNC, pRegion, midiData, size, FXversion ) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_CopyMidiProcInfo, (pRegion), (midiData), (size), (FXversion))
#else

typedef tbool (FINEXPORT *FF_CopyMidiUPP)( EREGION *pRegion, void *midiData, fourbyte *size, CONST ELONG FXversion );
#define NewFF_CopyMidiProc(FUNC)		\
		(/*(FF_CopyMidiUPP)*/ (FUNC))
#define CallFF_CopyMidiProc(FUNC, pRegion, midiData, size, FXversion) \
		(*(FUNC))((pRegion), (midiData), (size), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_PasteMidiUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_PasteMidiUPP;
enum {
	uppFF_PasteMidiProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EREGION *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(fourbyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_PasteMidiProc(FUNC)		\
		(FF_PasteMidiUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_PasteMidiProcInfo, GetCurrentArchitecture())
#define CallFF_PasteMidiProc(FUNC, pRegion, midiData, size, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_PasteMidiProcInfo, (pRegion), (midiData), (size), (FXversion))
#else

typedef tbool (FINEXPORT *FF_PasteMidiUPP)( EREGION *pSelRegion, void *midiData, fourbyte size, const ELONG FXversion );
#define NewFF_PasteMidiProc(FUNC)		\
		(/*(FF_PasteMidiUPP)*/ (FUNC))
#define CallFF_PasteMidiProc(FUNC, pRegion, midiData, size, FXversion) \
		(*(FUNC))((pRegion), (midiData), (size), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_NoteToMidiUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_NoteToMidiUPP;
enum {
	uppFF_NoteToMidiProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof( EXNOTE *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(twobyte)))
};
#define NewFF_NoteToMidiProc(FUNC)		\
		(FF_NoteToMidiUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_NoteToMidiProcInfo, GetCurrentArchitecture())
#define CallFF_NoteToMidiProc(FUNC, exNote, key, FXversion) \
		(twobyte)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_NoteToMidiProcInfo, (exNote),(key),(FXversion))
#else

typedef twobyte (FINEXPORT *FF_NoteToMidiUPP)( EXNOTE * exNote, twobyte key, CONST ELONG FXVersion );
#define NewFF_NoteToMidiProc(FUNC)		\
		(/*(FF_NoteToMidiUPP)*/ (FUNC))
#define CallFF_NoteToMidiProc(FUNC, exNote, key, FXversion) \
		(*(FUNC))((exNote),(key),(FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_NoteMoverUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_NoteMoverUPP;
enum {
	uppFF_NoteMoverProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EMUSICPOINT *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EMUSICPOINT *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_NoteMoverProc(FUNC)		\
		(FF_NoteMoverUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_NoteMoverProcInfo, GetCurrentArchitecture())
#define CallFF_NoteMoverProc(FUNC, source, dest, action, layer, allNotes, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_NoteMoverProcInfo, (source), (dest), (action), (layer), (allNotes), (FXversion))
#else

typedef void (FINEXPORT *FF_NoteMoverUPP)(EMUSICPOINT *source, EMUSICPOINT *dest, twobyte action, twobyte layer, tbool allNotes, CONST ELONG FXversion);
#define NewFF_NoteMoverProc(FUNC)		\
		(/*(FF_NoteMoverUPP)*/ (FUNC))
#define CallFF_NoteMoverProc(FUNC, source, dest, action, layer, allNotes, FXversion) \
		(*(FUNC))((source), (dest), (action), (layer), (allNotes), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SelectNoteUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SelectNoteUPP;
enum {
	uppFF_SelectNoteProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXENTRY *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SelectNoteProc(FUNC)		\
		(FF_SelectNoteUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SelectNoteProcInfo, GetCurrentArchitecture())
#define CallFF_SelectNoteProc(FUNC, entry, noteoff, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SelectNoteProcInfo, (entry), (noteoff), (FXversion))
#else

typedef void (FINEXPORT *FF_SelectNoteUPP)( EXENTRY *entry, twobyte noteoff, CONST ELONG FXversion );
#define NewFF_SelectNoteProc(FUNC)		\
		(/*(FF_SelectNoteUPP)*/ (FUNC))
#define CallFF_SelectNoteProc(FUNC, entry, noteoff, FXversion) \
		(*(FUNC))((entry), (noteoff), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetKeySigUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetKeySigUPP;
enum {
	uppFF_GetKeySigProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte *)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(twobyte)))
};
#define NewFF_GetKeySigProc(FUNC)		\
		(FF_GetKeySigUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetKeySigProcInfo, GetCurrentArchitecture())
#define CallFF_GetKeySigProc(FUNC, meas, inst, transpose, tcdTransAdj, FXversion) \
		(twobyte)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetKeySigProcInfo, (meas), (inst), (transpose), (tcdTransAdj), (FXversion))
#else

typedef twobyte (FINEXPORT *FF_GetKeySigUPP)( twobyte meas, twobyte inst, tbool transpose, twobyte * const tcdTransAdj, CONST ELONG FXversion );
#define NewFF_GetKeySigProc(FUNC)		\
		(/*(FF_GetKeySigUPP)*/ (FUNC))
#define CallFF_GetKeySigProc(FUNC, meas, inst, transpose, tcdTransAdj, FXversion) \
		(*(FUNC))((meas), (inst), (transpose), (tcdTransAdj), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SetTimerUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SetTimerUPP;
enum {
	uppFF_SetTimerProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ufourbyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SetTimerProc(FUNC)		\
		(FF_SetTimerUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SetTimerProcInfo, GetCurrentArchitecture())
#define CallFF_SetTimerProc(FUNC, hWnd, timerID, interval, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SetTimerProcInfo, (hWnd), (timerID), (interval), (FXversion))
#else

typedef void (FINEXPORT *FF_SetTimerUPP)( EWND hWnd, twobyte timerID, ufourbyte interval, CONST ELONG FXversion );
#define NewFF_SetTimerProc(FUNC)		\
		(/*(FF_SetTimerUPP)*/ (FUNC))
#define CallFF_SetTimerProc(FUNC, hWnd, timerID, interval, FXversion) \
		(*(FUNC))((hWnd), (timerID), (interval), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_MidiNoteToStringUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_MidiNoteToStringUPP;
enum {
	uppFF_MidiNoteToStringProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECSTR)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_MidiNoteToStringProc(FUNC)		\
		(FF_MidiNoteToStringUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_MidiNoteToStringProcInfo, GetCurrentArchitecture())
#define CallFF_MidiNoteToStringProc(FUNC, note, str, maxchars, reptype, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_MidiNoteToStringProcInfo, (note), (str), (maxchars), (reptype), (FXversion))
#else

typedef void (FINEXPORT *FF_MidiNoteToStringUPP)( twobyte note, ECSTR str, twobyte maxchars, twobyte reptype, CONST ELONG FXversion );
#define NewFF_MidiNoteToStringProc(FUNC)		\
		(/*(FF_MidiNoteToStringUPP)*/ (FUNC))
#define CallFF_MidiNoteToStringProc(FUNC, note, str, maxchars, reptype, FXversion) \
		(*(FUNC))((note), (str), (maxchars), (reptype), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_StringToMidiNoteUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_StringToMidiNoteUPP;
enum {
	uppFF_StringToMidiNoteProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(ECSTR)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(twobyte)))
};
#define NewFF_StringToMidiNoteProc(FUNC)		\
		(FF_StringToMidiNoteUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_StringToMidiNoteProcInfo, GetCurrentArchitecture())
#define CallFF_StringToMidiNoteProc(FUNC, str, FXversion) \
		(twobyte)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_StringToMidiNoteProcInfo, (str), (FXversion))
#else

typedef twobyte (FINEXPORT *FF_StringToMidiNoteUPP)( ECSTR str, CONST ELONG FXversion );
#define NewFF_StringToMidiNoteProc(FUNC)		\
		(/*(FF_StringToMidiNoteUPP)*/ (FUNC))
#define CallFF_StringToMidiNoteProc(FUNC, str, FXversion) \
		(*(FUNC))((str), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetDlgItemUserUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetDlgItemUserUPP;
enum {
	uppFF_GetDlgItemUserProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(double *)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_GetDlgItemUserProc(FUNC)		\
		(FF_GetDlgItemUserUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetDlgItemUserProcInfo, GetCurrentArchitecture())
#define CallFF_GetDlgItemUserProc(FUNC, hDlg, dlgItem, units, val, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetDlgItemUserProcInfo, (hDlg), (dlgItem), (units), (val), (FXversion))
#else

typedef void (FINEXPORT *FF_GetDlgItemUserUPP)( EWND hDlg, ECNTLID dlgItem, twobyte units, double *val, CONST ELONG FXversion );
#define NewFF_GetDlgItemUserProc(FUNC)		\
		(/*(FF_GetDlgItemUserUPP)*/ (FUNC))
#define CallFF_GetDlgItemUserProc(FUNC, hDlg, dlgItem, units, val, FXversion) \
		(*(FUNC))((hDlg), (dlgItem), (units), (val), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SetDlgItemUserUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SetDlgItemUserUPP;
enum {
	uppFF_SetDlgItemUserProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(double)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SetDlgItemUserProc(FUNC)		\
		(FF_SetDlgItemUserUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SetDlgItemUserProcInfo, GetCurrentArchitecture())
#define CallFF_SetDlgItemUserProc(FUNC, hDlg, dlgItem, units, val, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SetDlgItemUserProcInfo, (hDlg), (dlgItem), (units), (val), (FXversion))
#else

typedef void (FINEXPORT *FF_SetDlgItemUserUPP)( EWND hDlg, ECNTLID dlgItem, twobyte units, double val, CONST ELONG FXversion );
#define NewFF_SetDlgItemUserProc(FUNC)		\
		(/*(FF_SetDlgItemUserUPP)*/ (FUNC))
#define CallFF_SetDlgItemUserProc(FUNC, hDlg, dlgItem, units, val, FXversion) \
		(*(FUNC))((hDlg), (dlgItem), (units), (val), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetSyllableUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetSyllableUPP;
enum {
	uppFF_GetSyllableProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(ECCSTR)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(fourbyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ETextStyle *)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(ECSTR)))
};
#define NewFF_GetSyllableProc(FUNC)		\
		(FF_GetSyllableUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetSyllableProcInfo, GetCurrentArchitecture())
#define CallFF_GetSyllableProc(FUNC, str, syllNum, tag, style, FXversion) \
		(ECCSTR)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetSyllableProcInfo, (str), (syllNum), (tag), (style), (FXversion))
#else

typedef ECCSTR (FINEXPORT *FF_GetSyllableUPP)( ECCSTR str, fourbyte syllNum, EXTAG tag, ETextStyle *style, CONST ELONG FXversion );
#define NewFF_GetSyllableProc(FUNC)		\
		(/*(FF_GetSyllableUPP)*/ (FUNC))
#define CallFF_GetSyllableProc(FUNC, str, syllNum, tag, style, FXversion) \
		(*(FUNC))((str), (syllNum), (tag), (style), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SyllableInfoUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SyllableInfoUPP;
enum {
	uppFF_SyllableInfoProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ECSTR)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(twobyte *)))
		| STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(ETextStyle *)))
		| STACK_ROUTINE_PARAMETER(8, SIZE_CODE(sizeof(tbool *)))
		| STACK_ROUTINE_PARAMETER(9, SIZE_CODE(sizeof(EVPU *)))
		| STACK_ROUTINE_PARAMETER(10, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_SyllableInfoProc(FUNC)		\
		(FF_SyllableInfoUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SyllableInfoProcInfo, GetCurrentArchitecture())
#define CallFF_SyllableInfoProc(FUNC, tag, lyricTextNum, syllNum, syllStr, maxChars, syllLen, style, isHyphen, width, FXversion ) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SyllableInfoProcInfo, (tag), (lyricTextNum), \
		(syllNum), (syllStr), (maxChars), (syllLen), (style), (isHyphen), (width), (FXversion))
#else

typedef tbool (FINEXPORT *FF_SyllableInfoUPP)(  EXTAG tag, twobyte lyricTextNum, twobyte syllNum,
									ECSTR syllStr, twobyte maxChars, twobyte *syllLen,
									ETextStyle *style, tbool *isHyphen, EVPU *width, CONST ELONG FXversion );
#define NewFF_SyllableInfoProc(FUNC)		\
		(/*(FF_SyllableInfoUPP)*/ (FUNC))
#define CallFF_SyllableInfoProc(FUNC, tag, lyricTextNum, syllNum, syllStr, maxChars, syllLen, style, isHyphen, width, FXversion ) \
		(*(FUNC))((tag), (lyricTextNum), (syllNum), (syllStr), (maxChars), (syllLen), (style), (isHyphen), (width), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_ParseCaretCommandUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_ParseCaretCommandUPP;
enum {
	uppFF_ParseCaretCommandProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(ECCSTR)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(tbool *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(EXTAG)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ETextStyle *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(ECSTR)))
};
#define NewFF_ParseCaretCommandProc(FUNC)		\
		(FF_ParseCaretCommandUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_ParseCaretCommandProcInfo, GetCurrentArchitecture())
#define CallFF_ParseCaretCommandProc(FUNC, str, singleCaret, tag, style, FXversion) \
		(ECCSTR)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_ParseCaretCommandProcInfo, (str), (singleCaret), (tag), (style), (FXversion))
#else

typedef ECCSTR (FINEXPORT *FF_ParseCaretCommandUPP)( ECCSTR str, tbool *singleCaret, EXTAG tag, ETextStyle *style, CONST ELONG FXversion );
#define NewFF_ParseCaretCommandProc(FUNC)		\
		(/*(FF_ParseCaretCommandUPP)*/ (FUNC))
#define CallFF_ParseCaretCommandProc(FUNC, str, singleCaret, tag, style, FXversion) \
		(*(FUNC))((str), (singleCaret), (tag), (style), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_UpdateLayoutUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_UpdateLayoutUPP;
enum {
	uppFF_UpdateLayoutProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_UpdateLayoutProc(FUNC)		\
		(FF_UpdateLayoutUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_UpdateLayoutProcInfo, GetCurrentArchitecture())
#define CallFF_UpdateLayoutProc(FUNC, page, overrideRetainFrozMeas, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_UpdateLayoutProcInfo, (page), (overrideRetainFrozMeas), (FXversion))
#else

typedef void (FINEXPORT *FF_UpdateLayoutUPP)( twobyte page, tbool overrideRetainFrozMeas, CONST ELONG FXversion );
#define NewFF_UpdateLayoutProc(FUNC)		\
		(/*(FF_UpdateLayoutUPP)*/ (FUNC))
#define CallFF_UpdateLayoutProc(FUNC, page, overrideRetainFrozMeas, FXversion) \
		(*(FUNC))((page), (overrideRetainFrozMeas), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetEntryMetricsUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetEntryMetricsUPP;
enum {
	uppFF_GetEntryMetricsProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ENTNUM)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(EEntryMetrics *)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(EMeasureMetrics *)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_GetEntryMetricsProc(FUNC)		\
		(FF_GetEntryMetricsUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetEntryMetricsProcInfo, GetCurrentArchitecture())
#define CallFF_GetEntryMetricsProc(FUNC, inst, meas, entryNum, pEntryMetrics, pMeasureMetrics, rebuild, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetEntryMetricsProcInfo, (inst), (meas), (entryNum), (pEntryMetrics), (pMeasureMetrics), (rebuild), (FXversion))
#else

typedef tbool (FINEXPORT *FF_GetEntryMetricsUPP)(
	twobyte inst, twobyte meas, ENTNUM entryNum,
	EEntryMetrics *pEntryMetrics, EMeasureMetrics *pMeasureMetrics, tbool rebuild, CONST ELONG FXversion);
#define NewFF_GetEntryMetricsProc(FUNC)		\
		(/*(FF_GetEntryMetricsUPP)*/ (FUNC))
#define CallFF_GetEntryMetricsProc(FUNC, inst, meas, entryNum, pEntryMetrics, pMeasureMetrics, rebuild, FXversion) \
		(*(FUNC))((inst), (meas), (entryNum), (pEntryMetrics), (pMeasureMetrics), (rebuild), (FXversion))
#endif


/*--------------------------------------------------------------------------------*/

/* FF_GetTGFMetricsUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetTGFMetricsUPP;
enum {
	uppFF_GetTGFMetricsProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EEntryMetrics *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(EMeasureMetrics *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_GetTGFMetricsProc(FUNC)		\
		(FF_GetTGFMetricsUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetTGFMetricsProcInfo, GetCurrentArchitecture())
#define CallFF_GetTGFMetricsProc(FUNC, pTGF, pEntryMetrics, pMeasureMetrics, rebuild, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetTGFMetricsProcInfo, (pTGF), (pEntryMetrics), (pMeasureMetrics), (rebuild), (FXversion))
#else

typedef tbool (FINEXPORT *FF_GetTGFMetricsUPP)(
	EXTGF *pTGF, EEntryMetrics *pEntryMetrics, EMeasureMetrics *pMeasureMetrics,
	tbool rebuild, CONST ELONG FXversion );
#define NewFF_GetTGFMetricsProc(FUNC)		\
		(/*(FF_GetTGFMetricsUPP)*/ (FUNC))
#define CallFF_GetTGFMetricsProc(FUNC, pTGF, pEntryMetrics, pMeasureMetrics, rebuild, FXversion) \
		(*(FUNC))((pTGF), (pEntryMetrics), (pMeasureMetrics), (rebuild), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_FontDialogUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_FontDialogUPP;
enum {
	uppFF_FontDialogProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CONST EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EEnigmaFont *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ECCSTR)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_FontDialogProc(FUNC)		\
		(FF_FontDialogUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_FontDialogProcInfo, GetCurrentArchitecture())
#define CallFF_FontDialogProc(FUNC, hParent, pFont, pString, noSize, noEfx, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_FontDialogProcInfo, (hParent), (pFont), (pString), (noSize), (noEfx), (FXversion))
#else

typedef tbool (FINEXPORT *FF_FontDialogUPP)(
	CONST EWND hParent, EEnigmaFont *pFont,
	ECCSTR pString, CONST tbool noSize, CONST tbool noEfx, CONST ELONG FXversion );
#define NewFF_FontDialogProc(FUNC)		\
		(/*(FF_FontDialogUPP)*/ (FUNC))
#define CallFF_FontDialogProc(FUNC, hParent, pFont, pString, noSize, noEfx, FXversion) \
		(*(FUNC))((hParent), (pFont), (pString), (noSize), (noEfx), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SymbolDialogUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SymbolDialogUPP;
enum {
	uppFF_SymbolDialogProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CONST EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECHAR *)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(CONST EEnigmaFont *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_SymbolDialogProc(FUNC)		\
		(FF_SymbolDialogUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SymbolDialogProcInfo, GetCurrentArchitecture())
#define CallFF_SymbolDialogProc(FUNC, hParent, pChar, pFont, FXversion ) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SymbolDialogProcInfo, (hParent), (pChar), (pFont), (FXversion))
#else

typedef tbool (FINEXPORT *FF_SymbolDialogUPP)(
	CONST EWND hParent, ECHAR *pChar, CONST EEnigmaFont *pFont, CONST ELONG FXversion );
#define NewFF_SymbolDialogProc(FUNC)		\
		(/*(FF_SymbolDialogUPP)*/ (FUNC))
#define CallFF_SymbolDialogProc(FUNC, hParent, pChar, pFont, FXversion) \
		(*(FUNC))((hParent), (pChar), (pFont), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SelectionDialogUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SelectionDialogUPP;
enum {
	uppFF_SelectionDialogProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CONST EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(CMPER *)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(tbool *)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_SelectionDialogProc(FUNC)		\
		(FF_SelectionDialogUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SelectionDialogProcInfo, GetCurrentArchitecture())
#define CallFF_SelectionDialogProc(FUNC, hParent, type, cmper, update, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SelectionDialogProcInfo, (hParent), (type), (cmper), (update), (FXversion))
#else

typedef tbool (FINEXPORT *FF_SelectionDialogUPP)( CONST EWND hParent, twobyte type, CMPER *cmper, tbool *update, CONST ELONG FXversion );
#define NewFF_SelectionDialogProc(FUNC)		\
		(/*(FF_SelectionDialogUPP)*/ (FUNC))
#define CallFF_SelectionDialogProc(FUNC, hParent, type, cmper, update, FXversion) \
		(*(FUNC))((hParent), (type), (cmper), (update), (FXversion))
#endif

/*------------------------------------------------------*/

/* FF_GetMeasureDurationUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetMeasureDurationUPP;
enum {
	uppFF_GetMeasureDurationProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(TIME_EDU)))
};
#define NewFF_GetMeasureDurationProc(FUNC)		\
		(FF_GetMeasureDurationUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetMeasureDurationProcInfo, GetCurrentArchitecture())
#define CallFF_GetMeasureDurationProc(FUNC, inst, meas, FXversion) \
		(TIME_EDU)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetMeasureDurationProcInfo, (inst), (meas), (FXversion))
#else

typedef TIME_EDU (FINEXPORT *FF_GetMeasureDurationUPP)( const twobyte inst, const twobyte meas, const ELONG  FXversion );
#define NewFF_GetMeasureDurationProc(FUNC)		\
		(/*(FF_GetMeasureDurationUPP)*/ (FUNC))
#define CallFF_GetMeasureDurationProc(FUNC, inst, meas, FXversion) \
		(*(FUNC))((inst), (meas), (FXversion))
#endif

/*------------------------------------------------------*/

/* FF_GetTGFDurationUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetTGFDurationUPP;
enum {
	uppFF_GetTGFDurationProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(const EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(TIME_EDU)))
};
#define NewFF_GetTGFDurationProc(FUNC)		\
		(FF_GetTGFDurationUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetTGFDurationProcInfo, GetCurrentArchitecture())
#define CallFF_GetTGFDurationProc(FUNC, ptgf, FXversion) \
		(TIME_EDU)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetTGFDurationProcInfo, (ptgf), (FXversion))
#else

typedef TIME_EDU (FINEXPORT *FF_GetTGFDurationUPP)( const EXTGF *ptgf, const ELONG FXversion );
#define NewFF_GetTGFDurationProc(FUNC)		\
		(/*(FF_GetTGFDurationUPP)*/ (FUNC))
#define CallFF_GetTGFDurationProc(FUNC, ptgf, FXversion) \
		(*(FUNC))((ptgf), (FXversion))
#endif

/*------------------------------------------------------*/

/* FF_ReduceEntriesUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_ReduceEntriesUPP;
enum {
	uppFF_ReduceEntriesProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EXTGF *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(0))
};
#define NewFF_ReduceEntriesProc(FUNC)		\
		(FF_ReduceEntriesUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_ReduceEntriesProcInfo, GetCurrentArchitecture())
#define CallFF_ReduceEntriesProc(FUNC, ptgf, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_ReduceEntriesProcInfo, (ptgf), (FXversion))
#else

typedef void (FINEXPORT *FF_ReduceEntriesUPP)( EXTGF *ptgf, CONST ELONG FXversion );
#define NewFF_ReduceEntriesProc(FUNC)		\
		(/*(FF_ReduceEntriesUPP)*/ (FUNC))
#define CallFF_ReduceEntriesProc(FUNC, ptgf, FXversion) \
		(*(FUNC))((ptgf), (FXversion))
#endif


/*------------------------------------------------------*/

/* FF_StartMassEditUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_StartMassEditUPP;
enum {
	uppFF_StartMassEditProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CONST EREGION *)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(CONST fourbyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(EProgressDataP)))
};
#define NewFF_StartMassEditProc(FUNC)		\
		(FF_StartMassEditUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_StartMassEditProcInfo, GetCurrentArchitecture())
#define CallFF_StartMassEditProc(FUNC, pReg, jobSize, FXversion) \
		(EProgressDataP)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_StartMassEditProcInfo, (pReg), (jobSize), (FXversion))
#else

typedef EProgressDataP (FINEXPORT *FF_StartMassEditUPP)(CONST EREGION *pRegion, CONST fourbyte jobSize, CONST ELONG FXversion);
#define NewFF_StartMassEditProc(FUNC)		\
		(/*(FF_StartMassEditUPP)*/ (FUNC))
#define CallFF_StartMassEditProc(FUNC, pReg, jobSize, FXversion) \
		(*(FUNC))((pReg), (jobSize), (FXversion))
#endif

/*------------------------------------------------------*/

/* FF_EndMassEditUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_EndMassEditUPP;
enum {
	uppFF_EndMassEditProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EProgressDataP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(tbool)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(0))
};
#define NewFF_EndMassEditProc(FUNC)		\
		(FF_EndMassEditUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_EndMassEditProcInfo, GetCurrentArchitecture())
#define CallFF_EndMassEditProc(FUNC, progressData, canceled, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_EndMassEditProcInfo, (progressData),(canceled),(FXversion))
#else

typedef void (FINEXPORT *FF_EndMassEditUPP)( EProgressDataP progressData, tbool canceled, CONST ELONG FXversion );
#define NewFF_EndMassEditProc(FUNC)		\
		(/*(FF_EndMassEditUPP)*/ (FUNC))
#define CallFF_EndMassEditProc(FUNC, progressData, canceled, FXversion) \
		(*(FUNC))((progressData),(canceled), (FXversion))
#endif

/*------------------------------------------------------*/

/* FF_MassEditProgressUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_MassEditProgressUPP;
enum {
	uppFF_MassEditProgressProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EProgressDataP)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(CONST twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(CONST twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(CONST twobyte)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(CONST twobyte)))
		| STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_MassEditProgressProc(FUNC)		\
		(FF_MassEditProgressUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_MassEditProgressProcInfo, GetCurrentArchitecture())
#define CallFF_MassEditProgressProc(FUNC, p,increment,inst,meas,layer, FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_MassEditProgressProcInfo, (p),(increment),(inst),(meas),(layer),(FXversion))
#else

typedef tbool (FINEXPORT *FF_MassEditProgressUPP)( EProgressDataP p, CONST twobyte increment,
						CONST twobyte inst, CONST twobyte meas, CONST twobyte layer, CONST ELONG FXversion );
#define NewFF_MassEditProgressProc(FUNC)		\
		(/*(FF_MassEditProgressUPP)*/ (FUNC))
#define CallFF_MassEditProgressProc(FUNC, p,increment,inst,meas,layer, FXversion) \
		(*(FUNC))((p),(increment),(inst),(meas),(layer),(FXversion))
#endif

/*------------------------------------------------------*/
/* FF_GetShowLayerUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetShowLayerUPP;
enum {
	uppFF_GetShowLayerProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CONST twobyte)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(tbool)))
};
#define NewFF_GetShowLayerProc(FUNC)		\
		(FF_GetShowLayerUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetShowLayerProcInfo, GetCurrentArchitecture())
#define CallFF_GetShowLayerProc(FUNC,layer,FXversion) \
		(tbool)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetShowLayerProcInfo, (layer),(FXversion))
#else

typedef tbool (FINEXPORT *FF_GetShowLayerUPP)( CONST twobyte layer, CONST ELONG FXversion );
#define NewFF_GetShowLayerProc(FUNC)		\
		(/*(FF_GetShowLayerUPP)*/ (FUNC))
#define CallFF_GetShowLayerProc( FUNC, layer, FXversion ) \
		(*(FUNC))((layer), (FXversion))
#endif

/*------------------------------------------------------*/

/* FF_AlertMessageUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_AlertMessageUPP;
enum {
	uppFF_AlertMessageProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CONST EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(CONST EWORD)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(CONST ESTRID)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ECCSTR)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(twobyte)))
};
#define NewFF_AlertMessageProc(FUNC)		\
		(FF_AlertMessageUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_AlertMessageProcInfo, GetCurrentArchitecture())
#define CallFF_AlertMessageProc(FUNC,hParent, wType,alertStrID, alertStr, FXversion) \
		(twobyte)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_AlertMessageProcInfo, (hParent),(wType),(alertStrID),(alertStr), (FXversion))
#else

typedef twobyte (FINEXPORT *FF_AlertMessageUPP)( EWND hParent, CONST EWORD wType , CONST ESTRID alertStrID, ECCSTR alertStr, CONST ELONG FXversion);
#define NewFF_AlertMessageProc(FUNC)		\
		(/*(FF_AlertMessageUPP)*/ (FUNC))
#define CallFF_AlertMessageProc( FUNC, hParent, wType, alertStrID, alertStr, FXversion ) \
		(*(FUNC))((hParent),(wType),(alertStrID),(alertStr), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_GetDlgItemValUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetDlgItemValUPP;
enum {
	uppFF_GetDlgItemValProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_GetDlgItemValProc(FUNC)		\
		(FF_GetDlgItemValUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetDlgItemValProcInfo, GetCurrentArchitecture())
#define CallFF_GetDlgItemValProc(FUNC, hDlg, dialogItem, cntlType, FXversion) \
		(ELONG)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetDlgItemValProcInfo, (hDlg), (dialogItem), (cntlType), (FXversion))
#else

typedef ELONG (FINEXPORT *FF_GetDlgItemValUPP)( EWND hDlg, ECNTLID dialogItem, twobyte cntlType, CONST ELONG FXversion );
#define NewFF_GetDlgItemValProc(FUNC)		\
		(/*(FF_GetDlgItemValUPP)*/ (FUNC))
#define CallFF_GetDlgItemValProc(FUNC, hDlg, dialogItem, cntlType, FXversion ) \
		(*(FUNC))((hDlg), (dialogItem), (cntlType), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SetDlgItemValUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SetDlgItemValUPP;
enum {
	uppFF_SetDlgItemValProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
		| STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SetDlgItemValProc(FUNC)		\
		(FF_SetDlgItemValUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SetDlgItemValProcInfo, GetCurrentArchitecture())
#define CallFF_SetDlgItemValProc(FUNC, hDlg, dialogItem, textVal, val, FXversion ) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SetDlgItemValProcInfo, (hDlg), (dialogItem), (textVal), (val), (FXversion))
#else

typedef void (FINEXPORT *FF_SetDlgItemValUPP)( EWND hDlg, ECNTLID dialogItem, twobyte cntlType, ELONG val, CONST ELONG FXversion );
#define NewFF_SetDlgItemValProc(FUNC)		\
		(/*(FF_SetDlgItemValUPP)*/ (FUNC))
#define CallFF_SetDlgItemValProc(FUNC, hDlg, dialogItem, cntlType, val, FXversion ) \
		(*(FUNC))((hDlg), (dialogItem), (cntlType), (val), (FXversion))
#endif


/*--------------------------------------------------------------------------------*/

/* FF_GetDlgStringUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_GetDlgStringUPP;
enum {
	uppFF_GetDlgStringProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ECSTR)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(twobyte)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ELONG)))
		| RESULT_SIZE(SIZE_CODE(0))
};
#define NewFF_GetDlgStringProc(FUNC)		\
		(FF_GetDlgStringUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_GetDlgStringProcInfo, GetCurrentArchitecture())
#define CallFF_GetDlgStringProc(FUNC, hDlg, dialogItem, textVal, maxTextValLen, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_GetDlgStringProcInfo, (hDlg), (dialogItem), (textVal), (maxTextValLen), (FXversion))
#else

typedef void (FINEXPORT *FF_GetDlgStringUPP)( EWND hDlg, ECNTLID dialogItem, ECSTR textVal, twobyte maxTextValLen, CONST ELONG FXversion );
#define NewFF_GetDlgStringProc(FUNC)		\
		(/*(FF_GetDlgStringUPP)*/ (FUNC))
#define CallFF_GetDlgStringProc(FUNC, hDlg, dialogItem, textVal, maxTextValLen, FXversion ) \
		(*(FUNC))((hDlg), (dialogItem), (textVal),(maxTextValLen), (FXversion))
#endif

/*--------------------------------------------------------------------------------*/

/* FF_SetDlgStringUPP */

#if GENERATINGCFM

typedef UniversalProcPtr FF_SetDlgStringUPP;
enum {
	uppFF_SetDlgStringProcInfo = kThinkCStackBased
		| STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(EWND)))
		| STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ECNTLID)))
		| STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ECCSTR)))
		| STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ELONG)))
};
#define NewFF_SetDlgStringProc(FUNC)		\
		(FF_SetDlgStringUPP) NewRoutineDescriptor((ProcPtr)(FUNC), uppFF_SetDlgStringProcInfo, GetCurrentArchitecture())
#define CallFF_SetDlgStringProc(FUNC, hDlg, dialogItem, str, FXversion) \
		(void)CallUniversalProc((UniversalProcPtr)(FUNC), uppFF_SetDlgStringProcInfo, (hDlg), (dialogItem), (str), (FXversion))
#else

typedef void (FINEXPORT *FF_SetDlgStringUPP)( EWND hDlg, ECNTLID dialogItem, ECCSTR str, CONST ELONG FXversion);
#define NewFF_SetDlgStringProc(FUNC)		\
		(/*(FF_SetDlgStringUPP)*/ (FUNC))
#define CallFF_SetDlgStringProc(FUNC, hDlg, dialogItem, str, FXversion) \
		(*(FUNC))((hDlg), (dialogItem), (str), (FXversion))
#endif



/*------------------------------------------------------*/

/* This is used by Finale to call the extension: */

typedef struct {
    ELONG size;
    ELONG version;
    FX_VersionUPP	getversion;
    FX_InitUPP		init;
    FX_EnumerateUPP	enumerate;
    FX_InvokeUPP	invoke;
    FX_TerminateUPP terminate;
    FX_GetMenuStatusUPP getMenuStatus;
} FXPluginFuncs;

/*--------------------------------------------------------------------------------*/

#ifdef __cplusplus
}	/* extern "C" */
#endif


#ifdef __MWERKS__
#ifndef powerc
#pragma pointers_in_A0
#endif
#endif

#endif /* _FXUPPS_H_ */
