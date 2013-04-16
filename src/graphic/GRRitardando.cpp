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

#include "ARRitardando.h"
#include "GRStaff.h"
#include "GRRitardando.h"
// #include "NEPointerList.h"

#include "VGDevice.h"

// extern GFontRef hfonttext;

GRRitardando::GRRitardando( GRStaff * stf, ARRitardando * artrem )
					 : GRPTagARNotationElement(artrem)			
{
	assert(stf);
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = stf->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);
}

GRRitardando::~GRRitardando()
{
	// associations are removed within the GRPositionTag-Removal ...
	delete mAssociated; // still usefull ?
	mAssociated = NULL;
}

void GRRitardando::OnDraw(VGDevice & hdc) const
{
	
	GRPTagARNotationElement::OnDrawText( hdc, "rit" );

/*  const char *cp = "rit";
  HFONT hfontold = (HFONT) SelectObject(hdc,hfonttext);
  int nBackmode = GetBkMode(hdc);
  UINT ta = GetTextAlign(hdc);
  SetBkMode(hdc,TRANSPARENT);
  SetTextAlign(hdc,VGDevice::kAlignLeft | VGDevice::kAlignTop );
  TextOut(hdc,position.x,position.y,
	  	cp,lstrlen(cp));
  SetTextAlign(hdc,ta);
  SetBkMode(hdc,nBackmode);
  SelectObject(hdc,hfontold);
*/
}

void GRRitardando::tellPosition(GObject * caller, const NVPoint & np)
{
	if (caller == /*dynamic cast<GObject *>*/(mAssociated->GetHead()))
		setPosition(np);

}

unsigned int GRRitardando::getTextAlign() const
{
	return VGDevice::kAlignLeft | VGDevice::kAlignTop; 
}

