/*
Module Name:	VERSION.H -- conditional compilation control values for
							 operating system and hardware dependent code.
Module Number:
Date:			10/21/87

Author:			Jim Rauser

Purpose:		This header file defines symbolic constants for the operating
				systems and hardware environments under which Enigma has been
				installed.  They are intended to be used in conditional
				compilation directives to choose between sections of OS/HW
				dependent code.  As an example:

					#if OPERATING_SYSTEM == MAC_OS
					#	include <ExOtherIncs.h>
					#endif

					#if OPERATING_SYSTEM == WINDOWS
					#	include <exothrin.h>
					#endif

				This example overcomes the differences in the include file
				names between the Mac OS and MS-DOS.

				This mechanism is intended only to overcome small differences
				(such as the include file names) which exist throughout the
				Enigma code.  Note that separate versions of the hardware-
				specific routines for playback, transcription, etc. will still
				be maintained in separate source files.

Modification History:

		10/21/87	jsr		Created.

**  08/29/1989   version.h     randy      added miscellaneous defines
**  09/07/1989   version.h     randy      added NULLFP
**  04/30/1990   version.h     randy      windows.h now always included
**  10/19/1990   version.h     randy      added EXPORT definition
**  11/07/1990   version.h     randy      added APPLICATION defines
**  11/25/1990   version.h     randy      tags are now type ETAG. This was
**						done to alleviate the inconsistencies rampant
**						throughout Enigma (some of which is my fault), where
**						tags are sometimes signed and sometimes unsigned.
**						I have chosen to typedef ETAG as signed because
**						lyric tags set the high bit, and are thus sometimes
**						considered negative.
**  11/30/1990   version.h     randy      added defines for TWOBYTE_MIN and
**                      family. Needed a system independent way to specify
**                      the maximum and minimum values for ENIGMA's basic
**                      data types. In order to avoid including limits.h
**                      unless it is needed, the whole section is controlled
**                      by whether _NEED_LIMITS_ is defined.
**  11/30/1990   version.h     randy      added PRELUDE identifier
**  12/01/1990   version.h     randy      added ENOTE
**  01/03/1991   version.h     randy      tTIME is now signed; added TIME_EDU,
**                    TIME_RTU
**  01/11/1991   version.h     randy      All .H files: added H_ to name
**                    define to distinguish it from the _C_ used in the
**                    .C files
**  01/31/1991   version.h     randy      moved YES/NO definitions here from
**                    local.h. Also removed the (twobyte) casts; they are
**                    now defined simply as 1 and 0
**  02/05/1991   version.h     randy      added check for QSEG
**                    environment variable. This is for MicroQuill's
**                    Segmentor utility, which must see all PRIVATE
**                    functions.
**  04/02/1991   version.h     randy      added ENTNUM
**  04/04/1991   version.h     randy      defined MAX_FILENAME_LENGTH,
**                    MAX_PATH_LENGTH, MAX_PATHNAME_LENGTH
**  04/24/1991   version.h     randy      LIMITS.H is now unconditionally
**                    included. This greatly simplifies the inclusion process,
**                    at the risk of overflowing the compiler's symbol tables.
**  05/04/1991   version.h     randy      added CMPER, FLAG_16, FLAG_32
**                    defines
**  05/08/1991   version.h     randy      removed ENOTE. It is now typedefed
**                    in grafdecs.h for new ENTRY/ENOTE structure
**  05/15/1991   version.h     randy      added a bunch of defines for
**                    version 3.0: APP_NAME, ENV_NAME, ENIGMA_VERSION,
**                    APPLICATION_VERSION
**  07/11/1991   version.h     randy      QSEG --> SEGMENT
**  01/02/1992   version.h     randy      _VERSION_ONLY_: the files that
**                    include CTREE .h files (CTOTHERS.C, et al.) need to
**                    be able to include VERSION.H only to define the
**                    operating environment (specifically, OPERATING_SYSTEM),
**                    without defining all the other typedefs. This is because
**                    CTREE uses many of the same constants we do, for
**                    different purposes (GLOBAL, etc.). VERSION.H has been
**                    enhanced by Scott M. to handle the following simple
**                    protocol:
**
**                    #define _VERSION_ONLY_
**                    #include "VERSION.H"
**                    #if OPERATING_SYSTEM == MAC_OS
**                    ... (include CTREE files here)...
**                    #endif
**                    #include "VERSION.H"
**
**                    Notice that VERSION.H will #undef _VERSION_ONLY_
**                    internally, so all the caller has to do is include
**                    VERSION.H a second time to get all the rest of the
**                    typedefs and defines. VERSION.H also undefines
**                    symbols before it defines them.
**  01/02/1992   version.h     randy      added WINVER define to force
**                    Win 3.1 to produce 3.0 compatible applications
**  01/10/1992   version.h     randy      Windows: changed GLOBAL and LOCAL
**                    to use _fastcall calling convention as an experiment
**  01/10/1992   version.h     randy      can't use _fastcall. Although not
**                    documented, it is illegal with /Gw or /GW. So,
**                    basically, you can't use it with Windows.
**  02/05/1992   version.h     randy      added DEVELOP define. Be sure to
**                    undefine this before release!
**  02/20/1992   version.h     randy      added EXTERN define
**  03/03/1992   version.h     randy      added INIT(x) mechanism
**  03/03/1992   version.h      SKM	      added macros from local.h
**  04/08/1992   version.h     randy      integrated with Scott M.
**  05/06/1992   version.h     randy      added MAKETAG()
**  06/05/1992   version.h     randy      added EPOINT, PEPOINT, LPEPOINT
**  06/09/1992   version.h     randy      integrated with Mac version
**  06/11/1992   version.h     randy      added LPEHPOINT
**  06/12/1992   version.h     randy      added ECHAR, ECSTR, LECSTR
**  06/18/1992   version.h     randy      added PEHRECT, LPEHRECT (for
**                    Windows)
**  06/18/1992   version.h     randy      added ECNTL (for Windows)
**  06/25/1992   version.h     randy      added ERGN (for Windows)
**  06/30/1992   version.h     kraig      Removed the NOMDI #define - I'm
**                    making us MDI!
**  07/08/1992   version.h     randy      added EMENU (for Windows)
**  06/22/1992   version.h      SKM       moved TEXTMETRIC from dlgutpro.h
**  07/02/1992   version.h      SKM       added EPSTR
**  07/22/1992   version.h     randy      full Enigma merge completed
**  08/17/1992   version.h     randy      added PUBLIC definition (for
**                    future use; intended (like PRIVATE) to be used BEFORE
**                    the return type, unlike GLOBAL which is between the
**                    return type and the function name.
**  08/19/1992   version.h     randy      added LPVOID
**  08/28/1992   version.h     randy      added ECCSTR
**  09/09/1992   version.h     randy      LECSTR --> LPECSTR
**  10/08/1992   version.h     randy      added LPCERECT
**  11/13/1992   version.h     randy      LPVOID not needed on Windows side
**                    (already in WINDOWS.H)
**  12/01/1992   version.h		SKM	      moved DevicePixInchX & DevicePixInchY
**					  from HARDROTS.C to VERSION.H
**  12/01/1992   version.h		SKM	      removed #define DEVELOP on Mac (this is now
**					  defined in the Think C Project.
**  12/14/1992   version.h     randy      incremented version numbers to
**                    3.0b16
**  12/29/1992   version.h     randy      incremented version numbers to
**                    3.0b17
**  01/18/1992   version.h      SKM       added EFLOAT & EVPU typedefs
**  01/25/1993   version.h     randy      incremented version numbers to
**                    3.0b18
**  01/25/1993   version.h     randy      incremented version numbers to
**                    3.0b19
**  02/05/1993   version.h     randy      incremented version numbers to
**                    3.0b20
**  02/10/1993   version.h		SKM	      moved DevicePixInchX & DevicePixInchY
**					  to HARDSPEC.H
**  02/19/1993   version.h     randy      incremented version numbers to
**                    3.0b21
**  02/19/1993   version.h     randy      changed HIBYTE (et al.) to
**                    HI_UONEBYTE (et al.) and added signed versions of
**                    these macros
**  03/03/1993   version.h     randy      incremented version numbers to
**                    3.0b22
**  03/24/1993   version.h     randy      incremented version numbers to
**                    3.0b23
**  03/26/1993   version.h     randy      incremented version numbers to
**                    3.0b24
**  04/02/1993   version.h     Alan       Added ROUND macro.
**  04/13/1993   version.h     randy      added HUNIT, HFLOAT
**  04/15/1993   version.h     Alan       Added SIGN macro, changed ROUND macro.
**  04/16/1993   version.h     randy      changed ROUND() macro back to
**                    what it was before. The 'floor()' approach had its
**                    advantages (the expression was only evaluated once),
**                    but required including MATH.H even if it was not
**                    needed, and sometimes it was getting included twice,
**                    which caused errors. We don't know if casting to
**                    fourbyte is more or less efficient than calling
**                    floor().
**  04/20/1993   version.h     randy      incremented version numbers to
**                    3.0b25
**  04/20/1993   version.h     randy      incremented version numbers to
**                    3.0b26
**  04/29/1993   version.h     randy      Mac version only set to version
**                    3.0.0 release 0
**  05/17/1993   version.h     Alan       Advanced version numbers on Mac
**                    for 3.0.0 release 2 (EFILE_VERSION left at release 0).
**  05/25/1993   version.h     Alan       Advanced version numbers on Mac
**                    for 3.0.0 release 3 (EFILE_VERSION left at release 0).
**  05/27/1993   version.h     Alan       Advanced version numbers on Mac
**                    for 3.0.0 release 4 (EFILE_VERSION left at release 0).
**  05/27/1993   version.h     Alan       Advanced version numbers on Mac
**                    for 3.0.0 release 5, including EFILE_VERSION.
**  05/27/1993   version.h     Alan       Advanced version numbers on Mac
**                    for 3.0.0 release 6 (EFILE_VERSION left at release 5).
**  06/02/1993   version.h     randy      changed WINVER to 0x030a (3.1
**                    only)
**  06/03/1993   version.h     randy      moved "limits" stuff (TWOBYTE_MAX
**                    et al) to new ELIMITS.H, since it is not used
**                    frequently. Might speed compiles a bit, and will shut
**                    up PC-Lint on the subject.
**  06/03/1993   version.h     randy      NULLFP now defined as NULL for
**                    Visual C++ (Windows only)
**  06/08/1993   version.h     randy      moved *_VERSION defines to
**                    EVERSION.H, since only a few modules rely on them
**  06/14/1993   version.h     randy      changed things like _far to
**                    __far; EXPORT now includes __export again
**  06/15/1993   version.h     randy      changed double underscores back
**                    to single (MSC 6.0a only supports single underscore;
**					  until we scrap it entirely, we have to stick to that)
**  07/30/93     version.h      RES       added JR_FINALE definition
**  08/20/93     version.h     Alan       OPERATING_SYSTEM will be WINDOWS
**                    by default.  MAC_OS is defined in the Think C prefix.
**  10/05/93     version.h      RES       removed NODEFERWINDOWPOS (Windows)
**  10/28/93     version.h      RES       LOCALHANDLE --> HLOCAL (for Win32)
**  10/28/93     version.h      RES       GLOBALHANDLE --> HGLOBAL (for Win 32)
**  11/02/93     version.h      RES       removed offsetof() definition. This
**						is ANSI standard, so include <stddef.h> where needed.
**	11/18/93	 version.h		KO		  reinstated Hook and Help function
**						prototypes Windows.H
**  12/10/93     version.h      RES       added LARGE_MODEL
**  12/16/93     version.h      RES       Redefined NULLP and FNULLP as NULL.
**						The (void *)0 definition was causing problems when
**						compiled under C++. (Windows only for now.)
**  12/17/93     version.h      Alan      Added wide character types and macros.
**                     Added ECHARSET type.
**  03/14/94     version.h      RES       DEVELOP is no longer defined here for
**						Windows; instead, define it in the environment and
**						COMPOPT.MAK will define it for the compiler
**  03/23/94     version.h      SKM       added some typedefs/defines for THINK_C,
**  					to allow it to compile changes needed for the PowerPC.
**  04/14/94     version.h      RES       removed #undef OEMRESOURCE
**  06/10/94     version.h      RES       experimenting again with __fastcall.
**						Trying LOCAL first; if it works, we'll try GLOBAL too.
**  06/10/94     version.h      RES       as long as I'm in here, converted
**						single underscores to double underscores
**  06/11/94     version.h      RES       LOCAL __fastcall seems to work; trying
**						GLOBAL __fastcall
**  06/11/94     version.h      RES       GLOBAL is only defined to use __fastcall
**						if DEVELOP is not defined. This is because of an unexplained
**						linker error you get if you define far __fastcalls with
**						stack checking enabled.
**  06/17/1994   version.h     randy      Redefined REGISTER as "register"
**                    for Windows; for a long time I had been under the
**                    impression it was better to let the compiler choose
**                    registers; now I find that the only time it is better
**                    is with /Oe (enable global register allocation), which
**                    we can't use because it generated some bad code in
**                    GetClosestIMT() (never confirmed: see COMPOPT.MAK).
**                    Enabling "register" makes no detectable difference in
**                    performance (although it did flush out a few spots
**                    where we were taking addresses of REGISTER vars), but
**                    it gives us one more tool for tweaking in the future.
**  09/30/94     version.h      RES       removed SEGMENT (support for Micro Quill
**						Segmenter utility)
**  09/30/1994   version.h     randy      Removed a bunch of #undef's. I was
**                    concerned that they could be masking problems with
**                    other libraries or include files.
**  10/01/94     version.h      RES       added CEXPORT
**  10/04/94     version.h      RES       broke OPERATING_SYSTEM, APPLICATION,
**						and related defines into APPINFO.H. This allows us to
**						dispense with the VERSION_ONLY mechanism.
**  11/18/94     version.h      RES       changed MAX_PATHNAME_LENGTH for Windows.
**						There is some confusion in documentation concerning the
**						correct value; I was having trouble w/ GetTempFileName()
**						passing in an 80 char buffer; it is documented as requiring
**						at least 144. So I increased MAX_PATHNAME_LENGTH to 150.
**  01/05/95     version.h      Alan      Restored #undef LOCAL for Mac side to
**						hack around a conflict with a CTree #define for LOCAL.
**						Changed NULLP from a void pointer to just NULL to get
**						around a Symantec C++ compile complaint (Mac).
**  01/06/95     version.h      Alan      Changed Mac definition of finmsg to get
**						#pragma finmsg() statements to work with Symantec C++.
**  01/10/95     version.h      Alan      Added BOOL type for the Mac.
**  01/12/95     version.h      Alan      Changed usage of a THINK_C directive.
**	 01/12/95	  version.h		 Chris	   Obsoleted a section dealing with
**						Universal Includes.
**  01/18/95     version.h      Alan      Merged ETPOINT and ETOFFSET from Mac 3.2.1.
**  02/20/95     version.h      Alan      Removed WIDE_CHAR_VERS.  EWCHAR is
**                     unconditionally 16 bits in size.
**  04/03/95     version.h      TNW       PPC-TNW Added MAKE_UPP, MAKE_GLOBAL_UPP.
**  04/04/95     version.h      TNW       Include type info in MAKE_UPP, MAKE_GLOBAL_UPP.
** 					Added DECLARE_UPP. MAKE_UPP, MAKE_GLOBAL_UPP, DECLARE_UPP do nothing
** 					for non-mac platforms.
**  04/10/95     version.h      TNW       Added EXTCONST, to account for the fact that
**						under C++, consts are static by default.
**  05/09/95     version.h      RES       added EHCOLOR for Windows
**  05/11/95     version.h      RES       EHCOLOR --> EGRAY_VALUE, which is
**						much more correct
**  05/11/95     version.h      RES       moved EGRAY_VALUE to ECOLOR.H
**  05/20/95     version.h      RES       removed LARGE_MODEL (we will always
**						be large model from now on)
**  06/11/95     version.h      RES       FNULLP --> NULLP
**  07/31/95     version.h      CFC       Formalism: char --> signed char, which
**                     is the assumption we have been making up to now.
**                     Also added some experimental stuff that I was working
**                     on to get compiling under Borland C++ 4.5.
**  07/31/95     version.h      CFC       More than a formalism.  I really
**                     don't have time to deal with all the Mac compile
**                     errors. So I will leave it at char.
**  08/09/95     version.h      RES       added EUCHAR kludge
**  08/12/95     version.h      RES       removed UNDO_SUBSYS. There is no point
**						in expending the effort to maintain code we will never
**						return to (that is, we won't abandon undo).
**  11/14/95     version.h      RES       started added WIN32 defs
**  11/15/95     version.h      RES       removed EDIT_COMM (no longer used)
**  11/15/95     version.h      RES       removed NFILE_SUBSYS, VNOTE_SUBSYS
**						(outdated)
**  11/15/95     version.h      RES       removed WINVER def (defaults to 3.1
**						in Win16 anyway, and our definition confused WIN32)
**  11/15/95     version.h      RES       removed EFIX_STROKE_KLUDGE
**  11/16/95     version.h      RES       removed PEHRECT (redundant)
**  11/23/95     version.h      RES       added win16.h, win32.h, windowsx.h
**  11/28/95     version.h      RES       added macos.h for Mac
**  12/07/1995   version.h     RES        _CDECL --> CDECL
**  01/29/96     version.h      RES       lint asked me to parenthesize macros
**						(INIT())
**  02/05/96     version.h      TNW       removed NOMETAFILE define so that
**						metafile API's are defined in windows.h
**  03/06/96     version.h      RES       modified MAX_PATHNAME_LENGTH et al for
**						Win32 long file names
**  03/13/96     version.h      ALE       Removed #define NOCLIPBOARD (edtxtblk.c
**                     needs clipboard stuff in Windows.h).
**  04/09/96     VERSION.H      RES       because FARMEM is also defined in TIFFCOMP.H,
**						we need to undef it here first to remove order dependencies
**						amongst the include files
**  07/01/96     version.h      CFC       removed #define NOSOUND
**                     for experiment for internal speaker click.
**  08/28/96     VERSION.H      RES       added missing parentheses in EHPOINT_X/Y
**						macros
**  10/12/96     version.h      RES       added SEVPU type
**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  12/31/96     version.h      TNW       moved macos.h to end, moved UINT from findefs to here.
**  01/02/97     VERSION.H      RES       moved macos.h inside OPERATING_SYSTEM == MAC_OS
**						block
**  03/01/97     version.h      RES       removed LPECSTR (same as ECSTR because
**						pointers are always FAR); LPEWSTR (same as EWSTR)
**  03/14/97     VERSION.H      RES       WIN32 --> _WIN32
**  03/27/97     VERSION.H      RES       removed CEXPORT, _EXPORT
**  03/27/97     VERSION.H      RES       added DLLEXPORT
**  04/08/97     VERSION.H      RES       added _MFC_VER (experimental) for
**						building MFC version
**  04/17/97     VERSION.H      TNW       added ERSRCFREF
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/04/97     VERSION.H      RES       removed useless FARMEM-based typedefs;
**						removed FAR for Mac (what is it needed for?); removed
**						some other unused declarations
**  11/05/97     VERSION.H      RES       changed finmsg() wording for windows
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  12/01/97     VERSION.H      RES       use _MAX_PATH, _MAX_FNAME in Win32
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  03/13/98     VERSION.H      RES       added SHUNIT
**  04/24/98     VERSION.H      RES       always include finassrt.h
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  07/30/98     VERSION.H      CFC       added EHFLOAT, I wanted to use EFLOAT but that
**						was already used to mean EVPU float, and I wanted something
**						that simply meant hardware type for float.  This apparently
**						is like the same naming problem we must have run into with
**						ERECT and EHRECT
**  08/10/98     VERSION.H      RES       added test for simultaneous definition
**						of DEBUG/NDEBUG or _DEBUG/NDEBUG
**  08/26/98     VERSION.H      RES       NOKANJI no longer does anything
**  10/20/98	 version.h	    evs		  Made MAKE_EHPOINT macro Mac only.  See note below.
**  10/28/98	 version.h	    CFC		  added EFRECT
**  11/06/98     VERSION.H      RES       define CONST as const for Metro Werks
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/17/98     VERSION.H      CFC       fixed pragma finmsg for Mac, this will eventually
**						work in MWCW Pro 4
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
**  01/13/99     VERSION.H      CFC       EFIXPOINT, moved typedef for EFIX and ESFIX to here
**						from EFIX.H
**  01/14/99     VERSION.H      CFC       moved typedef for FPOINT to here from pathprot.h
**  01/14/99     VERSION.H      RES       changed a bunch of structs from typedefs
**						to named structs
**  01/26/99     VERSION.H      CFC       adding ECMDID (Enigma Command ID)
**  05/17/99     Version.h      RES       remove NEARMEM, FARMEM, HUGEMEM
*//*~*/
/*
$Log: VERSION.H,v $
Revision 1.1.1.1  2002/12/19 19:13:13  dfober
first import

** 
** 20    8/31/99 12:35p Rstokes
** #ifndef STRICT (see comments)
** 
** 19    8/31/99 12:22p Rstokes
** changed #include <> to #include "" in one case pointed out by Robert
** Patterson that causes problems when compiling Win32 targets in
** CodeWarrior.
** 
** 18    8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 17    7/28/99 12:18 PM ccianflo
** comments, added DUMMY
** 
** 16    7/22/99 3:09 PM ccianflo
** moved HFIX type here from Mac HARDROTS.CPP
** 
** 15    7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 14    6/15/99 5:44p Rstokes
** moved finassert.h include above macos.h
** 
** 13    6/03/99 2:57p Rstokes
** added egraf.h
** moved EHPOINT, EHRECT, EPOINT, ERECT, EFRECT, EFIXPOINT, FPOINT,
** ETOFFSET, ETPOINT and supporting declarations from version.h to new
** egraf.h
** removed EFARPROC, ENEARPROC
** 
** 12    5/25/99 12:37p Rstokes
** Windows: If we set __stdcall or __fastcall in the project, we don't
** have to remember to use LOCAL/GLOBAL anymore (actually, LOCAL has
** served little purpose since we dropped Win16 anyway). LOCAL/GLOBAL are
** now defined as nothing for Windows, and the appropriate calling
** convention is set in the projects.
** 
** 11    5/18/99 1:21p Rstokes
** removed NEARMEM, FARMEM, HUGEMEM
** 
** 10    3/29/99 2:32p Rstokes
** changed FINEXPORT back to EXPORT in comments (for historical accuracy
** and tracking)
** 
** 9     3/26/99 4:54 PM Tfischer
** Make plug-ins compilable in Fin2000.  EXPORT->FINEXPORT;  other minor
** related changes
** 
** 8     2/04/99 3:53p Beth
** Added MFC support.
 * 
 * 7     1/28/99 2:42p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _VERSION_H_
#define _VERSION_H_

#ifndef _APPINFO_H_
#include "appinfo.h"	// defines OPERATING_SYSTEM and related values
#endif

//-----------------------------------------------------------------------------
#if OPERATING_SYSTEM == WINDOWS
// 8/10/98 RES: As we worked on setting up an IDE-based project, I ran into a
// situation where it appears the IDE was defining both DEBUG and NDEBUG for
// the NCP Release build. Although it did not show up in the Project Settings
// dialog, I was getting weird warnings like "__fastcall is inefficient with
// stack probes". Further investigation into Finale.dsp hinted at a conflicting
// definition, which I corrected manually. I added this test here to be sure
// the situation does not arise again.
#if defined(DEBUG) && defined(NDEBUG)
#error both DEBUG and NDEBUG defined!
#endif
#if defined(_DEBUG) && defined(NDEBUG)
#error both _DEBUG and NDEBUG defined!
#endif

#ifdef USE_MFC

// EVS 2/4/99 - I have turned all of these off for the MFC compile.  It makes me
// just a bit nervous to define these without knowing the effect on MFC.
//#define   NOCOMM
//#define	NOATOM
// EVS - 2/4/99 - Include the standard MFC and Windows files.
#include "stdafx.h"
						 
/* supress min() and max() in windows.h
(because they are defined in stdlib.h): */
//#define NOMINMAX


