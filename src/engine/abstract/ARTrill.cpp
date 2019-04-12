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

#include "ARTrill.h"
#include "ARKey.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "gddefs.h"
#include "GuidoDefs.h"

using namespace std;



ARTrill::ARTrill(ORNAMENT typ, const ARKey* currentKey) : ARMTParameter(), fCurrentKey(currentKey), fTrillType(typ)
{
	setupTagParameters (gMaps->sARTrillMap);

	rangesetting            = ONLY;
	fChordType				= ARMusicalVoice::UP_SIMPLE;
//	fChordAccidental			= ARMusicalVoice::NONE;
	fCautionaryAccidental	= false;
	fForceAccidental		= false;
	fShowTR                 = true;
//	fDrawOnNoteHead            = false;
	fTrillPosition			= AUTO;
	fAdx = fAdy				= 0;
	fRepeat					= kUndefined;
	fBegin					= true;
}

ARTrill::ARTrill(int pid, const ARTrill* copy) : ARMTParameter(pid, copy)
{
	setupTagParameters (gMaps->sARTrillMap);
	copyParameters (copy->getTagParameters());
	fCurrentKey					= copy->fCurrentKey;

//	setTagParameters (copy->getTagParameters());

	rangesetting        = ONLY;

	fTrillType			= copy->getOrnament();
	fType				= copy->getType();
//	fChordType			= copy->getChordType();
	fAccidental			= copy->getIsAuto() ?  kNoAccidental : copy->getAccidental();
//	fChordAccidental	= copy->getChordAccidental();
	fCautionaryAccidental= copy->getCautionary();
	fForceAccidental	= copy->fForceAccidental;
	fShowTR				= copy->fShowTR;
//	fDrawOnNoteHead		= copy->fDrawOnNoteHead;
	fTrillPosition		= copy->fTrillPosition;
	fRepeat				= copy->fRepeat;
	fAdx				= copy->getadx();
	fAdy				= copy->getady();
	fBegin				= copy->getStatus();
}

//--------------------------------------------------------------------------
ARMusicalObject * ARTrill::Copy() const
{
	ARTrill* trill = new ARTrill(-1, this);
	trill->setContinue();
	trill->setIsAuto(true);
	return trill;
}

//--------------------------------------------------------------------------
static map<int,float> keyNum2pitches (int key) {
	map<int,float> out;
	if (key > 0) {
		out[NOTE_F] = 1.f;
		if (key > 1) out[NOTE_C] = 1.f;
		if (key > 2) out[NOTE_G] = 1.f;
		if (key > 3) out[NOTE_D] = 1.f;
		if (key > 4) out[NOTE_A] = 1.f;
		if (key > 5) out[NOTE_E] = 1.f;
		if (key > 6) out[NOTE_H] = 1.f;
	}
	else if (key < 0) {
		out[NOTE_H] = -1.f;
		if (key < -1) out[NOTE_E] = -1.f;
		if (key < -2) out[NOTE_A] = -1.f;
		if (key < -3) out[NOTE_D] = -1.f;
		if (key < -4) out[NOTE_G] = -1.f;
		if (key < -5) out[NOTE_C] = -1.f;
		if (key < -6) out[NOTE_F] = -1.f;
	}
	return out;
}

static int note2freekeyIndex (int note) {
	switch (note) {
		case NOTE_C:	return 0;
		case NOTE_D:	return 1;
		case NOTE_E:	return 2;
		case NOTE_F:	return 3;
		case NOTE_G:	return 4;
		case NOTE_A:	return 5;
		case NOTE_H:	return 6;
		default:		return -1;
	}
}

//--------------------------------------------------------------------------
// gives the accidental that should be displayed according to the trilled
// note and the current key
// takes account of cautionnary and force attributes
// return a signed count of half tones as a floating point to cope with microtones
//--------------------------------------------------------------------------
float ARTrill::getAccidental (const std::string& note, const ARKey* key) const
{
	const char* ptr = note.c_str();
	int pitch = ARKey::getNote (ptr);
	if ((pitch == EMPTY) || (pitch == REST)) return kNoAccidental;

	float acc = ARKey::getAccidental (ptr);		// acc is a count of sharps (>0) or flat (<0)
	acc += getParameter<TagParameterFloat>(kDetuneStr, true)->getValue();
	if (getCautionary()) return acc;

	if (key) {
		if (key->freeKey()) {
			int ki = note2freekeyIndex (pitch);
			if (ki >= 0) {
				float fk[NUMNOTES];
				key->getFreeKeyArray(fk);
				if ((acc == fk[ki]) && !fForceAccidental) acc = kNoAccidental;
			}
		}
		else {
			map<int,float> km = keyNum2pitches (key->getKeyNumber());
			if ((acc == km[pitch]) && !fForceAccidental) acc = kNoAccidental;
		}
	}
	else if (!acc) acc = kNoAccidental;
	return acc;
}

//--------------------------------------------------------------------------
void ARTrill::setTagParameters (const TagParameterMap& params)
{
	ARMTParameter::setTagParameters (params);
	const TagParameterString* note = getParameter<TagParameterString>(kNoteStr);
	if (note) fTrilledNote = note->getValue();

	const TagParameterString* p = getParameter<TagParameterString>(kAccidentalStr);
	if (p) {
		string val = p->getValue();
		if (val == "cautionary")
			fCautionaryAccidental = true;
		else if (val == "force")
			fForceAccidental = true;
	}
	
	fAdx	= getParameter<TagParameterFloat>(kAdxStr, true)->getValue();
	fAdy	= getParameter<TagParameterFloat>(kAdyStr, true)->getValue();
	fShowTR = getParameter<TagParameterString>(kTrStr, true)->getBool();
	
	p = getParameter<TagParameterString>(kPositionStr);
	if (p) {
		string val = p->getValue();
		if (val == "below") fTrillPosition = BELOW;
		else if (val == "above") fTrillPosition = ABOVE;
	}
//	p = getParameter<TagParameterString>(kAnchorStr);
//	if (p) fDrawOnNoteHead = string(p->getValue()) == "note";
	p = getParameter<TagParameterString>(kRepeatStr);
	if (p) fRepeat = p->getBool() ? kOn : kOff;
	
	fBegin = getParameter<TagParameterString>(kBeginStr, true)->getBool();
	fAccidental = fTrilledNote.size() ? getAccidental (fTrilledNote, fCurrentKey) : kNoAccidental;
	
	string type = getParameter<TagParameterString>(kTypeStr, true)->getValue();
	if (type == "prall")			fType = kPrall;
	else if (type == "prallprall")	fType = kPrallPrall;
	else if (type == "inverted")	fType = kInverted;
	else if (type == "prallinverted") fType = kPrallInverted;
}

//--------------------------------------------------------------------------
bool ARTrill::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\trillEnd",s))
		return true;
	return false;
}

//--------------------------------------------------------------------------
string ARTrill::getGMNName() const
{
    switch (fTrillType) {
		case TRILL:	return "\\trill";
		case TURN:	return "\\turn";
		case MORD:	return "\\mord";
    }
	return "\\unknownTrill";
}
