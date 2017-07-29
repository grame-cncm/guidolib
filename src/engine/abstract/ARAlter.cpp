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
#include <sstream>

#include "ARAlter.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

using namespace std;


ARAlter::ARAlter(const ARAlter * p_savealter, const ARAlter * copyalter )
	: ARMTParameter(-1,copyalter) 
{
	setupTagParameters (gMaps->sARAlterMap);
	relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
	setDuration (DURATION_0);
	rangesetting = RANGEDC;	// RANGEDC replaced by ONLY while impl. finalzed [DF 2001-04-20];
	fSaveAlter = p_savealter;

	if (copyalter) {
		copyParameters (copyalter->getTagParameters());
		setTagParameters (copyalter->getTagParameters());
	}
}

std::string ARAlter::getAlterText() const
{ 
	const TagParameterString* text = getParameter<TagParameterString>(kTextStr);
	if (text) return text->getValue();

	stringstream sstr;
	sstr << getDetune() << ".2f";
	return sstr.str();
}

float ARAlter::getDetune() const
{ 
	const TagParameterFloat* detune = getParameter<TagParameterFloat>(kDetuneStr);
	return detune ? detune->getValue() : 0;
}