// This has been replaced with VC_EXTRALEAN, which AppWizard defined in stdafx.h.
//#define WIN32_LEAN_AND_MEAN		// See MSVC 5.0 windows.h (WIN32 never gets
									// defined if this is used!!)
									// 3/14/97 RES: Trying it again; this time we're
									// looking for _WIN32, and defining WIN32 ourselves
									// internally

// Taken care of by MFC - 
//#ifdef	NEED_SHELLAPI
//#include <shellapi.h>		// must be included before windowsx.h
//#endif
// Moved to stdafx.h - 
//#include <windowsx.h>

#else   // NON-MFC Defines ---------------- #ifndef USE_MFC

/* these defines will skip several hundred lines of windows.h: */

// 8/26/98 RES: NOKANJI no longer does anything
//#define NOKANJI
//#define NOMETAFILE
//#define NOSOUND
#define NOCOMM
#define	NOATOM
//#define	NOCLIPBOARD
//#define NOWH
//#define NOHELP
//#undef OEMRESOURCE
//#define	NODEFERWINDOWPOS

// These seem to cause other problems I did not have time to track down:
//#define NOSERVICE				// See MSVC 2.0 windows.h
//#define NOMCX          		// See MSVC 2.0 windows.h
//#define NOIME           		// See MSVC 2.0 windows.h

