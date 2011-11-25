/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Here is the implementation of several musical tags.

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

// ============================================================================
ARBase::ARBase() 
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

void 
ARBase::PrintName( std::ostream & os) const
{
	os << "\\baseBegin";
}

void 
ARBase::PrintParameters( std::ostream & os) const
{
	os << "<" << base.getNumerator() << "," <<
		base.getDenominator() << "," <<
		dur.getNumerator() << "," <<
		dur.getDenominator() << ">";
}

void 
ARBase::addEvent(const ARMusicalEvent * ev)
{
	// this adds an event to the current base
	dur += ev->getDuration();
}

// ============================================================================
std::ostream & 
ARNewPage::operator<<(std::ostream &os) const
{
	os << "\\newPage";
	if (getRange())
		os << "(";
	return os << " ";
}

// ============================================================================

ARRepeatEndRangeEnd::ARRepeatEndRangeEnd( ARRepeatEnd * in ) : mRepeatEnd( in ) 
{ 
}

int 
ARRepeatEndRangeEnd::getNumRepeat() const
{ 
	return mRepeatEnd ? mRepeatEnd->getNumRepeat() : -1;
}

void
ARRepeatEndRangeEnd::setTagParameterList(TagParameterList & pl)
{
	pl.RemoveAll();
}

// ============================================================================
ARAutoBeam::ARAutoBeam()
{
	setAssociation(ARMusicalTag::RA);
	isAuto = true;
}

void 
ARAutoBeam::PrintName(std::ostream & os) const
{
	os << "\\autobeamBegin";
}

// ============================================================================
void
ARAutoBeamEnd::PrintName(std::ostream & os) const
{
	os << "\\autobeamEnd ";
}

// ============================================================================
std::ostream & 
ARMerge::operator<<(std::ostream & os) const
{
	os << "\\merge";
	if (getRange())
		os << "(";

	return os << " ";
}

void 
ARMerge::setError(int i)
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

// ============================================================================
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

ARMusicalObject * 
ARDummyRangeEnd::Copy() const
{
	return new ARDummyRangeEnd(this);
}

void 
ARDummyRangeEnd::PrintName(std::ostream & os) const
{
	os << endstr.c_str();
}

// ============================================================================
void 
ARStaffOn::setTagParameterList(TagParameterList & pl)
{
	pl.RemoveAll();
}

std::ostream & 
ARStaffOn::operator<<(std::ostream & os) const
{
	return os << "\\staffOn ";
}

// ============================================================================
void 
ARStaffOff::setTagParameterList(TagParameterList & pl)
{
	pl.RemoveAll();
}

std::ostream & 
ARStaffOff::operator<<(std::ostream & os) const
{
	return os << "\\staffOff ";
}

// ============================================================================
void 
ARBeamState::PrintName(std::ostream & os) const
{
	os << "\\beams";
	if (state == AUTO)
		os << "Auto";
	else if (state == OFF)
		os << "Off";

}

// ============================================================================
std::ostream & 
ARPossibleBreak::operator<<(std::ostream & os) const
{
	os << "\\pBreak";
	os << "<" << value << ">";
	if (getRange())
		os << "(";
	return os << " ";
}

// ============================================================================
void 
ARSecondGlue::PrintName(std::ostream & os) const
{
	os << "\\secGlue ";
}

// ============================================================================
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
