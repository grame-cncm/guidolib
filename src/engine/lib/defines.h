#ifndef GuidoDefines_H
#define GuidoDefines_H

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

#include <iosfwd>

#include "nvstring.h"
#include "Fraction.h"

// - Guido types (that should be moved elsewhere ?)
typedef Fraction TYPE_TIMEPOSITION;
typedef Fraction TYPE_DURATION;

#define MIN_TIMEPOSITION Frac_0
#define DURATION_7_4  Frac_7_4
#define DURATION_3_2  Frac_3_2
#define DURATION_1	 Frac_1
#define DURATION_7_8  Frac_7_8
#define DURATION_3_4  Frac_3_4
#define DURATION_2    Frac_1_2
#define DURATION_7_16 Frac_7_16
#define DURATION_3_8  Frac_3_8
#define DURATION_4    Frac_1_4
#define DURATION_7_32 Frac_7_32
#define DURATION_3_16 Frac_3_16
#define DURATION_8    Frac_1_8
#define DURATION_7_64 Frac_7_64
#define DURATION_3_32 Frac_3_32
#define DURATION_3_64 Frac_3_64
#define DURATION_3_128 Frac_3_128
#define DURATION_7_128 Frac_7_128
#define DURATION_7_256 Frac_7_256
#define DURATION_1_12 Frac_1_12
#define DURATION_16   Frac_1_16
#define DURATION_32   Frac_1_32
#define DURATION_64	  Frac_1_64
#define DURATION_128  Frac_1_128
#define DURATION_0    Frac_0
#define DURATION_n1   Frac_n1
#define MIN_DURATION DURATION_0
#define MAX_DURATION Frac_Max


// Implementation is in GUIDOEngine.cpp
void GuidoWarn( const char * inMessage, const char * inInfo = 0 );
void GuidoTrace( const char * inMessage );

#endif
