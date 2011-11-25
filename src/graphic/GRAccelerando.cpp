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
	GRPTagARNotationElement::OnDrawText( hdc, "accel" );
}

void GRAccelerando::tellPosition(GObject * caller, const NVPoint & np)
{
	if (caller == /*dynamic cast<GObject *>*/(getAssociations()->GetHead())) // useless dynamic cast ?
		setPosition(np);

}
