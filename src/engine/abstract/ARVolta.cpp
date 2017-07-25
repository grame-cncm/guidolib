/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>
#include <iostream>

#include "ARVolta.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TimeUnwrap.h"

using namespace std;

static const TagParameterMap sARVoltaMap (kARVoltaParams);

ARVolta::ARVolta()
{
	setupTagParameters (sARVoltaMap);
	rangesetting = ONLY;
}

void ARVolta::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kVolta);
}

bool ARVolta::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\voltaEnd",s))
		return true;
	return false;
}

const char* ARVolta::getMark() const
{
	const TagParameterString* p = getParameter<TagParameterString>(kMarkStr, true);
	return p->getValue();
}

const char* ARVolta::getFormat() const
{
	return getParameter<TagParameterString>(kFormatStr, true)->getValue();
}


