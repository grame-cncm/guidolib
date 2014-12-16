/*
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  03/09/98     FXVERS.H     CFC       bumped FXT_VERSION from
**					0x03800401 (3.8.0 r1) to 0x04000107 (4.0.0 a7),
**					this will prevent Finale 98 plugins from running
**					in Finale 97
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
*//*~*/
/*
$Log: FXVERS.H,v $
Revision 1.1.1.1  2002/12/19 19:13:12  dfober
first import

** 
** 5     8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 4     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 3     2/01/99 3:03p Rstokes
** increased FXT_VERSION to 5.0.0d5 (0x05000005); for now, this means
** Finale99 will not load Fin98 plug-ins; this is not acceptable and will
** have to be resolved, but currently the mechanism for resolving
** versioning on Finale API calls is flawed (this is a problem for the new
** MSPEC record)
 *
 * 2     1/28/99 2:43p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _FXVERS_H_
#define _FXVERS_H_

/*--------------------------------------------------------------------------------*/

/* version: major version # in high word, minor version number in low word */

//Moved this to a separate file so that it can be used as a resource
//include.

//TNW 09/23/97 At least for now, I decided that it would be most useful
//if this matched the application version (or was organized like EFILE_VERSION)

//major version is application version (3.8.0)
//LSW is type and build (lsb).
//type: DEV = 0; ALPHA = 1; BETA = 2; RC = 3; RELEASE = 4;

//Note, this doesn't mean that FXT_VERSION is the application version,
//it is rather, the first application version that uses that version.

#define FXT_VERSION 0x05000005

#endif	// _FXVERS_H_
