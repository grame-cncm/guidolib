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

#include "ARArticulation.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TimeUnwrap.h"

using namespace std;

const char* kAboveStr = "above";
const char* kBelowStr = "below";

static const TagParameterMap sARArticulationMap (kARArticulationParams);

ARArticulation::ARArticulation() : fPosition(kDefaultPosition)
{
	setupTagParameters (sARArticulationMap);
	rangesetting = ONLY;
}

void ARArticulation::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* pos = getParameter<TagParameterString>(kPositionStr);
	if (pos) {
		string posStr = pos->getValue();
		if (posStr == kAboveStr)		fPosition = kAbove;
		else if (posStr == kBelowStr)	fPosition = kBelow;
		else cerr << "Guido Warning: '" << posStr << "': incorrect articulation position";
	}
}

// --------------------------------------------------------------------------
void ARArticulation::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kAccent);
}
