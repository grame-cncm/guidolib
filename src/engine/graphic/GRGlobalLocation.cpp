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

#include "ARShareLocation.h"
#include "GREvent.h"
#include "GRGlobalLocation.h"
#include "GRGlobalStem.h"
#include "GRStaff.h"
#include "GRStdNoteHead.h"
#include "GRVoice.h"

GRGlobalLocation::GRGlobalLocation( GRStaff * grstaff, const ARShareLocation * pshare )
: GRPTagARNotationElement(pshare)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;

	sse->grsystem = grstaff->getGRSystem();
	sse->startElement = NULL;
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endElement = NULL;
	sse->p = NULL;

	addSystemStartEndStruct (sse);
	mFirstEl = NULL;
}

GRGlobalLocation::~GRGlobalLocation()
{
	// we just remove any association manually
	if (mAssociated) {
		GuidoPos pos = mAssociated->GetHeadPosition();
		while(pos) {
			GRNotationElement * el = mAssociated->GetNext(pos);
			if( el ) el->removeAssociation(this);
		}
	}
	if (mFirstEl)
		mFirstEl->removeAssociation(this);
}

void GRGlobalLocation::removeAssociation(GRNotationElement * grnot)
{
    if (grnot == mFirstEl)
	{
		GRNotationElement::removeAssociation(grnot);
		mFirstEl = NULL;
	}
	else
		GRPTagARNotationElement::removeAssociation(grnot);
}

void GRGlobalLocation::addAssociation(GRNotationElement * grnot)
{
	// grnot->getNeedsSpring() is zero, if an other
	// location-grabber has been already active.
	// (that is a \shareStem within a \shareLocation range)
	// then, the \shareStem-Tag is responsible for
	// setting the position. -> the first event 
	// of a shareStem is then associated with the
	// global shareLocation tag.
	if (grnot->getNeedsSpring() == 0) return;

	GREvent * ev = NULL;
	if ( (ev = GREvent::cast(grnot)) != NULL)
	{
		// take the length ...
		ev->setGlobalLocation(this);
	}

	// This sets the first element in the range ...
	if (!mFirstEl)
	{
		// this associates the first element with this tag ....
		mFirstEl = grnot;
		mFirstEl->addAssociation(this);
		return;

		// the firstElement is not added to the associated ones -> it does not have to told anything!?
	}

	
	GRNotationElement::addAssociation(grnot);
	GRPositionTag::addAssociation(grnot);

	// this is needed, because the share location can
	// be used in different staves. Elements that need
	// be told of the location can be deleted before this
	// tag, therefor we must know, if these elements are
	// deleted. 
	// The recursive cycle with tellPosition is broken,
	// because only the first element really sets the
	// position. And the first is not included in the
	// own associated list.
	grnot->addAssociation(this);
}

void GRGlobalLocation::RangeEnd(GRStaff *grstaff)
{
	GRPTagARNotationElement::RangeEnd(grstaff);
}

void GRGlobalLocation::setHPosition( GCoord nx)
{
	// the first tells the element itself of its new position
	// and also the associated elements ... 
	GRPTagARNotationElement::setHPosition(nx + getOffset().x);
}

void GRGlobalLocation::OnDraw( VGDevice & hdc) const
{
}

void GRGlobalLocation::tellPosition( GObject * obj, const NVPoint &pt)
{
	if (dynamic_cast<GRNotationElement *>(obj) == mFirstEl) // useless cast ?
	{
		setHPosition(pt.x);
	}
}

int GRGlobalLocation::getHighestAndLowestNoteHead (GRStdNoteHead ** highest, GRStdNoteHead ** lowest) const
{
	*highest = *lowest = 0;
	if (mFirstEl) {
		// find the GRGlobalStem
		const NEPointerList * plist = mFirstEl->getAssociations();
		if (plist) {
			GuidoPos pos = plist->GetHeadPosition();
			while (pos) {
				GRGlobalStem * gstem = dynamic_cast<GRGlobalStem *>(plist->GetNext(pos));
				if (gstem) {
					gstem->getHighestAndLowestNoteHead(highest,lowest);
					return gstem->getStemDir();
				}
			}
		}
	}

	// then we have not found it yet ....
	// check my own associations ....
	const NEPointerList * plist = mAssociated;
	if (plist) {
		GuidoPos pos = plist->GetHeadPosition();
		while (pos) {
			GRGlobalStem * gstem = dynamic_cast<GRGlobalStem *>(plist->GetNext(pos));
			if (gstem) {
				gstem->getHighestAndLowestNoteHead(highest,lowest);
				return gstem->getStemDir();
			}
		}
	}
	return 0;
}