/* supress min() and max() in windows.h
(because they are defined in stdlib.h): */

#define NOMINMAX

// 8/30/99 RES: STRICT is reportedly already defined when compiling Win32 target
// in CodeWarrior
#ifndef STRICT
#define STRICT
#endif

#define WIN32_LEAN_AND_MEAN		// See MSVC 5.0 windows.h (WIN32 never gets
								// defined if this is used!!)
								// 3/14/97 RES: Trying it again; this time we're
								// looking for _WIN32, and defining WIN32 ourselves
								// internally

#if (_MSC_VER >= 700)
#pragma warning( disable:4001 4135 4018 4706 4010 4127 )
#endif

#include <windows.h>

#ifdef	NEED_SHELLAPI
#include <shellapi.h>		// must be included before windowsx.h
#endif

// 11/22/95 RES: it is easier to include windowsx.h here than in every file
// where it's needed. This was exacerbated by the need to add win16.h
// in some places (see comments in win16.h for details). A few trial runs
// suggest that it doesn't add much overhead if any to the compile time to include
// it here.
#include <windowsx.h>

/* no need to clutter the symbol table with these: */
#undef  NOKANJI
#undef  NOMETAFILE
#undef  NOSOUND
#undef  NOWH
#undef  NOCOMM
#undef  NOHELP
#undef 	NOATOM
#undef 	NOMDI
#undef 	NOCLIPBOARD
#undef 	NODEFERWINDOWPOS
#undef  NOMINMAX

