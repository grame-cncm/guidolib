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
#ifdef WIN32
# pragma warning (disable : 4800)
#endif

#include <cassert>

#include "GRARNotationElement.h"
#include "ARMusicalObject.h"

GRARNotationElement::GRARNotationElement(const ARMusicalObject * arNotationElement, bool ownsAR )
  : mAbstractRepresentation( arNotationElement ),
	 mOwnsAR(ownsAR)
{
	assert(mAbstractRepresentation);
	mDurationOfGR = mAbstractRepresentation->getDuration(); 
	setRelativeTimePosition(mAbstractRepresentation->getRelativeTimePosition());
	mDraw = arNotationElement->getDrawGR();
}


GRARNotationElement::~GRARNotationElement()
{
	// remove the representation 
	if (mAbstractRepresentation)
	{
		if (mOwnsAR)
			delete mAbstractRepresentation;
		mAbstractRepresentation = 0;
	}
}

bool GRARNotationElement::isChordComma() const									{ return getAbstractRepresentation()->isARChordComma(); }
const ARMusicalObject * GRARNotationElement::getAbstractRepresentation()		{ return mAbstractRepresentation; }
const ARMusicalObject * GRARNotationElement::getAbstractRepresentation() const	{ return mAbstractRepresentation; }



