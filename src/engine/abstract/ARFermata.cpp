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

#include "ARFermata.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"
#include "TimeUnwrap.h"

using namespace std;

extern const char* kBelowStr;
static const TagParameterMap sARFermataMap (kARFermataParams);

ARFermata::ARFermata()
{
	setupTagParameters (sARFermataMap);

	rangesetting = RANGEDC;
	fType = REGULAR;
	fPosition = ARArticulation::kAbove;
}

//--------------------------------------------------------------------------------------
void ARFermata::setTagParameters(const TagParameterMap & params)
{
	ARArticulation::setTagParameters (params);

	string type = getParameter<TagParameterString>(kTypeStr, true)->getValue();
	if (type == "short")		fType = SHORT;
	else if (type == "long")	fType = LONG;
	else						fType = REGULAR;

	string pos = getParameter<TagParameterString>(kPositionStr, true)->getValue();
	if (pos == kBelowStr)		fPosition = ARArticulation::kBelow;
}

// --------------------------------------------------------------------------
void ARFermata::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kFermata);
}

