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

