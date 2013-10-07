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

// Implementation is in GUIDOEngine.cpp
void GuidoWarn( const char * inMessage, const char * inInfo = 0 );
void GuidoTrace( const char * inMessage );

#endif
