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

#include <cassert>
#include <iostream>

#include "ARMusicalObject.h"
#include "ARMusicalTag.h"
#include "ARVisitor.h"


ARMusicalObject::ARMusicalObject()
		: relativeTimePosition(MIN_TIMEPOSITION), fVoiceNum(0), fDrawGR(true), /* mGrObject(NULL),*/ fDuration(DURATION_0)
{
}

ARMusicalObject::ARMusicalObject(const TYPE_TIMEPOSITION & relativeTimeposition)
		: relativeTimePosition(relativeTimeposition), fVoiceNum(0), fDrawGR(true), /*mGrObject(NULL),*/ fDuration(DURATION_0)
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
	fDrawGR = armo.fDrawGR;
}


void ARMusicalObject::accept(ARVisitor& visitor)
{
	visitor.visitIn (this);
	visitor.visitOut(this);
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

void ARMusicalObject::print(std::ostream & os) const
{
	const ARMusicalTag * mt = dynamic_cast<const ARMusicalTag *>(this);
	if (mt) os << mt;
}

std::ostream & operator<<(std::ostream& os, const ARMusicalObject* o)
{
	if (o) o->print(os);
	return os;
}

void ARMusicalObject::setRelativeTimePosition(const TYPE_TIMEPOSITION & newRelativeTimePosition)
{
	assert(newRelativeTimePosition>=DURATION_0);
	relativeTimePosition = newRelativeTimePosition;
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
