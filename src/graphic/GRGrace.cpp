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
	if (mAssociated->GetCount() != 1)
	{
		GuidoWarn("More than one association for GRGrace");
		return;
	}

	// this is called by the "super-event"

	const float graceOffset = 60; // Hardcoded

	setHPosition( inPos.x - graceOffset );
	
	// mAssociated->setPosition(position);
}
