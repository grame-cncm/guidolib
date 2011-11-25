/*
Module Name:	ELIMITS.H
Module Number:

Author:  	Randall Stokes
Revision:	06/03/1993

Purpose:


**  01/13/95     elimits.h      RES       moved MAX_CMPER from SHSELDLG.C to
**						ELIMITS.H
**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  08/21/98     elimits.h      CFC       added UONEBYTE_MAX
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: ELIMITS.H,v $
Revision 1.1.1.1  2002/12/19 19:13:11  dfober
first import

** 
** 5     8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 4     7/20/99 4:33p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 3     2/25/99 2:23p Rstokes
** added FENCE
 *
 * 2     1/28/99 2:41p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _ELIMITS_H_
#define	_ELIMITS_H_

#include <limits.h>

#define UONEBYTE_MAX	UCHAR_MAX
#define	TWOBYTE_MIN		SHRT_MIN
#define	TWOBYTE_MAX		SHRT_MAX
#define	UTWOBYTE_MAX	USHRT_MAX
#define	FOURBYTE_MIN	LONG_MIN
#define	FOURBYTE_MAX	LONG_MAX
#define	UFOURBYTE_MAX	ULONG_MAX

#define	MAX_CMPER		UTWOBYTE_MAX

#define FENCE			FOURBYTE_MAX		// The maximum possible time. Used as
											//	a terminator in TIME vars
											//	(such as ScrbBuffer[])

#endif	// _ELIMITS_H_

