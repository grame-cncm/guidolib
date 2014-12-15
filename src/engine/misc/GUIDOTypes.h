#ifndef GUIDOTypes_H
#define GUIDOTypes_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

