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
#include "TagParameterInt.h"


ARBar::ARBar(const TYPE_TIMEPOSITION &timeposition)
	: ARMTParameter(timeposition)
{
	setupTagParameters (gMaps->sARBarMap);

	fMeasureNumber               = 0;
    fMeasureNumberDisplayed      = kNoNum;
    fMeasureNumberDisplayedIsSet = false;
    fSkippedMeasureNum          = false;
	numDx = numDy = 0;
}

ARBar::ARBar()
{
	setupTagParameters (gMaps->sARBarMap);

	fMeasureNumber               = 0;
    fMeasureNumberDisplayed      = kNoNum;
    fMeasureNumberDisplayedIsSet = false;
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
		fMeasureNumberDisplayed = (p->getBool() ? kNumAll : kNoNum);
		fMeasureNumberDisplayedIsSet = true;
	}
    p = getParameter<TagParameterString>(kHiddenStr);
	if (p) fHidden = p->getBool();
    const TagParameterInt* num = getParameter<TagParameterInt>(kMeasNumStr);
    fMeasureNumber = num ? num->getValue() : 0;

    const TagParameterFloat* dx = getParameter<TagParameterFloat>(kNumDxStr);
	numDx = dx ? dx->getValue() : 0;
    const TagParameterFloat* dy = getParameter<TagParameterFloat>(kNumDyStr);
	numDy = dy ? dy->getValue() : 0;
}

