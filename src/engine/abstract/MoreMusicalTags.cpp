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
    ARMusicalTag::printName(os);
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
    ARMusicalTag::printName(os);
}

void ARRepeatEndRangeEnd::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARAutoBeam ====================

ARAutoBeam::ARAutoBeam()
{
	setAssociation(ARMusicalTag::RA);
	isAuto = true;
}

void ARAutoBeam::printName(std::ostream& os) const
{
    os << "ARAutoBeam";
    ARMusicalTag::printName(os);
}

void ARAutoBeam::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARAutoBeamEnd ====================

void ARAutoBeamEnd::printName(std::ostream& os) const
{
    os << "ARAutoBeamEnd";
    ARMusicalTag::printName(os);
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
    ARMusicalTag::printName(os);
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
    os << "ARDummyRangeEnd";
    ARMusicalTag::printName(os);
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
    ARMusicalTag::printName(os);
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
    ARMusicalTag::printName(os);
}

void ARStaffOff::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

// ==================== ARBeamState ====================

void ARBeamState::printName(std::ostream& os) const
{
    os << "ARBeamState";
    ARMusicalTag::printName(os);
}

void ARBeamState::printParameters(std::ostream& os) const
{
    os << "state:" << (state == AUTO ? "Auto" : "off");
    os << ";";

    ARMusicalTag::printParameters(os);
}

// ==================== ARSecondGlue ====================

void ARSecondGlue::printName(std::ostream& os) const
{
    os << "ARSecondGlue";
    ARMusicalTag::printName(os);
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