#endif //  #ifdef USE_MFC


// The following includes and defines are for both an MFC and a non-MFC build
#include <stdlib.h>

/*
** DISPLAY_POSTSCRIPT controls the inclusion of code to use the operating
**	system to draw in display postscript on the screen (NeXT machine).
**	IBM and Mac should define it as 0.
*/
#define	DISPLAY_POSTSCRIPT	0

/*
** miscellaneous
*/

// the following allows use to use #pragma message() or a specific
// compilers variation on it, without causing other compilers to barf.
// SYNTAX - #pragma finmsg( "message goes here." )
//#define finmsg(s) message(__FILE__" -- warning: " s )
// 11/5/97 RES: changed "warning" to "message"; this will make it easier and
// faster to filter out messages: the old "warning:" would have to be scanned
// all the way to the colon to distinguish it from warnings issued by the compiler.
#define finmsg(s) message(__FILE__" -- message: " s )

// (6/15/94 RES): A note about __fastcall: MSDN 7 CD-ROM says "be sure to use
// standard include files or you will get link errors", but there is no hint
// as to which files to include, and "aFrchkstk" cannot be found in any MSVC
// includes. No problem if stack checking is off. This is why we use __fastcall
// only for release, when stack checking is off.
// (6/15/94 RES): _aFrchkstk is in LLIBCE.LIB, but not in LLIBCEW.LIB (which we
// use). Theoretically, we could extract the module "frchkstk" from LLIBCE.LIB.
#undef	FARMEM			// 4/8/96 RES: see TIFFCOMP.H

