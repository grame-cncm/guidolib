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

#include "ARTremolo.h"
#include "GRStaff.h"
#include "GRTremolo.h"
#include "VGDevice.h"
// #include "NEPointerList.h"


GRTremolo::GRTremolo( GRStaff * stf, ARTremolo * artrem )
					 	: GRPTagARNotationElement(artrem)				
{
	assert(stf);
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = stf->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);

}

GRTremolo::~GRTremolo()
{
}

void GRTremolo::OnDraw( VGDevice & hdc ) const
{
	
	GRPTagARNotationElement::OnDrawText(hdc,"trem.");
}

void GRTremolo::tellPosition(GObject * caller, const NVPoint &np)
{
	if (caller == /*dynamic cast<GObject *>*/(mAssociated->GetHead()))
		setPosition(np);

}

unsigned int GRTremolo::getTextAlign() const
{ 
	return (VGDevice::kAlignLeft | VGDevice::kAlignTop); 
}
