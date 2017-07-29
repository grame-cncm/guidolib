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

#include <cstring>
#include <iostream>

#include "ARBeam.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"


ARBeam::ARBeam()
{
	setupTagParameters (gMaps->sARBeamMap);
	rangesetting = ONLY;
	dx1 = dx2 = dx3 = dx4 = NULL;
	dy1 = dy2 = dy3 = dy4 = NULL;
	fFeathered = false;
}

bool ARBeam::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\beamEnd",s))
		return true;
	return false;
}

void ARBeam::setTagParameters (const TagParameterMap& params)
{
	size_t n = params.size();
	bool hasdy1 = params.find(kDy1Str) != params.end();
	bool hasdy2 = params.find(kDy2Str) != params.end();
	if (n == 1) {
		if (params.find(kDyStr) != params.end())
			dy1 = getParameter<TagParameterFloat>(kDyStr);
		else if (hasdy1 || hasdy2) {
			dy1 = getParameter<TagParameterFloat>(kDy1Str, true);
			dy3 = getParameter<TagParameterFloat>(kDy2Str, true);
		}
	}
	else if ((n == 2) && hasdy1 && hasdy2) {
		dy1 = getParameter<TagParameterFloat>(kDy1Str, true);
		dy3 = getParameter<TagParameterFloat>(kDy2Str, true);
	}
	else {
		dx1 = getParameter<TagParameterFloat>(kDx1Str, true);
		dx2 = getParameter<TagParameterFloat>(kDx2Str, true);
		dx3 = getParameter<TagParameterFloat>(kDx3Str, true);
		dx4 = getParameter<TagParameterFloat>(kDx4Str, true);

		dy1 = getParameter<TagParameterFloat>(kDy1Str, true);
		dy2 = getParameter<TagParameterFloat>(kDy2Str, true);
		dy3 = getParameter<TagParameterFloat>(kDy3Str, true);
		dy4 = getParameter<TagParameterFloat>(kDy4Str, true);
	}
}

bool ARBeam::isGuidoSpecBeam() const
{
	const bool oneset = (dy1 && dy1->TagIsSet());
	const bool twoset = (dy2 && dy2->TagIsSet());
	const bool fourset = (dy4 && dy4->TagIsSet());
	if (oneset && !twoset && !fourset)	// threeset ? 
		return true;
	return false;
}