/////////////////////////////////////////////////////////////////////////////
// 32Bit Windows includes and defines
/////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

// 3/14/97 RES: It is not at all clear to me when WIN32 is defined, and when
// _WIN32 is defined! Compiler docu mentions _WIN32, but not WIN32 at all. Yet
// it is usually defined by the time we get here...
// 3/20/97 RES: We define it ourselves on the command line! (compopt.mak) But
// it still would not be defined if called from the resource compiler.
#ifndef	WIN32
#define	WIN32
#endif

#include "win32.h"		// see comments in win16.h for explanation (11/22/95 RES)

//#define	FARMEM
//#define	NEARMEM
//#define	HUGEMEM

#define STDCALL			__stdcall

//#define	EXPORT		__stdcall
// EVS 2/4/99 - EXPORT has been redefined by Windows and MFC.  To make it clearer, I have
// added a new define FINEXPORT which can be used in our code so we have better control over our
// function declarations.
#define FINEXPORT		CALLBACK
#define	DLLEXPORT		__declspec(dllexport) __stdcall


#else 

/////////////////////////////////////////////////////////////////////////////
// 16Bit Windows includes and defines
/////////////////////////////////////////////////////////////////////////////

#undef	WIN32

#include "win16.h"		// see comments in win16.h for explanation (11/22/95 RES)

//#define	FARMEM		__far
//#define	NEARMEM		__near
//#define	HUGEMEM		__huge

#define STDCALL		__pascal

// EVS 2/4/99 - Changed EXPORT to FINEXPORT
#define	FINEXPORT		__export __far __pascal
#define	DLLEXPORT		__export __far __pascal

#endif		 		  // Win32 vs. Win16 includes and defines

#ifdef __BORLANDC__

// experimental, is that the correct predefined macro to use: __BORLANDC__
#define LOCAL		NEAR PASCAL
#define GLOBAL		FAR PASCAL

#else

//#ifdef DEVELOP
//#define LOCAL		/*NEARMEM*/ STDCALL
//#define GLOBAL		/*FARMEM*/ STDCALL
//#else
//#define LOCAL		/*NEARMEM*/ __fastcall
//#define GLOBAL		/*FARMEM*/ __fastcall
//#endif    // ifdef DEVELOP

// 5/24/99 RES: If we set __stdcall or __fastcall in the project, we don't have
// to remember to use LOCAL/GLOBAL anymore (actually, LOCAL has served little
// purpose since we dropped Win16 anyway).

#define LOCAL
#define GLOBAL

#endif	  // ifdef __BORLANDC__

/* more defines for the Windows/MSDOS environment: */

#ifdef _WIN32
const int MAX_FILENAME_LENGTH = _MAX_FNAME;	/* (256) filename + '.' + ext + NULL */
const int MAX_PATH_LENGTH	  = _MAX_PATH;	/* (260) including drive, path, NULL */
const int MAX_PATHNAME_LENGTH = MAX_PATH_LENGTH;
#else
const int MAX_FILENAME_LENGTH = 13;		/* filename + '.' + ext + NULL */
const int MAX_PATH_LENGTH	  = 137;	/* 135 plus 2 for drive and 1 for NULL */
const int MAX_PATHNAME_LENGTH = (MAX_PATH_LENGTH+MAX_FILENAME_LENGTH);
#endif  // #ifdef _WIN32

/////////////////////////////////////////////////////////////////////////////
// Type definitions ----
/////////////////////////////////////////////////////////////////////////////
#define TRUE        1
#define FALSE       0

