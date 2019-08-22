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
#include <string.h>

#include "ARAccelerando.h"

using namespace std;

#ifndef WIN32
#warning ("TODO: revise ARAccelerando parameters implementation");
#endif


ARAccelerando::ARAccelerando()
{
	setupTagParameters (gMaps->sARAccelerandoMap);
    rangesetting = ONLY;
}

//void ARAccelerando::setTagParameters (const TagParameterMap& params)
//{
//	ARFontAble::setupTagParameters(params);
//}

bool ARAccelerando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\accelEnd",s))
		return true;
	return false;
}

