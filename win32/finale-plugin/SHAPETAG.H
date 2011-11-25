/*
File Name:		SHAPETAG.H
Module Name:	SHAPETAG.H -- #defines for SHAPELIST instruction tags
Module Number:
Date:			10/30/87
Author:			Jim Rauser

Modification History:

		10/30/87	jsr		Created.

Purpose:		This file defines a set of coded values for the instructions
				used in Engima SHAPELIST records.  Previous versions of
				the Enigma source used double-character constants (e.g. 'AB')
				to encode the instructions.  This construction is non-ANSI
				and is not supported in a consistent way by all C compilers.
				The values are defined in such a way to make them numerically
				consistent across all machine architecture.

				Note: the numbers in parentheses in the comments below are
				the number of data value the instruction expects.

	11/5/91: For version 3.0+, line width will be stored in 64ths of an
	EVPU rather than hundredths of a point. To convert old files, we
	would have to actually load each shape in ConvertOldFile()
	(FILE\CONVERT.C) in order to scan it and find the proper data offset.
	Rather than mess with this, I thought it easier to define a new tag
	st_linewidth to replace st_setwidth.  Whenever LoadShape() finds an
	archaic st_setwidth tag, it will replace it with st_linewidth and
	scale the data value accordingly. This means LoadShape() is the only
	routine that needs to see the st_setwidth definition, so I moved it
	there. (RES)

**  09/21/1991   shapetag.h    randy      gave all the shape tags better,
**                    more mnemonic names
**  09/23/1991   shapetag.h    randy      added st_slur, st_startobject,
**                    st_setgray
**  09/24/1991   shapetag.h    randy      added st_ellipse, st_rectangle
**  12/23/1991   shapetag.h    randy      retired st_lineto and st_moveto.
**                    They complicated some of the logic in the shape
**                    support routines, and I don't believe they were
**                    ever actually stored in a shape.
**  09/18/1992   shapetag.h    randy      added st_bracket
**  08/26/93     shapetag.h     RES       added st_clonechar (for future
**						development)
**  05/23/1995   shapetag.h    ALE        Merged Mac 3.2.1 changes:
**                     Added st_tie.
**  01/11/1996   shapetag.h    TNW        Added st_extgraphic. (eps,tiff,pict support)
**  02/09/96     shapetag.h     RES       comments
**  02/09/96     shapetag.h     RES       added st_vertmode (for brackets; see
**						drawbrac.c)
**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  03/31/97   shapetag.h    TNW        More comments.
**  05/31/97   shapetag.h    CFC        Comments for changes made to st_tie,
**						moved some stuff here from FDEBUG.H that I thought
**						would be useful elsewhere.  included pathprot.h
**						for new MakePathInst()
**  06/01/97   shapetag.h    CFC        moved strings to resources, added
**						finstrid.h
**  06/01/97   shapetag.h    CFC        blasted 16 bit Windows! move some stuff
**						back to FDEBUG.C
**  06/02/97   shapetag.h    TNW        moved new stuff to paths.h (new file)
**						since extensions need shapetags.h (but not other stuff)
**  09/24/97     SHAPETAG.H     RES       added st_gotoorigin
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  06/25/98 ===== shipped Finale Mac 4.0.0r1 =====
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
**  12/22/98 ===== shipped Finale Mac 98c.r1 =====
**  01/18/99     SHAPETAG.H     CFC       added experimental st_shape, st_cloneshape,
**						moved st_stretchhorz and st_stretchvert to here from smrtshap.h,
**						changed defines to const ETAG
**  01/25/99     SHAPETAG.H     CFC       more work on st_shape, st_cloneshape
*//*~*/
/*
$Log: SHAPETAG.H,v $
Revision 1.1.1.1  2002/12/19 19:13:13  dfober
first import

** 
** 11    8/16/99 12:02p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 10    7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 9     7/6/99 3:39 PM ccianflo
** experimenting with st_arrowheadinfo
** 
** 8     6/1/99 2:47 PM ccianflo
** moved st_textblock outside of DEVELOP section
** 
** 7     5/2/99 7:25 PM ccianflo
** changed number of data from 4 to 5 for st_clonechar (ok to do since
** st_clonechar is only internally used by Enigma, not saved in any shapes
** anywhere), extra data item was for a "baseline" shift value in EMs
** 
** 6     4/5/99 11:20 AM ccianflo
** st_setarrowhead
** 
** 5     3/23/99 11:52 AM ccianflo
** added experimental st_textblock
 * 
 * 4     1/28/99 2:42p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _SHAPETAG_H_
#define _SHAPETAG_H_

#ifndef _VERSION_H_
#include "VERSION.H"
#endif

/*--------------------------------------------------------------------------------------
	Shape Tags
--------------------------------------------------------------------------------------*/