#define NULLP       NULL
#define NULLFP      NULL			// for Visual C++ (null function pointer)

#define STATIC      static
#define PRIVATE	    static
#define	PUBLIC
#define VOID		void
#define REGISTER	register
#define	CONST		const

//#ifndef CDECL
#undef CDECL
#define CDECL		__cdecl
//#endif

typedef char onebyte;
typedef unsigned char uonebyte;
typedef short twobyte;
typedef unsigned short utwobyte;
typedef long fourbyte;
typedef unsigned long ufourbyte;
typedef float EHFLOAT;
typedef long tTIME;				/* time, units unspecified (TIME_EDU or TIME_RTU) */

// 8/9/95 RES: char is by default signed, so ECHAR ends up signed. This is causing
// us various sign-extension problems (most notably when assignment a high character
// from an ECHAR to a twobyte. I'd like to examine why ECHAR is signed and whether
// it needs to be, but we are too close to shipping 3.5 to do that. For now, I'm
// adding EUCHAR as a clumsy way of casting ECHARs to prevent sign extension.
// It would be much better if we could make ECHAR unsigned.

typedef char ECHAR;
typedef unsigned char EUCHAR;

// Although it seems like a good idea, making tbool an enum causes problems
// under C++. The worst is that a boolean cannot be assigned to a tbool,
// like this: a = (c == 1)
//typedef enum
//{
//	NO = 0,
//	YES = 1
//} tbool;			   			/* boolean */

typedef twobyte tbool;			/* boolean */

/* hardware Enigma structures, etc: */

//typedef POINT			EHPOINT;
//typedef RECT			EHRECT, *LPEHRECT;

typedef HCURSOR			ECURSOR;
typedef	HBITMAP			EBITMAP;
typedef HDC				EMEMDC;
typedef HDC				EDC;
typedef HRGN			ERGN;
typedef HWND			EWND, ECNTL;
typedef HWND			EDLG;
typedef HWND			ESCROLLBAR;
typedef	int				ECNTLID;
typedef	int				ERSRCID;
typedef	ERSRCID			ESTRID;
typedef HMENU			EMENU;
typedef	int				EMENUID;
typedef EMENUID			ECMDID;	// On Windows an ECMDID will be identical to an EMENUID.
typedef LONG			ELONG;
typedef WORD			EWORD;
typedef DWORD			EDWORD;

//typedef FARPROC		EFARPROC;
//typedef NEARPROC		ENEARPROC;

typedef HINSTANCE		ERSRCFREF; //Used to locate resources. Windows instance handle.

#endif		// OPERATING_SYSTEM == WINDOWS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if OPERATING_SYSTEM == MAC_OS

/*
** DISPLAY_POSTSCRIPT controls the inclusion of code to use the operating
**	system to draw in display postscript on the screen (NeXT machine).
**	IBM and Mac should define it as 0.
*/

#define	DISPLAY_POSTSCRIPT	0

/*
** miscellaneous
*/

#include <stdlib.h>
//#include <limits.h>

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

// the following allows use to use #pragma message() or a specific
// compilers variation on it, without causing other compilers to barf.
// SYNTAX - #pragma finmsg( "message goes here." )
//#define finmsg(str)		message __FILE__ " " str
// CFC 12/17/98: Adding the parentheses turned these into true recognizable
// pragmas now.
#define finmsg(str)		message( __FILE__ " " str )


#ifdef	LOCAL
#undef	LOCAL
#endif
#define LOCAL

//#ifdef	PRIVATE
//#undef	PRIVATE
//#endif
#define PRIVATE		static

//#ifdef	GLOBAL
//#undef	GLOBAL
//#endif
#define GLOBAL

//#ifdef	PUBLIC
//#undef	PUBLIC
//#endif
#define	PUBLIC

//#ifdef	VOID
//#undef	VOID
//#endif
#define VOID		void

//#define LARGE_MODEL
//#undef	FARMEM			// 4/8/96 RES: see TIFFCOMP.H
//#define	FARMEM
//#define	NEARMEM
//#define	HUGEMEM	 

// EVS 2/4/99 - Changed EXPORT to FINEXPORT.
#define	FINEXPORT

//#define	CEXPORT
// 11/4/97 RES: why is FAR needed for Mac?
//#define	FAR

#define REGISTER

//#define NULLP		((void *)0)
//#define FNULLP		0L		// ((void FAR *)0)
//#define NULLFP		NULL
#define NULLP       NULL
//#define FNULLP      NULL
#define NULLFP      NULL			// for Mac Symantec C++ (null function pointer)

//#ifdef	STATIC
//#undef	STATIC
//#endif
#define STATIC		static

#ifdef __mwerks__
//#define CONST				// metro works doesn't like some of our usages of CONST
//							// so for now I will undefine it.
// 11/6/98 RES: Whoever made the above unsigned(!) undated(!) comment did not specify
// the version of metro werks or what the problem was! I'm using v. 2.1 of the IDE
// (I think the compiler is numbered differently but I don't know how to get
// version info on that) and had no problems compiling, so I'm going ahead and
// defining CONST as const again. I vaguely recall Tim W. saying something about
// problems with "const char * const" type constructions (multiple const); I'm
// going to assume it wasn't a runtime issue or bad-code generation problem...
#define CONST		const
#else
#define CONST		const
#endif

#define CDECL

typedef char onebyte;
typedef unsigned char uonebyte;
typedef short twobyte;
typedef unsigned short utwobyte;
typedef long fourbyte;
typedef unsigned long ufourbyte;
typedef float EHFLOAT;
typedef long tTIME;				/* time, units unspecified (TIME_EDU or
								TIME_RTU) */

// 8/9/95 RES: char is by default signed, so ECHAR ends up signed. This is causing
// us various sign-extension problems (most notably when assignment a high character
// from an ECHAR to a twobyte. I'd like to examine why ECHAR is signed and whether
// it needs to be, but we are too close to shipping 3.5 to do that. For now, I'm
// adding EUCHAR as a clumsy way of casting ECHARs to prevent sign extension.
// It would be much better if we could make ECHAR unsigned.

typedef char ECHAR;
typedef unsigned char EUCHAR;

typedef unsigned char EPCHAR;
typedef EPCHAR *EPSTR;					/* Pascal string types */
// Although it seems like a good idea, making tbool an enum causes problems
// under C++. The worst is that a boolean cannot be assigned to a tbool,
// like this: a = (c == 1)
//typedef enum
//{
//	NO = 0,
//	YES = 1
//} tbool;							/* boolean */
typedef twobyte tbool;			/* boolean */
typedef tbool BOOL;			/* boolean */

