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

#include "ARUnits.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

const char* ARUnits::sType = "hs";

static const TagParameterMap sARUnitsMap (kARUnitsParams);

ARUnits::ARUnits()
{
	setupTagParameters (sARUnitsMap);
    sType = "hs";
}

//--------------------------------------------------------------------------
void ARUnits::setTagParameters (const TagParameterMap& params)
{
	ARMTParameter::setTagParameters (params);
	const TagParameterString* p = getParameter<TagParameterString>(kTypeStr, true);
	sType = p->getValue();
}

