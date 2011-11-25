#ifndef GUIDOTypes_H
#define GUIDOTypes_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */


/*!
\addtogroup factory GUIDO Factory
@{
*/

// struct __GuidoPos { int unused; };	// TODO: Replace
// typedef __GuidoPos * GuidoPos;		//	with (void *) ?

//! \internal
typedef void *			GuidoPos;

//! used for dots
// typedef unsigned int	TYPE_POINTS; replaced by int
//! used for accidentals
typedef int				TYPE_ACCIDENTALS;
//! used for pitch
typedef int				TYPE_PITCH;
typedef unsigned int	TYPE_INTENSITY;
typedef int				TYPE_REGISTER;

typedef float			GCoord; // Guido coordinates are now floats

typedef char * 			TYPE_TAG_NAME;
typedef const char * 	TYPE_TAG_NAME_CONST;
typedef char * 			TYPE_PARAMETER;
typedef char * 			TYPE_TAGPARAMETER_STRING;
typedef int 			TYPE_TAGPARAMETER_INT;
typedef double 			TYPE_TAGPARAMETER_REAL;
/*! @} */

#endif