typedef	void /*FARMEM*/ *LPVOID;

typedef char*			PSTR;
typedef PSTR			LPSTR;
typedef char**			HANDLE;
typedef HANDLE			HLOCAL;
typedef HANDLE			HGLOBAL;

/* more defines for the Mac enivronment: */

#define	MAX_FILENAME_LENGTH		32	/* filename + NULL */
#define	MAX_PATH_LENGTH			224	/* pathname (excluding filename + NULL) */
#define	MAX_PATHNAME_LENGTH		(MAX_PATH_LENGTH+MAX_FILENAME_LENGTH)

/* Windows OS built-in types, needed by Mac */

// Removed CTREE_HEADERS_INCLUDED 12/14/95 RES. I don't think it is necessary any more.
//#ifndef CTREE_HEADERS_INCLUDED
typedef unsigned		UINT;
//#endif

typedef long		WPARAM;
typedef long		LPARAM;
typedef long		LRESULT;

/* other Enigma structures, etc: */

typedef unsigned short		EWORD;

typedef unsigned long		EDWORD;
typedef long		ELONG;


typedef short		ECNTLID;
typedef long		EMENUID;
//#ifdef PowerPlant
//typedef ECMDID CommandT;	// This is what ECMDID will become in PowerPlant.
//#else
typedef SInt32 		ECMDID;
//#endif
typedef short		ERSRCID;
typedef long		ESTRID;		// string resource id ( on Mac...if high word
								// is non-zero the high word is STR# resource id
								// and low word is index; else if high word is
								// zero, then low word is STR resource id.

//typedef Point		EHPOINT;
//typedef Rect		EHRECT, *LPEHRECT;
typedef RgnHandle	ERGN;

typedef short 		ERSRCFREF; //Used to locate resources (passed to FinaleModalDialog). Mac resource file

struct EBITMAPHEADER
{
	BitMap	bm;
	tbool	ourBits; // if YES the we allocated the bits
};
typedef EBITMAPHEADER **EBITMAP;

struct TEXTMETRIC		// added for portabilty
{
    twobyte     tmHeight;
    twobyte     tmAscent;
    twobyte     tmDescent;
    //twobyte     tmInternalLeading;
    twobyte     tmExternalLeading;
    //twobyte     tmAveCharWidth;
    //twobyte     tmMaxCharWidth;
    //twobyte     tmWeight;
    //BYTE    tmItalic;
    //BYTE    tmUnderlined;
    //BYTE    tmStruckOut;
    //BYTE    tmFirstChar;
    //BYTE    tmLastChar;
    //BYTE    tmDefaultChar;
    //BYTE    tmBreakChar;
    //BYTE    tmPitchAndFamily;
    //BYTE    tmCharSet;
    //twobyte   tmOverhang;
    //twobyte   tmDigitizedAspectX;
    //twobyte   tmDigitizedAspectY;

};

typedef GrafPort		EDCSTRUCT, *EDC;

typedef WindowPtr		EWND;
typedef ControlHandle	ECNTL;

typedef MenuHandle		EMENU;
//typedef TEHandle		ETEH;

//typedef ProcPtr		EFARPROC;
//typedef ProcPtr		ENEARPROC;
//typedef ProcPtr		WNDPROC;

#define MACSCREENRES	72			// Macintosh screen resolution

#define	ScreenPixInchX	MACSCREENRES
#define	ScreenPixInchY	MACSCREENRES


#if 0
// CFC 01/12/95: Obsoleted because we don't really need this anymore.
// We know we are using Universal Includes, so there is no use for this.

// the following have been added to allow Think C 6.0 to compile
// changes needed for PowerPC version.
#ifdef THINK_C
#ifndef UNIVERSAL_INCLUDES

#define DisposeRoutineDescriptor(x)

// Intl0Hndl
// typedef ProcPtr ModalFilterProcPtr;
typedef ProcPtr UserItemProcPtr, ControlActionProcPtr, AEEventHandlerProcPtr;
typedef AEEventHandlerProcPtr AEEventHandlerUPP;
typedef ControlActionProcPtr ControlActionUPP;
typedef ProcPtr DlgHookUPP;
typedef UserItemProcPtr UserItemUPP;
typedef ModalFilterProcPtr ModalFilterUPP;
//typedef TimerProcPtr TimerUPP;
typedef pascal void (*TimerUPP)(void);

#define NewAEEventHandlerProc(x)	x
#define NewControlActionProc(x)	x
#define NewVBLProc(x)	x
#define NewDlgHookProc(x)	x
#define NewUserItemProc(x)	x
#define NewModalFilterProc(x)	x

#endif
#endif

#endif


#endif	// OPERATING_SYSTEM == MAC_OS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/* system-independent definitions: */

#ifdef	DEFINE_GLOBALS

#define	EXTERN
#define	INIT(x)	= (x)

#else

#define	EXTERN	extern
#define	INIT(x)

#endif

//Need extern for consts whether we are defining or declaring.
#define EXTCONST extern const

typedef fourbyte EVPU;			// long EVPU
typedef twobyte SEVPU;			// short EVPU
typedef fourbyte EFIX;			// 64ths of an EVPU
typedef twobyte ESFIX;			// 64ths of an EVPU

typedef fourbyte HUNIT;			// hardware unit
typedef twobyte SHUNIT;			// short hardware unit
typedef float EFLOAT;			// EVPU float
typedef float HFLOAT;			// hardware unit float
typedef HUNIT HFIX;				// 64ths of an HUNIT, or if you prefer, device pixels * 64.
								// This is like the EFIX and EVPU relationship.

typedef tTIME TIME_EDU;			/* time in Enigma Durational Units (EDUs) */
typedef tTIME TIME_RTU;			/* time in Real Time Units (RTUs) */
typedef twobyte ETAG;			/* Enigma tag */
typedef fourbyte ENTNUM;		/* Enigma entry number (signed so -1 can
									serve as invalid number) */
typedef	utwobyte CMPER;			/* comparator for others, details */
typedef utwobyte FLAG_16;		/* 16 bit flags */
typedef ufourbyte FLAG_32;		/* 32 bit flags */
typedef ECHAR *ECSTR /*, FARMEM *LPECSTR*/;	/* string types */
typedef CONST ECHAR * ECCSTR;

typedef FLAG_16 ECHARSET;		/* Character set / script */

