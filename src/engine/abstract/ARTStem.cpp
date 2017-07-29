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

#include "ARTStem.h"
#include "TagParameterFloat.h"
#include "TagParameterStrings.h"

using namespace std;


ARTStem::ARTStem(int st, const ARTStem * p_savestem, const ARTStem * p_copystem )
									: ARMTParameter(-1, p_copystem)
{
	TagParameterMap map (kARTStemParams);
	setupTagParameters (gMaps->sARTStemMap);

	rangesetting = RANGEDC;
	mStemState = (ARTStem::STEMSTATE) st;
	mSaveStem = p_savestem;

	if (p_copystem)
	{
		mStemState = p_copystem->getStemState();
		copyParameters (p_copystem->getTagParameters());
		setTagParameters (p_copystem->getTagParameters());
	}
}


ARTStem::ARTStem(const ARTStem * tstem) : ARMTParameter(-1,tstem)
{
	setupTagParameters (gMaps->sARTStemMap);
	mSaveStem = NULL;
	mStemState = tstem->mStemState;
	copyParameters (tstem->getTagParameters());
	setTagParameters (tstem->getTagParameters());
}

string ARTStem::getGMNName() const
{
	switch (mStemState) {
		case UP:	return "\\stemsUp";
		case DOWN:	return "\\stemsDown";
		case AUTO:	return "\\stemsAuto";
		case OFF:	return "\\stemsOff";
	}
	return "\\stem";
};

const TagParameterFloat * ARTStem::getLength() const
{
	return getParameter<TagParameterFloat>(kLengthStr, true);
}

