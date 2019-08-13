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
//	fStretchLastLineState   = kOff;
//	fStretchFirstLineState  = kOff;
	fLyricsAutoPos		    = kOff;
	fInstrAutoPos		    = kOff;
	fIntensAutoPos		    = kOff;
	fFingeringPos			= kDefault;
	fHarmonyPos				= kDefault;
	fFingeringSize			= 0;
}

void ARAuto::setTagParameters (const TagParameterMap& params)
{
	const string off("off");
	const string on("on");
	const TagParameterString * p = getParameter<TagParameterString>(kAutoEndBarStr, kEndBarStr);
	if (p && (off == p->getValue())) fEndBarState = kOff;

	p = getParameter<TagParameterString>(kAutoPageBreakStr, kPageBreakStr);
	if (p && (off == p->getValue())) fPageBreakState = kOff;

	p = getParameter<TagParameterString>(kAutoSystemBreakStr, kSystemBreakStr);
	if (p && (off == p->getValue())) fSystemBreakState = kOff;

	p = getParameter<TagParameterString>(kAutoClefKeyMeterOrderStr, kClefKeyMeterOrderStr);
	if (p && (off == p->getValue())) fClefKeyMeterOrderState = kOff;

//	p = getParameter<TagParameterString>(kAutoStretchLastLineStr, kStretchLastLineStr);
//	if (p && (on == p->getValue())) fStretchLastLineState = kOn;
//
//	p = getParameter<TagParameterString>(kAutoStretchFirstLineStr, kStretchFirstLineStr);
//	if (p && (on == p->getValue())) fStretchFirstLineState = kOn;

	p = getParameter<TagParameterString>(kAutoInstrPosStr, kInstrAutoPosStr);
	if (p && (on == p->getValue())) fInstrAutoPos = kOn;

	p = getParameter<TagParameterString>(kAutoLyricsPosStr, kLyricsAutoPosStr);
	if (p && (on == p->getValue())) fLyricsAutoPos = kOn;

	p = getParameter<TagParameterString>(kAutoIntensPosStr, kIntensAutoPosStr);
	if (p && (on == p->getValue())) fIntensAutoPos = kOn;

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

	const TagParameterFloat * f = getParameter<TagParameterFloat>(kFingeringSizeStr);
	if (f) {
		fFingeringSize = f->getValue();
		fHasFingeringSize = true;
	}
}
