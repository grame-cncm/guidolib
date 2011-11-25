/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/

#include "ARUserChordTag.h"

#include "GRStaff.h"
// #include "GREmpty.h"
#include "GRSpring.h"
#include "GRSystem.h"
#include "GRSystemTag.h"
#include "GRChordTag.h"
#include "GRVoice.h"


GRChordTag::GRChordTag(GRStaff * grstaff, ARUserChordTag * pchordtag )
						: GRPTagARNotationElement(pchordtag)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;

	sse->grsystem = grstaff->getGRSystem();
	sse->startElement = NULL;
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endElement = NULL;
	sse->p = NULL;

	mStartEndList.AddTail(sse);
}

GRChordTag::~GRChordTag()
{
}

void GRChordTag::addAssociation(GRNotationElement * grnot)
{
//	GRPTagARNotationElement::addAssociation(grnot);
}

void GRChordTag::removeAssociation(GRNotationElement * grnot)
{
//	GRPTagARNotationElement::removeAssociation(grnot);
}

void GRChordTag::RangeEnd(GRStaff * grstaff)
{
}

void GRChordTag::tellPosition(GObject * obj, const NVPoint & pt)
{
}

/** \brief Called when the linked GRSystemTag gets a position-update
*/
void GRChordTag::checkPosition(const GRSystem * grsys)
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grsys);
	if (!sse) return;

	assert(sse);
	mIsSystemCall = true;	
	// ...
	mIsSystemCall = false;
}

const char* GRChordTag::getLabel() const
{
	const ARUserChordTag * puct = dynamic_cast<const ARUserChordTag *>(getAbstractRepresentation());
	return puct ? puct->getLabelValue() : "";
}
