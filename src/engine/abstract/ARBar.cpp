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

#include "ARBar.h"

#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"


ARBar::ARBar(const TYPE_TIMEPOSITION &timeposition)
	: ARMTParameter(timeposition)
{
	setupTagParameters (gMaps->sARBarMap);

	measureNumber               = 0;
    measureNumberDisplayed      = kNoNum;
    measureNumberDisplayedIsSet = false;
    fSkippedMeasureNum          = false;
	numDx = numDy = 0;
}

ARBar::ARBar()
{
	setupTagParameters (gMaps->sARBarMap);

	measureNumber               = 0;
    measureNumberDisplayed      = kNoNum;
    measureNumberDisplayedIsSet = false;
    fSkippedMeasureNum          = false;
	numDx = numDy = 0;
}

void ARBar::setTagParameters (const TagParameterMap& params)
{
    const TagParameterString* p = getParameter<TagParameterString>(kDisplayMeasNumStr);
	if (p) {
		std::string skipped("skipped");
		const char* displayMeasNum = p->getValue();
		
		fSkippedMeasureNum = (skipped == displayMeasNum);
		measureNumberDisplayed = (p->getBool() ? kNumAll : kNoNum);
		measureNumberDisplayedIsSet = true;
	}

    const TagParameterFloat* num = getParameter<TagParameterFloat>(kNumDxStr);
	numDx = num ? num->getValue() : 0;
    num = getParameter<TagParameterFloat>(kNumDyStr);
	numDy = num ? num->getValue() : 0;
}

