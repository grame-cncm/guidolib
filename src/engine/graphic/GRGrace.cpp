/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARGrace.h"
#include "GRGrace.h"
#include "GRStaff.h"


GRGrace::GRGrace( GRStaff * stf, ARGrace * argrc )
					: GRARCompositeNotationElement(argrc),
						GRPositionTag(argrc->getEndPosition(), argrc )
{
	assert(stf);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = stf->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);
}

GRGrace::~GRGrace()
{
}

void GRGrace::OnDraw( VGDevice & hdc) const
{
	if(!mDraw)
		return;
	if( mAssociated->size() == 1 ) // why 1 ?
		DrawSubElements( hdc );
}

void GRGrace::addAssociation(GRNotationElement *p )
{
	GRARCompositeNotationElement::addAssociation(p);
}

GuidoPos GRGrace::AddTail(GRNotationElement * el)
{
	GuidoPos mypos = GRARCompositeNotationElement::AddTail(el);

	// now, we remove the associatons ...
	// of those elements, that are members of 
	// the composite-sructure ...

	GuidoPos elpos = NULL;
	if (mAssociated && (elpos = mAssociated->GetElementPos(el)) != NULL)
	{
		mAssociated->RemoveElementAt(elpos);
	}

	return mypos;
}

/*
void GRGrace::setHPosition( GCoord nx)
{
	GRARCompositeNotationElement::setHPosition(nx);
}

void GRGrace::setPosition(const NVPoint &np)
{
	GRARCompositeNotationElement::setPosition(np);
}
*/

void GRGrace::tellPosition(GObject *caller, const NVPoint & inPos )
{
	/*if (mAssociated->GetCount() != 1)
	{
		GuidoWarn("More than one association for GRGrace");
		return;
	}
*/
	// this is called by the "super-event"

	const float graceOffset = 60; // Hardcoded

	setHPosition( inPos.x - graceOffset );
	
	// mAssociated->setPosition(position);
}
