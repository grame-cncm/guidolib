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

#include "ARStaccato.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TimeUnwrap.h"

using namespace std;

extern const char* kAboveStr;
extern const char* kBelowStr;


static const TagParameterMap sARStaccatoMap (kARStaccatoParams);

//--------------------------------------------------------------------------
ARStaccato::ARStaccato()
{
	setupTagParameters (sARStaccatoMap);
	fType = REGULAR;
}

//--------------------------------------------------------------------------
void ARStaccato::setTagParameters (const TagParameterMap& params)
{
	ARArticulation::setTagParameters (params);
	const TagParameterString * p = getParameter<TagParameterString>(kTypeStr);
	if (p) {
		string val = p->getValue();
		fType = (val == "heavy") ? HEAVY : REGULAR;
	}
}

// --------------------------------------------------------------------------
void ARStaccato::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kStaccato);
}

bool ARStaccato::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() && !strcmp("\\staccEnd",s))
		return 1;
	return 0;
}

