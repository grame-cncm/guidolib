/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARAccelerando.h"

using namespace std;

static const TagParameterMap sARAccelerandoMap (kARAccelerandoParams);

ARAccelerando::ARAccelerando()
{
	setupTagParameters (sARAccelerandoMap);
    rangesetting = ONLY;
}

bool ARAccelerando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\accelEnd",s))
		return true;
	return false;
}

