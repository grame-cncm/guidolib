#ifndef ARDefine_H
#define ARDefine_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GuidoDefs.h"


#define MUSICAL_MIN_NUMERATOR 0
#define MUSICAL_MAX_NUMERATOR 32767
#define MUSICAL_MIN_DENOMINATOR 1
#define MUSICAL_MAX_DENOMINATOR 128
#define DEFAULT_DENOMINATOR 4
#define DEFAULT_NUMERATOR 1

#define MIN_ACCIDENTALS (-2)	
#define MAX_ACCIDENTALS 2	

#define MIN_PITCH 0			// ?
#define MAX_PITCH 255		// ? 127 ?

#define UNKNOWN 0

#define MIN_INTENSITY 0
#define MAX_INTENSITY 127
#define DEFAULT_INTENSITY 80

#define MIN_REGISTER (-10)
#define MAX_REGISTER 10
#define DEFAULT_REGISTER 1

#endif
