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

#include <iostream>
#include  "kf_ilist.h"	// for GRMultipleGRObject

#include "ARMusicalObject.h"
#include "ARMusicalTag.h"

#include "GObject.h"	// for GRMultipleGRObject template instanciation. 
#include "GRMultipleGRObject.h"

// Class ARMusicalObject 

ARMusicalObject::ARMusicalObject()
		: relativeTimePosition(MIN_TIMEPOSITION), duration(DURATION_0), fVoiceNum(0), mGrObject(NULL)
{
}

ARMusicalObject::ARMusicalObject(const TYPE_TIMEPOSITION & relativeTimeposition)
		: relativeTimePosition(relativeTimeposition), duration(DURATION_0), fVoiceNum(0), mGrObject(NULL)
{
	assert(relativeTimePosition >= MIN_TIMEPOSITION);
}

ARMusicalObject::ARMusicalObject(const ARMusicalObject & armo)
{
	relativeTimePosition = armo.relativeTimePosition;
	// export = armo.export;
	duration = armo.duration;
	fVoiceNum = 0;

	assert(armo.mGrObject == NULL);
	mGrObject = NULL;
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
	return relativeTimePosition + duration; // EndTimeposition==relativeTimeposition,
	// da duration==0 !
}

void ARMusicalObject::print() const
{
	TYPE_DURATION dur = getDuration();
	TYPE_TIMEPOSITION tpos = getRelativeTimePosition();
	std::cout << "ARMusicalObject: duration: " 
		<<  dur.getNumerator() << '/' << dur.getDenominator()
		<< " time pos: " 
		<<  tpos.getNumerator() << '/' << tpos.getDenominator() << '\n';
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

