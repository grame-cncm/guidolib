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

#include "GRRange.h"
#include "GRStaff.h"
#include "GRStaccato.h"
#include "GREvent.h"


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

void GRRange::print(std::ostream& os) const
{

}
