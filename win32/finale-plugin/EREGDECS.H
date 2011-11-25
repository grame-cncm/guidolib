/*
Module Name:	EREGDECS.H
Module Number:	

Author:  	Tim Wright
Revision:	03/25/1993

Purpose:  Types for mass-mover type actions.


**  06/23/97	TNW	Separated from EREGION.H for benefit of Plugin EDK.
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: EREGDECS.H,v $
Revision 1.1.1.1  2002/12/19 19:13:11  dfober
first import

** 
** 4     8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 3     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
 * 
 * 2     1/28/99 2:42p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef	_EREGDECS_H_
#define	_EREGDECS_H_

#ifndef	_VERSION_H_
#include "version.h"
#endif

/*---------------------------------------------------------------*/

typedef struct
{
	twobyte IUlist;			/* the IU list we're using */
	twobyte IUslot;			/* the position in the IU list (we are storing
							this instead of instrument number (which we did
							pre-3.0) because of the overhead of processing
							instrument number) */
	twobyte meas;			/* the measure number */
	TIME_EDU duraPos;		/* the EDU position in the measure (0 == start
							of measure, FENCE == end of measure) */
} EMUSICPOINT;

/*---------------------------------------------------------------*/

/* an Enigma Region is defined by a starting point and an inclusive
ending point. */

typedef struct
{
	EMUSICPOINT start;
	EMUSICPOINT end;		/* the IUlist element is ignored, since it must
							match the IUlist element in 'start'. The end
							point in an EREGION is inclusive, including the
							'duraPos' element. */
} EREGION;

/*---------------------------------------------------------------*/

#endif

