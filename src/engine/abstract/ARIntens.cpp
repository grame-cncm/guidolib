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

#include "ARIntens.h"

#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TimeUnwrap.h"


ARIntens::ARIntens()
{
	setupTagParameters (gMaps->sARIntensMap);
	relativeTimePosition.set( -1, 1 );
	setDuration ( DURATION_0 );
 }

ARIntens::ARIntens(const char * txt)
{
	setupTagParameters (gMaps->sARIntensMap);
	fIntens = txt;
	relativeTimePosition.set( -1, 1 );
	setDuration ( DURATION_0 );
}

void ARIntens::setTagParameters (const TagParameterMap& params)
{
	ARFontAble::setTagParameters(params);

	const TagParameterString* p = getParameter<TagParameterString>(kTypeStr);
	fIntens = p ? p->getValue() : "";
	p = getParameter<TagParameterString>(kBeforeStr);
	fBefore = p ? p->getValue() : "";
	p = getParameter<TagParameterString>(kAfterStr);
	fAfter = p ? p->getValue() : "";
	fAutoPos = getParameter<TagParameterString>(kAutoposStr, true)->getBool();
}

// --------------------------------------------------------------------------
void ARIntens::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kIntens);
}