typedef utwobyte EWCHAR;
//typedef EWCHAR *EWSTR /*, FARMEM *LPEWSTR*/;

//#define EFIX_STROKE_KLUDGE			/* temporary kludge to make postscript
//									output cleaner */

// values for tbool type:
#define YES         1
#define NO          0

/*
** these macros give byte of words offsets of a member within a structure
*/

#ifndef woffsetof
//#define woffsetof( type, memb ) ((unsigned)(((int  *)&((type *)0)->memb)-(int  *)0))
#define woffsetof( type, memb ) (offsetof( type, memb ) / sizeof(twobyte))
#endif

//#define wcharsizeof( a )		( sizeof( a ) / sizeof( EWCHAR ) )

#ifndef NULL
#define NULL	    0
#endif

// CFC 07/28/99: DUMMY can be used in those cases where you do not care
// what value is used.  It is intended to be used for readability or
// to make the meaning of code more clear.
// Examples:
// 	1. Parameter to a function is only used on one platform, so you
//		could do something like
//			foo( WINCODE( 1 ) MACCODE( DUMMY ) );
//	2. Local variable is initialized and it really does not matter
//		what it was initialized to, for example, to shut up Lint.
//		In that case you could simply do
//			twobyte temp = DUMMY;
//	3. Some parameters do not make sense in a given situation (overloaded
//		functions might be more approriate in these cases, but that is
//		a separate issue)
//			foo( YES, 0, 0, 0 );
//			but perhaps when foo() takes NO as parameter 1, the last two
//			parameters do not make sense, then you could write
//			foo( NO, 0, DUMMY, DUMMY );

#define DUMMY 0

/*
** Math macros, could be moved to emath.h
*/

#ifndef ABS
#define ABS(x)      (((x)<0) ? -(x) : (x))
#endif
#ifndef MAX
#define MAX(x,y)    (((x) < (y)) ? (y) : (x))
#endif
#ifndef MIN
#define MIN(x,y)    (((x) < (y)) ? (x) : (y))
#endif
#define	DIST(x,y)	((x) > (y) ? ((x) - (y)) : ((y) - (x)) )
/* round a signed float: */
#define ROUND(f)	((fourbyte)(((f) >= 0.0F) ? ((f) + 0.5F) : ((f) - 0.5F) ))
#define SIGN(x)     (((x)<0) ? -1L : 1L )

/*
** these macros dissect and assemble words and longs
*/

#define HI_TWOBYTE(l)		((twobyte)(((l) >> 16) & 0xFFFF))
#define LO_TWOBYTE(l)		((twobyte)((l) & 0xFFFF))

#define HI_UTWOBYTE(l)		((utwobyte)(((l) >> 16) & 0xFFFF))
#define LO_UTWOBYTE(l)		((utwobyte)((l) & 0xFFFF))

#define HI_ONEBYTE(w)		((onebyte)(((w) >> 8) & 0xFF))
#define LO_ONEBYTE(w)		((onebyte)((w) & 0xFF))

#define HI_UONEBYTE(w)		((uonebyte)(((w) >> 8) & 0xFF))
#define LO_UONEBYTE(w)		((uonebyte)((w) & 0xFF))

#define MAKE_TWOBYTE(lo,hi) 	((twobyte)(((uonebyte)(lo))|(((twobyte)((uonebyte)(hi)))<<8)))
#define MAKE_UTWOBYTE(lo,hi) 	((utwobyte)(((uonebyte)(lo))|(((utwobyte)((uonebyte)(hi)))<<8)))
#define MAKE_FOURBYTE(lo,hi)	((fourbyte)(((utwobyte)(lo))|(((fourbyte)((utwobyte)(hi)))<<16)))
#define MAKE_UFOURBYTE(lo,hi)	((ufourbyte)(((utwobyte)(lo))|(((ufourbyte)((utwobyte)(hi)))<<16)))

#define	MAKETAG(a,b)		((ETAG)((LO_UONEBYTE(a)<<8)|LO_UONEBYTE(b)))

//#define	MAKEBOOL(x)		(!!(x))		/* if 'x' is zero, macro returns FALSE
//									(or NO), else it returns TRUE (or YES) */
#define	MAKEBOOL(x)		((x) != 0 )	// 6/2/99 RES: much less clever than the
									// old one above, but possibly more efficient,
									// and certainly easier to understand!

//#define MAKE_WCHAR(c)	( (EWCHAR)((uonebyte)c) )	/* precast to unsigned to avoid
//														sign extension */

// 6/15/99 RES: moved here so it is included before macos.h
#ifndef _FINASSRT_H_
#include "finassrt.h"
#endif

/* Routines for building Universal ProcPtrs.
   The PowerPC toolbox routines need to know whether
	functions passed to them are 68K or PowerPC, so
	that the PPC tool box knows when to switch in
   and out of the 68K emulator when it calls the routine.

	For non-PPC Mac platforms, MAKE_UPP merely makes an
   alias of the function, functionUPP.

	For non-Mac platforms, MAKE_UPP does nothing.
*/

#if OPERATING_SYSTEM == MAC_OS

#if NEED_ROUTINE_DESCRIPTORS

#define MAKE_UPP(proc, type)\
 const RoutineDescriptor proc ## RD =\
 BUILD_ROUTINE_DESCRIPTOR(upp ## type ## ProcInfo, proc);\
 const type ## UPP proc ## UPP = (type ## UPP)&(proc ## RD);

#define MAKE_GLOBAL_UPP(proc, type)\
 extern const RoutineDescriptor proc ## RD =\
 BUILD_ROUTINE_DESCRIPTOR(upp ## type ## ProcInfo, proc);\
 extern const type ## UPP proc ## UPP = (type ## UPP)&(proc ## RD);

#else //!NEED_ROUTINE_DESCRIPTORS

#define MAKE_UPP(proc, type)\
 const type ## UPP proc ## UPP = (type ## UPP)proc;

#define MAKE_GLOBAL_UPP(proc, type)\
 extern const type ## UPP proc ## UPP = (type ## UPP)proc;

#endif

#define DECLARE_GLOBAL_UPP(proc, type) extern const type ## UPP proc ## UPP;

#ifndef _MACOS_H_
#include "macos.h"		// see comments in win16.h for explanation (11/22/95 RES)
#endif

#else //OPERATING_SYSTEM != MAC_OS

#define MAKE_UPP(proc, type)
#define MAKE_GLOBAL_UPP(proc, type)
#define DECLARE_GLOBAL_UPP(proc, type)

#endif

#endif

