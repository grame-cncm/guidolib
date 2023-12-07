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
#include "GRClef.h"
#include "GREmpty.h"
#include "GRMusic.h"
#include "GROctava.h"
#include "GREvent.h"
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
	fLastDate = -1;
	fLastX = -1;
	fDone = false;
	fStaff = nullptr;
	music->accept (*this);
	NVPoint p;
	if (fDone && fStaff) {
		// convert midi pitch in pitch class and octava
		int oct = (midipitch / 12) - 4;
		int pitch = midipitch % 12;
		pitch = ((pitch < 5) ? (pitch / 2) : (pitch+1) / 2) + 2;
//cerr << "GRPitchYVisitor::getPitchPos " << pitch << " " << oct << " date: " << date << " staff y " << fStaff->getPosition() <<  endl;
		float y = fStaff->getNotePosition ( pitch, oct, fBasePitch, fBaseLine, fBaseOct) + fStaff->getPosition().y;
		p.x = fLastX;
		p.y = y;
	}
	return p;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRStaff* o)
{
	if (fDone) return;
	fCurrentStaff = o->getStaffNumber();
//cerr << "GRPitchYVisitor::visitStart GRStaff " << fCurrentStaff << endl;
	fStaff = o;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRSingleNote* o)
{
	if (fDone || (fCurrentStaff != fTargetStaff)) return;
//cerr << "GRPitchYVisitor::visitStart GRSingleNote " << o << endl;
	TYPE_TIMEPOSITION date = o->getRelativeTimePosition();
	if (date > fTargetDate) fDone = true;
	else fLastX = o->getPosition().x;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GREmpty* o)
{
	if (fDone || (fCurrentStaff != fTargetStaff)) return;
//cerr << "GRPitchYVisitor::visitStart GREmpty " << o << endl;
	TYPE_TIMEPOSITION date = o->getRelativeTimePosition();
	if (date > fTargetDate) fDone = true;
	else fLastX = o->getPosition().x;
}

//-------------------------------------------------------------------------------
void GRPitchYVisitor::visitStart (GRSingleRest* o)
{
	if (fDone || (fCurrentStaff != fTargetStaff)) return;
//cerr << "GRPitchYVisitor::visitStart GRSingleRest " << o << endl;
	TYPE_TIMEPOSITION date = o->getRelativeTimePosition();
	if (date > fTargetDate) fDone = true;
	else fLastX = o->getPosition().x;
}

void GRPitchYVisitor::visitStart (GRClef* o)
{
	if (fDone || (fCurrentStaff != fTargetStaff)) return;
	fBasePitch = o->getBasePitch();
	fBaseOct = o->getBaseOct();
}

void GRPitchYVisitor::visitStart (GROctava* o)
{
	if (fDone || (fCurrentStaff != fTargetStaff)) return;
	fOctava = o->getOctava();
}
