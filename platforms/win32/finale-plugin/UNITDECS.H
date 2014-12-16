/*
Module Name:	UNITDECS.H
Module Number:	

Author:  	Randall Stokes  
Revision:	06/03/1990

Purpose:	symbolic constants for UnitsOfMeasurement


**  02/10/1992   unitdecs.h    randy      added UNIT_8THNOTES (this is used 
**                    in the shape designer when designing executable shapes)
**  04/04/95     unitdecs.h     RES       added UNIT_SPACES
**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: UNITDECS.H,v $
Revision 1.1.1.1  2002/12/19 19:13:13  dfober
first import

** 
** 4     8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 3     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
 * 
 * 2     1/28/99 2:43p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _UNITDECS_H_
#define _UNITDECS_H_

#define		UNIT_UNDEFINED	0
#define		UNIT_EVPUS		1
#define		UNIT_INCHES		2
#define		UNIT_CENTS		3
#define		UNIT_POINTS		4
#define		UNIT_PICAS		5
#define		UNIT_SPACES		6
#define		UNIT_8THNOTES	7

#define		UNIT_DEFAULT	0

#endif

