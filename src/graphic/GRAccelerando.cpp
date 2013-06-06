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

#include "ARAccelerando.h"
#include "GRStaff.h"
#include "GRAccelerando.h"
#include "VGDevice.h"
// #include "NEPointerList.h"

GRAccelerando::GRAccelerando( GRStaff * inStaff, ARAccelerando * artrem )
  : GRPTagARNotationElement(artrem)
{
  assert(inStaff);
  GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
  sse->grsystem = inStaff->getGRSystem(); 
  sse->startflag = GRSystemStartEndStruct::LEFTMOST;
  
  sse->p = (void *) getNewGRSaveStruct();
  
  mStartEndList.AddTail(sse);

}

GRAccelerando::~GRAccelerando()
{
//	if (mAssociated)
//	{
//	}
}

unsigned int GRAccelerando::getTextAlign() const
{
	return (VGDevice::kAlignLeft | VGDevice::kAlignTop);
}

void GRAccelerando::OnDraw( VGDevice & hdc ) const
{
	if(mDraw)
		GRPTagARNotationElement::OnDrawText( hdc, "accel" );
}

void GRAccelerando::tellPosition(GObject * caller, const NVPoint & np)
{
	if (caller == /*dynamic cast<GObject *>*/(getAssociations()->GetHead())) // useless dynamic cast ?
		setPosition(np);

}