/* shape tags. Number in () is the number of PATHDATA items associated with this tag.
   Every shape should start with a st_startobject instruction.
   if isFloat is set in  the path structure,  All coordinate data, line widths,
   grey values are floating point; otherwise they are long integers.
   */

const ETAG st_closepath		= MAKETAG( 'c','p' );
	/* close path (0) */

const ETAG st_curve			= MAKETAG( 'c','v' );
	/* relative curveto (bezier); (6: 2 control points and an end point.) */

const ETAG st_drawchar		= MAKETAG( 'd','c' );
	/* draw character (1: character to draw, may be a 16-bit char for twobyte fonts.) */

const ETAG st_dashline		= MAKETAG( 'd','l' );
	/* relative dashed lineto (4: X, Y, dash length, length between dashes) */

const ETAG st_fillalt		= MAKETAG( 'f','l' );
	/* fill path (alternating, or even-odd rule) (default) (0) */

const ETAG st_fillsolid		= MAKETAG( 'f','s' );
	/* fill path (solid, or non-zero winding rule) (0) */

const ETAG st_gotostart		= MAKETAG( 'g','s' );
	/* go to start of path, same as close path for filled shapes (0) */

const ETAG st_gotoorigin	= MAKETAG( 'g','o' );
	/* for converted 2.X shapes: before 3.0, "go to start" meant "go to origin";
	in 3.0 we changed it to mean "go to beginning of shape"; when reading 2.X
	files, we encapsulate the old shapes but do not attempt to convert them to
	the new syntax; we therefore need to substitute st_gotoorigin for st_gotostart
	wherever found in order for the 2.X shapes to draw correctly. (9/24/97 RES) */

const ETAG st_linewidth		= MAKETAG( 'l','w' );
	/* line width (1: EFIX line width desired) */

const ETAG st_rlineto		= MAKETAG( 'r','l' );
	/* relative lineto (2: X,Y end point.) */

const ETAG st_rmoveto		= MAKETAG( 'r','m' );
	/* relative moveto (2: X,Y end point.) */

const ETAG st_setdash		= MAKETAG( 's','d' );
	/* set dash (2: dash length, length between dashes) */
	
const ETAG st_setarrowhead	= MAKETAG( 's','a' );
	/* set arrowhead (4: startArrowID, endArrowID, startFlags, endFlags ) */
	// The flags tell us how to interpret startArrowId and endArrowID.  We
	// will probably support some built in arrow types, and user defined
	// arrow types.  There is also the possibility that this will become
	// a more general instruction like line cap, allowing for other types
	// of ends on lines, other than arrowheads.

const ETAG st_arrowheadinfo = MAKETAG( 'a','h' );
	// This will only be found in shapes that are arrowheads.  If the tag is
	// not found, default values will be used.  Currently, the tag is used
	// to store the amount of "backoff" when a line connects to an arrowhead.
	// (1: the amount of backoff, EFIX)

const ETAG st_setfont		= MAKETAG( 's','f' );
	/* set font (3: (all integers) enigma font id, size, efx) */

const ETAG st_slur			= MAKETAG( 's','l' );
	/* slur (6: 2 control points and an end point, like curve, except 2 more control points
	         are deduced from a global slur thickness.) */

const ETAG st_tie			= MAKETAG( 't','i' );
	/* tie (6: 2 control points and an end point, like curve, except 2 more control points
	         are deduced from a global tie thickness.)
		CFC 05/31/97: Since st_tie is only used by Finale during application
		runtime (it is not saved anywhere, I am free to add 2 more data
		items)  They are the amount by which we drop the inner control points */

const ETAG st_startobject	= MAKETAG( 's','o' );
	/* start object (11):
							data offsets:
	0,1: 	point for object origin (relative to starting point of enclosing object)
	2..5: 	current bounding rect (left, top, right, bottom; in absolute coordinates)
	6,7: 	X and Y transform (each are a scale ratio * 1000)
	8: 		rotation transform (not implemented)
// 3/6/93: 9, 10 no longer used for this:
//								9,10: 	scratch area for scale_shape()
//										(stores pre-scaled X and Y transforms)
	*/

const ETAG st_stroke		= MAKETAG( 's','t' );
	/* stroke path (0) */

const ETAG st_setgray		= MAKETAG( 's','g' );
	/* set gray (1: grey value between 0 and 100, where 0=black,100=white) */

const ETAG st_ellipse		= MAKETAG( 'e','l' );
	/* ellipse (2: width, height of bounding rectangle, current point is lower left corner) */

const ETAG	st_rectangle	= MAKETAG( 'r','e' );
	/* rectangle (2: width, height of rectangle, current point is lower left corner) */

const ETAG st_extgraphic	= MAKETAG( 'x','g' );
	/* external graphic (eps, pict, tiff, etc) (3: width, height of graphic,
	and graphic id (id of an ot_SHAPE_BASED_GRAPHIC) */

