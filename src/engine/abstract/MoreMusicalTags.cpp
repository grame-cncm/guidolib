/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "TagParameterList.h"

#include "ARMusicalEvent.h"
#include "ARBase.h"
#include "ARNewPage.h"
#include "ARRepeatEndRangeEnd.h"
#include "ARRepeatEnd.h"
#include "ARAutoBeam.h"
#include "ARAutoBeamEnd.h"
#include "ARMerge.h"
#include "ARDummyRangeEnd.h"
#include "ARStaffOn.h"
#include "ARStaffOff.h"
#include "ARBeamState.h"
#include "ARPossibleBreak.h"
#include "ARSecondGlue.h"
#include "ARTagEnd.h"

// ==================== ARBase ====================

ARBase::ARBase() 
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

void ARBase::printName(std::ostream& os) const
{
    os << "ARBase";
}

void ARBase::printGMNName(std::ostream& os) const
{
    os << "\\base";
}

void ARBase::printParameters(std::ostream& os) const
{
	os << "base: " << base.getNumerator() << "/" << base.getDenominator();
    os << ", dur:" << dur.getNumerator()  << "/" << dur.getDenominator();
    
    os << ";";

    ARMusicalTag::printParameters(os);
}

void ARBase::addEvent(const ARMusicalEvent * ev)
{
	// this adds an event to the current base
	dur += ev->getDuration();
}

// ==================== ARRepeatEndRangeEnd ====================

ARRepeatEndRangeEnd::ARRepeatEndRangeEnd( ARRepeatEnd * in ) : mRepeatEnd( in ) 
{ 
}

void ARRepeatEndRangeEnd::setTagParameterList(TagParameterList & pl)
{
	pl.RemoveAll();
}

void ARRepeatEndRangeEnd::printName(std::ostream& os) const
{
    os << "ARRepeatEndRangeEnd";
}

void ARRepeatEndRangeEnd::printGMNName(std::ostream& os) const
{
    os << "\\repeatEndRangeEnd";
}

void ARRepeatEndRangeEnd::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARAutoBeam ====================

ARAutoBeam::ARAutoBeam(bool full)
{
	setAssociation(ARMusicalTag::RA);
	isAuto = true;
	fFullBeaming = full;
}

void ARAutoBeam::printName(std::ostream& os) const
{
    os << "ARAutoBeam";
}

void ARAutoBeam::printGMNName(std::ostream& os) const
{
    os << "\\autoBeam";
}

void ARAutoBeam::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARAutoBeamEnd ====================

void ARAutoBeamEnd::printName(std::ostream& os) const
{
    os << "ARAutoBeamEnd";
}

void ARAutoBeamEnd::printGMNName(std::ostream& os) const
{
    os << "\\autoBeamEnd";
}

void ARAutoBeamEnd::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARMerge ====================

void ARMerge::setError(int i)
{
	ARMusicalTag::setError(i);
	if (i!=0)
	{
		setAssociation(ARMusicalTag::ER);
		ARPositionTag * pt = getCorrespondence();
		if (pt)
		{
			ARMusicalTag * armt = dynamic_cast<ARMusicalTag *>(pt);
			if (armt)
				armt->setAssociation(ARMusicalTag::EL);
		}
	}
}

void ARMerge::printName(std::ostream& os) const
{
    os << "ARMerge";
}

void ARMerge::printGMNName(std::ostream& os) const
{
    os << "\\merge";
}

void ARMerge::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARDummyRangeEnd ====================

ARDummyRangeEnd::ARDummyRangeEnd(const ARDummyRangeEnd * copy)
	: ARTagEnd(-1,copy)
{
	if (copy)
		endstr = copy->endstr; 
}

ARDummyRangeEnd::ARDummyRangeEnd(const char * txt) 
{
	 if (txt)	 endstr = txt;
	 else		 endstr = ")";
};

ARMusicalObject * ARDummyRangeEnd::Copy() const
{
	return new ARDummyRangeEnd(this);
}

void ARDummyRangeEnd::printName(std::ostream& os) const
{
    os << "ARDummyRangeEnd;";
    if (mPositionTag) {
        os << " (";
        mPositionTag->printName(os);
        os << ")";
    }
}

void ARDummyRangeEnd::printGMNName(std::ostream& os) const
{
    mPositionTag->printGMNName(os);
    os << "End";
}

void ARDummyRangeEnd::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARStaffOn ====================

void ARStaffOn::setTagParameterList(TagParameterList & pl)
{
	pl.RemoveAll();
}

void ARStaffOn::printName(std::ostream& os) const
{
    os << "ARStaffOn";
}

void ARStaffOn::printGMNName(std::ostream& os) const
{
    os << "\\staffOn";
}

void ARStaffOn::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARStaffOff ====================

void ARStaffOff::setTagParameterList(TagParameterList & pl)
{
	pl.RemoveAll();
}

void ARStaffOff::printName(std::ostream& os) const
{
    os << "ARStaffOff";
}

void ARStaffOff::printGMNName(std::ostream& os) const
{
    os << "\\staffOff";
}

void ARStaffOff::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARBeamState ====================

void ARBeamState::printName(std::ostream& os) const
{
    os << "ARBeamState";
}

void ARBeamState::printGMNName(std::ostream& os) const
{
    os << "\\beamState";
}

void ARBeamState::printParameters(std::ostream& os) const
{
	os << "state:";
	switch (state) {
		case OFF:
			os << "off";
			break;
		case FULL:
			os << "full";
			break;
		case AUTO:
			os << "Auto";
			break;
		default:
			os << "unknown";
	}
//	os << "state:" << (state == AUTO ? "Auto" : "off");
    os << ";";

    ARMusicalTag::printParameters(os);
}

// ==================== ARSecondGlue ====================

void ARSecondGlue::printName(std::ostream& os) const
{
    os << "ARSecondGlue";
}

void ARSecondGlue::printGMNName(std::ostream& os) const
{
    os << "\\secondGlue";
}

void ARSecondGlue::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARTagEnd ====================

ARTagEnd::ARTagEnd(int id, const ARTagEnd * copy ) 
			: ARMTParameter(id,copy)
{
	setAssociation(ARMusicalTag::LA);
}

void ARTagEnd::setEndPosition(GuidoPos)
{
	assert(false);
}

GuidoPos ARTagEnd::getEndPosition() const
{
	assert(false);
	return (GuidoPos)0;
}
