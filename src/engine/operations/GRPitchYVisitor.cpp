/*
  GUIDO Library
  Copyright (C) 2023 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <iostream>

#include "GRPitchYVisitor.h"

#include "GuidoDefs.h"
#include "GRBar.h"
#include "GRClef.h"
#include "GREmpty.h"
#include "GRKey.h"
#include "GRMeter.h"
#include "GRMusic.h"
#include "GROctava.h"
#include "GREvent.h"
#include "GRRepeatBegin.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRStaff.h"

using namespace std;

//-------------------------------------------------------------------------------
NVPoint GRPitchYVisitor::getPitchPos (GRMusic* music, int staffNum, int midipitch, TYPE_TIMEPOSITION date)
{
	fTargetStaff = staffNum;
	fCurrentStaff = 0;
	fBasePitch 	= NOTE_G;
	fBaseLine 	= 3;
	fBaseOct 	= 1;
	fOctava 	= 0;
	fTargetDate = date;
	fNextX = 0.f;
	fDone = false;
	fStaff = nullptr;
	fTargetElt = nullptr;
	music->accept (*this);
	NVPoint p;
	if (fDone && fStaff && fTargetElt) {
		midipitch -= (12 * fOctava);
		// convert midi pitch in pitch class and octava
		int oct = (midipitch / 12) - 4;
		int pitch = midipitch % 12;
		pitch = ((pitch < 5) ? (pitch / 2) : (pitch+1) / 2) + 2;
		NVPoint spos = fStaff->getPosition();
		float y = fStaff->getNotePosition ( pitch, oct, fBasePitch, fBaseLine, fBaseOct);
		p.x = interpolateXPos(fTargetElt, fTargetDate, fNextX);
		p.y = (spos.y + y);
	}
	return p;
}

//-------------------------------------------------------------------------------
float GRPitchYVisitor::interpolateXPos (const GRNotationElement* elt, TYPE_TIMEPOSITION target, float nextx) const
{
	TYPE_TIMEPOSITION offset = target - elt->getRelativeTimePosition();
	float ratio = float(offset) / float(elt->getDuration());
	float x = elt->getPosition().x;
	return x + (nextx - x) * ratio;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRStaff* o)
{
	if (fDone) return;
	fCurrentStaff = o->getStaffNumber();
	fStaff = o;
}

//-------------------------------------------------------------------------------
bool GRPitchYVisitor::checkTimePos (const GRNotationElement* elt)
{
	TYPE_TIMEPOSITION date = elt->getRelativeTimePosition();
	TYPE_TIMEPOSITION endDate = date + elt->getDuration();
	if ((fTargetDate >= date) && (fTargetDate < endDate)) {
		fTargetElt = elt;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::check (const GRNotationElement* o)
{
	if (fCurrentStaff != fTargetStaff) return;
	if (fDone) {
		if (!fNextX) fNextX = o->getPosition().x;
	}
	else fDone = checkTimePos(o);
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRBar* o)
{
	if (fCurrentStaff != fTargetStaff) return;
	if (fDone && !fNextX) fNextX = o->getPosition().x;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRMeter* o)
{
	if (fCurrentStaff != fTargetStaff) return;
	if (fDone && !fNextX) fNextX = o->getPosition().x;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRKey* o)
{
	if (fCurrentStaff != fTargetStaff) return;
	if (fDone && !fNextX) fNextX = o->getPosition().x;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRRepeatBegin* o)
{
	if (fCurrentStaff != fTargetStaff) return;
	if (fDone && !fNextX) fNextX = o->getPosition().x;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRSingleNote* o) 	{ check(o); }
void GRPitchYVisitor::visitStart (GREmpty* o) 		{ check(o); }
void GRPitchYVisitor::visitStart (GRSingleRest* o) 	{ check(o); }

void GRPitchYVisitor::visitStart (GRClef* o)
{
	if (fCurrentStaff != fTargetStaff) return;
	if (fDone) {
		if (!fNextX) fNextX = o->getPosition().x;
		return;
	}
	fBasePitch = o->getBasePitch();
	fBaseOct = o->getBaseOct();
	fBaseLine = o->getBaseLine();
}

void GRPitchYVisitor::visitStart (GROctava* o)
{
	if (fDone || (fCurrentStaff != fTargetStaff)) return;
	fOctava = o->getOctava();
}
