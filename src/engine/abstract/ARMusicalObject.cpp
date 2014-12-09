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

#include <iostream>
#include  "kf_ilist.h"	// for GRMultipleGRObject

#include "ARMusicalObject.h"
#include "ARMusicalTag.h"

#include "GObject.h"	// for GRMultipleGRObject template instanciation. 
#include "GRMultipleGRObject.h"

// Class ARMusicalObject 

ARMusicalObject::ARMusicalObject()
		: relativeTimePosition(MIN_TIMEPOSITION), fVoiceNum(0), drawGR(true), mGrObject(NULL), fDuration(DURATION_0)
{
}

ARMusicalObject::ARMusicalObject(const TYPE_TIMEPOSITION & relativeTimeposition)
		: relativeTimePosition(relativeTimeposition), fVoiceNum(0), drawGR(true), mGrObject(NULL), fDuration(DURATION_0)
{
	assert(relativeTimePosition >= MIN_TIMEPOSITION);
}

ARMusicalObject::ARMusicalObject(const ARMusicalObject & armo)
{
	relativeTimePosition = armo.relativeTimePosition;
	// export = armo.export;
	fDuration = armo.fDuration;
	assert(fDuration >= DURATION_0);

	fVoiceNum = 0;

	assert(armo.mGrObject == NULL);
	mGrObject = NULL;
	drawGR = armo.drawGR;
}


ARMusicalObject::~ARMusicalObject()
{
	delete (GRMultipleGRObject *) mGrObject; // not very nice.
	mGrObject = 0;
}


// creates a copy of itself
ARMusicalObject * ARMusicalObject::Copy() const
{
	return new ARMusicalObject(*this);
}

TYPE_TIMEPOSITION ARMusicalObject::getRelativeEndTimePosition() const
{
	return relativeTimePosition + fDuration; // EndTimeposition==relativeTimeposition,
	// da duration==0 !
}

// provides a GUIDO/SALIERI-Conform output of the current object
std::ostream & ARMusicalObject::operator<< (std::ostream & ostream) const
{
	const ARMusicalTag * mt = dynamic_cast<const ARMusicalTag *>(this);
	if (mt)
	{
		return mt->operator <<(ostream);
	}
	return ostream;
}

void ARMusicalObject::setRelativeTimePosition(const TYPE_TIMEPOSITION & newRelativeTimePosition)
{
	assert(newRelativeTimePosition>=DURATION_0);
	relativeTimePosition = newRelativeTimePosition;
}

void ARMusicalObject::addGRRepresentation(GObject * p_grep)
{
	// add something to the grafical representation
	if (mGrObject == 0)
		mGrObject = new GRMultipleGRObject;

	((GRMultipleGRObject *) mGrObject)->AddTail(p_grep);
}

void ARMusicalObject::resetGRRepresentation()
{
	delete (GRMultipleGRObject *) mGrObject;
	mGrObject = 0;
}

GObject * ARMusicalObject::getFirstGRRepresentation()
{
	if (!mGrObject) return 0;

	return ((GRMultipleGRObject * ) mGrObject)->GetHead();
}

GObject * ARMusicalObject::getLastGRRepresentation()
{
	if (!mGrObject) return 0;

	return ((GRMultipleGRObject * ) mGrObject)->GetTail();

}

// void ARMusicalObject::setExport(int p_export)
//{
//	export = p_export;
//}

/** \brief Detaches a GR object from this AR object.
*/
void ARMusicalObject::removeGRRepresentation(GObject * p_grep)
{
	if (mGrObject == NULL) return;
	
	((GRMultipleGRObject *) mGrObject)->RemoveElement(p_grep);
}


/** \brief Determines wether the given fraction
	has a power of two in the denominator.
	
	the power is tested up to the maxpower ...
	The routine uses the fact, that the number is
	power of two, if there is exaclty ONE bit set.
	 2^0 = 0x0000001
	2^1 = 0x0000010
	2^2 = 0x0000100
*/
bool ARMusicalObject::IsPowerOfTwoDenom(const TYPE_DURATION & dur /*, int maxpower*/)
{
	/* was:	int val = dur.getDenominator();
	for (int i = 0; i <= maxpower; ++i)
	{
		if ( ((1 << i ) & val) == val)
			return true;
	}
	return false;
	*/

	// new:
	const int x = dur.getDenominator();
	// if( x == 0 ) return false; // because the denominator can't be 0
	return ((x & -x) == x);
}

void ARMusicalObject::print(std::ostream& os) const
{
    os << "print() needs to be implemented in ARMusicalObject subclasses" << std::endl;
}