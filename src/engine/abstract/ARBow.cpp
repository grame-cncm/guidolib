/*
  GUIDO Library
  Copyright (C) 2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARBow.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

#include "TimeUnwrap.h"

using namespace std;


//--------------------------------------------------------------------------
ARBow::ARBow()
{
	setupTagParameters (gMaps->sARBowMap);
	rangesetting = RANGEDC;
}

//--------------------------------------------------------------------------
void ARBow::setTagParameters (const TagParameterMap& params)
{
	ARArticulation::setTagParameters (params );
	const TagParameterString* p = getParameter<TagParameterString>(kTypeStr);
	if (p) {
		string type = p->getValue();
		if (type == kUpStr) 		fUp = true;
		else if (type == kDownStr) 	fUp = false;
	}
}