const ETAG st_null			= MAKETAG( 'n','u' );
	/* null instruction. Used during conversion of pre-3.0 files, and stripped out
		by LoadShape() (see CONVERT.C). */

const ETAG st_setwhite		= MAKETAG( 'w','o' );
	/* set white (0). Equivalent to st_setgray( 100 ), but simpler to use in conversions
		of pre 3.0 files because we don't have to insert data. */

const ETAG st_setblack		= MAKETAG( 'b','l' );
	/* set black (0). Equivalent to st_setgray( 0 ). */

const ETAG st_startgroup	= MAKETAG( 's','r' );
	/* start group (11) (same data as st_startobject, all instructions within group will
	   have the groups transform, origin applied to it, should be bracketed by
	   st_endgroup) */

const ETAG st_endgroup		= MAKETAG( 'e','r' );
	/* end group (0) marks the end of the group. */

// a future improvement would be to remove st_bracket and replace it
// with a flag in the st_startgroup & st_startobject instruction.
// NOTE: we could also take this a step further storing the type of all
// objects in its st_startgroup & st_startobject instruction.

const ETAG st_bracket		= MAKETAG( 'b','r' );
	/* bracket (1). No action taken; used only to identify a complex shape as
	   a bracket.  Data item is the bracket type (PIANOBRAC, etc.). */

const ETAG st_clonechar		= MAKETAG( 'c','c' );
	/* cloned character for stretching a character string horizontally (e.g. trills)
	   Does nothing for filled shapes.

	   (5: 2 for ending point of character string (2nd y param currently ignored),
	       1 for extra space (not used currently)
	       1 for "baseline" shift, in EMs, 1/1000 of the current point size
				(CFC 04/29/99 This was necessary for doing things like
	       		the new angled glissando and we would like the wiggle to have the
	       		appearance of starting and ending at the terminating points of
	       		the line, especially for doing attachments to entries)
	       1 for character (either a byte or a twobyte char) */

const ETAG st_vertmode		= MAKETAG( 'v','m' );
	/* set pen vmode (1:  EPEN_VMODE_CENTER(1), EPEN_VMODE_LEFT(2), or EPEN_VMODE_RIGHT(3)
	   tells commands that draw lines to either center on, draw to the left or
	   draw to the right of the coordinates specified.

	*/

/*--------------------------------------------------------------------------------------
	Shape Tags: Internal use only (i.e. not for use by user in Shape Designer, just 
				for internal use by Enigma.
--------------------------------------------------------------------------------------*/

/* Shape Tags to allow programmable stretching of vertical and horizontal
    components and rotation of Smart Shapes. These tags toggle the states
    on and off. All states are on at the beginning of a shape */

const ETAG st_stretchhorz 	= MAKETAG( 's','h' );		/* Toggle Stretch Horizontal */
const ETAG st_stretchvert 	= MAKETAG( 's','v' );		/* Toggle Stretch Vertical */


/*--------------------------------------------------------------------------------------
	Shape Tags: Experimental, DEVELOP only.
--------------------------------------------------------------------------------------*/

#ifdef DEVELOP

//const ETAG st_shape		= MAKETAG( 's','h' );	// whoops, already used by st_stretchhorz
const ETAG st_shape			= MAKETAG( 'S','H' );
	/* shape id (1: the shape id of the shape to imbed).
	Allow a shape to be imbedded in a shape.  Initially, this will only be used
	internally by Finale to allow for more complex definition of predefined smart-shapes.
	For example, instead of 8va, let the user create what 8va should look like! */

const ETAG st_cloneshape	= MAKETAG( 'c','S' );
	/* cloned shape for stretching a shape in any direction (e.g. glissandos)
	Note: This is similar to st_clonechar
	
	   (4:	2 for ending point,
			1 for extra space, flags (like rotate shape along path), etc. (not used currently)
	   		1 for shape id */
	   		
#endif	// DEVELOP

const ETAG st_textblock		= MAKETAG( 't','b' );
	// text block (2)
	// Do a text block.  The idea here is to be able to specify the text of a glissando
	// smart shape for example, and enter the text in the standard Finale way via
	// an edit text dialog, complete with font changes, etc.  If I can make this work,
	// it will be much easier than inventing a new solution for text associated with
	// things like glissando.
	
	// 2: 	1 for text block id
	//		1 for text block tag	(ttp_block, etc)

/*--------------------------------------------------------------------------------------
	no longer used
--------------------------------------------------------------------------------------*/

//const ETAG st_lineto		MAKETAG( 'l','n' )		/* line to (2) (archaic) */
//const ETAG st_moveto		MAKETAG( 'm','v' )		/* move to (2) (archaic) */
//const ETAG st_setwidth	MAKETAG( 's','w' )		/* set line width (1) (archaic) */


#endif	// _SHAPETAG_H_

