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

#include "GRRange.h"
#include "GRStaff.h"
#include "GRStaccato.h"
#include "GREvent.h"
// #include "GRStdNoteHead.h"


GRRange::GRRange(GRStaff * pstaff,ARMusicalTag * mtag)
		: GRARNotationElement(mtag),
		GRPositionTag(dynamic_cast<ARPositionTag *>(mtag)->getEndPosition(), mtag) //, tinfo(typeid(*mtag))
{
	assert(pstaff);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = pstaff->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);
}

GRRange::~GRRange()
{
		// associated is handled in
		// GRNotationElement ...
	//	if (associated)
	//		delete associated;
	//	associated = NULL;
}

//const ARTFermata * GRTFermata::getARTFermata() const
//{
//	return dynamic cast<ARTFermata *>(abstractRepresentation);
//}

/** \brief Called when an event is created while a GRRange is active.
*/
void GRRange::addAssociation(GRNotationElement * el)
{
	GREvent * grev = GREvent::cast(el);
	// now add a Real Element...
	// depending on the type...
	if (grev)
	{
		grev->addArticulation( /*dynamic*/ static_cast<ARMusicalTag *>(mAbstractRepresentation));
	}
}

void GRRange::removeAssociation(GRNotationElement * el)
{
	// assert(false);
	// do nothing, all those notes will be automatically
	// deleted ...
}

void GRRange::OnDraw( VGDevice & hdc) const
{

}

void GRRange::print() const
{

}
