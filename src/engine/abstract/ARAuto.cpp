/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARAuto.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

using namespace std;


ARAuto::ARAuto()
{
	setupTagParameters (gMaps->sARAutoMap);

	fEndBarState            = kOn;
	fPageBreakState         = kOn;
	fSystemBreakState       = kOn;
	fClefKeyMeterOrderState = kOn;
	fLyricsAutoPos		    = kOff;
	fInstrAutoPos		    = kOff;
	fIntensAutoPos		    = kOff;
	fFingeringPos			= kDefault;
	fAutoHideTiedAccidentals = kOff;
	fHarmonyPos				= kDefault;
	fFingeringSize			= 0;
	fAutoMultiVoiceCollisions = kOff;
}

void ARAuto::setTagParameters (const TagParameterMap& params)
{
	const string off("off");
	const string on("on");
	const TagParameterString * p = getParameter<TagParameterString>(kAutoEndBarStr, kEndBarStr);
	if ( p ) fEndBarState = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoPageBreakStr, kPageBreakStr);
	if ( p ) fPageBreakState = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoSystemBreakStr, kSystemBreakStr);
	if ( p ) fSystemBreakState = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoClefKeyMeterOrderStr, kClefKeyMeterOrderStr);
	if ( p ) fClefKeyMeterOrderState = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoInstrPosStr, kInstrAutoPosStr);
	if ( p ) fInstrAutoPos = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoLyricsPosStr, kLyricsAutoPosStr);
	if ( p ) fLyricsAutoPos = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoIntensPosStr, kIntensAutoPosStr);
	if ( p ) fIntensAutoPos = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kAutoHideTiedAccidentals);
	if ( p ) fAutoHideTiedAccidentals = p->getBool() ? kOn : kOff;

	p = getParameter<TagParameterString>(kFingeringPosStr);
	if (p) {
		if (p->getString() == kAboveStr) 	 fFingeringPos = kAbove;
		else if (p->getString() == kBelowStr) fFingeringPos = kBelow;
		fHasFingeringPos = true;
	}

	p = getParameter<TagParameterString>(kHarmonyPosStr);
	if (p) {
		if (p->getString() == kAboveStr) 	  fHarmonyPos = kAbove;
		else if (p->getString() == kBelowStr) fHarmonyPos = kBelow;
		fHasHarmonyPos = true;
	}

	p = getParameter<TagParameterString>(kAutoMultiVoiceCollisions);
	if ( p ) {
		fAutoMultiVoiceCollisions = p->getBool() ? kOn : kOff;
		fHasMVoiceCollision = true;
	}

	const TagParameterFloat * f = getParameter<TagParameterFloat>(kFingeringSizeStr);
	if (f) {
		fFingeringSize = f->getValue();
		fHasFingeringSize = true;
	}
	
}
