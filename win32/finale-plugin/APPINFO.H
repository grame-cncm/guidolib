/*
File Name:	APPINFO.H

Copyright 1994 Coda Music Technology (all rights reserved)

Author:  	Randy Stokes
Created:	10/04/94

Purpose:

This file was originally part of VERSION.H. I broke it out for two reasons:
(1) the "VERSION_ONLY" mechanism documented in VERSION.H was clumsy and
confusing, and (2) I needed version information for the resource compiler
to compile Allegro.

This file defines OPERATING_SYSTEM, APPLICATION, and related values for the
application.

Modification History:

**  10/04/94     appinfo.h      RES       JR_FINALE --> ALLEGRO
**  03/02/95     appinfo.h      RES       added RSHIFT_OK
**  04/03/95     appinfo.h      TNW       PPC-TNW Added PPC defines,
**						ENDIAN (used in place of PROCESSOR, to query byte order)
**  04/04/95     appinfo.h      TNW       Added NEED_ROUTINE_DESCRIPTORS.
**  05/19/95     appinfo.h      SKM       Added FINVIEWER & FINVIEWER_NAME.
**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  05/15/98     APPINFO.H      RES       cosmetic
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: APPINFO.H,v $
Revision 1.1.1.1  2002/12/19 19:13:10  dfober
first import

** 
** 6     8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 5     7/20/99 4:33p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 4     7/12/99 11:32a Rstokes
** added NOTATIONENGINE, INTRO, PRINTMUSIC, INTRO_NAME, PRINTMUSIC_NAME,
** NENGINE_NAME
** 
** 3     4/21/99 12:27p Rstokes
** removed obsolete and unused defines PETRUCCI, SONATA, and DEFAULT_FONT
 * 
 * 2     1/28/99 2:41p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _APPINFO_H_
#define _APPINFO_H_

/*
** operating system values
*/

#define MAC_OS		1			/* Macintosh operating system */
#define WINDOWS		2			/* Microsoft Windows (MS-DOS) */
#define NEXT_STEP	4			/* NextMachine */

/*
** hardware environments
*/

#define MACINTOSH	1			/* Apple Macintosh */
#define	IBM_PC		2			/* IBM PC line -- including PS/2 */
#define	NEXT		4			/* NextMachine */

/*
** processors -- watch out for byte order stuff
*/

#define CPU_80x86	1			/* Intel 8086 line */
#define CPU_68K		2			/* Motorola 68000 line */
#define CPU_PPC	  3			/* IBM/Motorola PPC line */


/*
** endians -- defines byte storage order.
*/

#define LITTLE_ENDIAN	1			/* integers stored LSB to MSB */
#define BIG_ENDIAN		2			/* integers stored MSB to LSB */

/*
** application identifiers
*/

#define	FINALE			1
#define	MUSIC_PROSE		2		// obsolete
#define	PRELUDE			3		// obsolete (never released)
#define	ALLEGRO			4
#define	FINVIEWER		5
#define	NOTATIONENGINE	6
#define	INTRO			7		// obsolete identifier for PRINTMUSIC
#define	PRINTMUSIC		7

// environment names

#define	WIN_NAME		"WIN"
#define	MAC_NAME		"MAC"

// application names

#define	FINALE_NAME		"FIN"
#define	ALLEGRO_NAME	"FJR"
#define	INTRO_NAME		"NTR"	// obsolete identifier for PRINTMUSIC
#define	PRINTMUSIC_NAME	"NTR"
#define	FINVIEWER_NAME  "FVW"
#define	NENGINE_NAME	"ENG"

/*
** These symbols define the operating environment
*/

// The Mac version currently defines OPERATING_SYSTEM in the Think C prefix
// box. Therefore, if OPERATING_SYSTEM is undefined it defaults to WINDOWS.

#ifndef OPERATING_SYSTEM
#define OPERATING_SYSTEM  WINDOWS
#endif

// Windows currently predefines APPLICATION (see COMPOPT.MAK)

#ifndef APPLICATION
#define APPLICATION       FINALE
#endif

#if APPLICATION == ALLEGRO
#define APP_NAME          ALLEGRO_NAME
#elif APPLICATION == FINVIEWER
#define APP_NAME          FINVIEWER_NAME
#else
#define APP_NAME          FINALE_NAME
#endif

/*
** These symbols define the environment for Finale Windows
*/

#if OPERATING_SYSTEM == WINDOWS

#define HARDWARE			IBM_PC
#define PROCESSOR			CPU_80x86
#define ENDIAN				LITTLE_ENDIAN
#define ENV_NAME			WIN_NAME
#define WINCODE(x)			x
#define MACCODE(x)
#define	RSHIFT_OK			1
// RSHIFT_OK is defined as 1 if the compiler will sign extend a signed number
// during a right shift, 0 if it fills with zeroes. Stroustrup says this
// behavior is implementation defined. I am making a change in fetch_tcd() that
// relies on it, however, so we have to get it right here. (3/2/95 RES)

#endif

/*
** These symbols define the environment for Finale Mac
*/

#if OPERATING_SYSTEM == MAC_OS

#define HARDWARE            MACINTOSH

// RSHIFT_OK is defined as 1 if the compiler will sign extend a signed number
// during a right shift, 0 if it fills with zeroes. Stroustrup says this
// behavior is implementation defined. I am making a change in fetch_tcd() that
// relies on it, however, so we have to get it right here. (3/2/95 RES)

#if (defined(powerc) || defined(__powerc))

#define PROCESSOR  			CPU_PPC
#define ENDIAN		 			BIG_ENDIAN
#define RSHIFT_OK				 1

#else

#define PROCESSOR           CPU_68K
#define ENDIAN		 	 		 BIG_ENDIAN
#define RSHIFT_OK				 1

#endif

#define ENV_NAME            MAC_NAME
#define WINCODE(x)
#define MACCODE(x)			x

#endif

#define POWER_PC  ((OPERATING_SYSTEM == MAC_OS) && (PROCESSOR==CPU_PPC))
#define NEED_ROUTINE_DESCRIPTORS POWER_PC


#endif // _APPINFO_H_
