/*
Module Name:	FINASSRT.H
Module Number:

Author:  	Randall Stokes
Revision:	05/03/1991

Purpose:


**  02/25/1992   finassrt.h    randy      added str to FinAssert() macro
**  06/18/1992   finassrt.h    randy      added FinaleUnderConstruction()
**  06/19/1992   finassrt.h    randy      onebyte/uonebyte --> ECHAR/ECSTR
**  10/25/93     finassrt.h     RES       redefined FinAssert() along lines
**						suggested by Steve Maquire in his book "Writing Solid
**						Code" (p17)
**  03/11/94     finassrt.h     SKM       replaced FinaleUnderConstruction
**						by '#include "needwork.h"'.
**  04/06/94     finassrt.h     RES       moved _FinAssert() into #ifdef DEVELOP
**						block
**  04/06/94     finassrt.h     RES       added HitDebugger()
**  04/06/94     finassrt.h     RES       HitDebugger() (and therefore _FinAssert())
**						needs to be available outside #ifdef DEVELOP
**  03/16/95     finassrt.h     TNW       Cast NULL in FinAssert to (void) to
**						avoid compiler warnings that it isn't being assigned.
**  07/19/95     finassrt.h     RES       redefined _FinAssert() for Windows
**						(see comments)
**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  05/02/97     FINASSRT.H     RES       tinkered with FinAssert()
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/04/97     FINASSRT.H     RES       removed __FILE__ from macro in Win16
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  05/12/98     FINASSRT.H     RES       added SetAssert()
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  07/01/98     FINASSRT.H     RES       removed WIN16 stuff
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: FINASSRT.H,v $
Revision 1.1.1.1  2002/12/19 19:13:11  dfober
first import

** 
** 5     8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 4     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 3     4/01/99 4:53p Rstokes
** added const, ECCSTR to  _FinAssert() (to shut Lint up)
 * 
 * 2     1/28/99 2:43p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _FINASSRT_H_
#define _FINASSRT_H_

#ifndef	_VERSION_H_
#include "version.h"
#endif

/* the assert is triggered if the passed expression is FALSE */

PUBLIC tbool GLOBAL _FinAssert(
	const tbool fatal,
	ECCSTR exp,			// may be NULLP
	ECCSTR fname,		// may be NULLP
	const unsigned line,
	ECCSTR str );			// may be NULLP (explanatory string)
PUBLIC tbool GLOBAL SetAssert( const tbool active );

#define	HitDebugger()	_FinAssert( NO, "NO", __FILE__, __LINE__, "HitDebugger() called" )

#ifdef DEVELOP

// Mac and Win32

#define FinAssert(fatal,exp,str) 	\
	if ( exp )						\
		(void)NULL;					\
	else							\
		_FinAssert( fatal, #exp, __FILE__, __LINE__, (ECSTR)str )

#else

#define FinAssert(fatal,exp,str)	(void)0

#endif	// DEVELOP

#endif

