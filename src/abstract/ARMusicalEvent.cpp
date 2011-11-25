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

#include <typeinfo>
#include <cassert>
#include <iostream>
using namespace std;

#include "ARMusicalEvent.h"


// ATTENTION kf has changed MUSICAL_MIN_NUMERATOR to 0 !
#include "ARDefine.h"
// #include "defines.h"

// Class ARMusicalEvent

ARMusicalEvent::ARMusicalEvent(int theNumerator, int theDenominator) : mPoints(0)
{
	if (theNumerator < MUSICAL_MIN_NUMERATOR)
		theNumerator = MUSICAL_MIN_NUMERATOR;
	else if (theNumerator > MUSICAL_MAX_NUMERATOR)
		theNumerator = MUSICAL_MAX_NUMERATOR;


	if (theDenominator < MUSICAL_MIN_DENOMINATOR)
		theDenominator = 1;

//	assert(theNumerator>=MUSICAL_MIN_NUMERATOR); //(JB) useless !?
//	assert(theDenominator>=MUSICAL_MIN_DENOMINATOR);
//	assert(theNumerator<=MUSICAL_MAX_NUMERATOR);

	duration.set( theNumerator,theDenominator);
}


ARMusicalEvent::ARMusicalEvent(const TYPE_TIMEPOSITION & relativeTimePositionOfEvent,
	const TYPE_DURATION & durationOfEvent)
  : ARMusicalObject(relativeTimePositionOfEvent)
{

	duration = durationOfEvent;
	mPoints = 0;
	assert(duration.getNumerator() >= MUSICAL_MIN_NUMERATOR);
	const int tmp = duration.getDenominator();
	if (tmp < MUSICAL_MIN_DENOMINATOR)
		assert(duration.getDenominator() >= MUSICAL_MIN_DENOMINATOR);
	// so now what .... take it out there
}

ARMusicalEvent::ARMusicalEvent(const TYPE_DURATION & durationOfEvent)
{
	duration = durationOfEvent;
	mPoints = 0;
}

ARMusicalEvent::ARMusicalEvent(const ARMusicalEvent & armev)
			: ARMusicalObject( (const ARMusicalObject &)armev )
{
	mPoints = armev.mPoints;
}

ARMusicalObject * ARMusicalEvent::Copy() const
{
	return new ARMusicalEvent(*this);
}

ARMusicalEvent::~ARMusicalEvent()
{
}

void ARMusicalEvent::print() const
{
}

void ARMusicalEvent::setDenominator(int newDenominator)
{
	if (!(newDenominator>=MUSICAL_MIN_DENOMINATOR))
	{
		GuidoWarn("Denominator is too small" );
		newDenominator= MUSICAL_MIN_DENOMINATOR;
	}
	duration.setDenominator(newDenominator);
}

void ARMusicalEvent::setNumerator(int newNumerator)
{
	if (newNumerator<MUSICAL_MIN_NUMERATOR)
	{
		newNumerator = MUSICAL_MIN_NUMERATOR;
	}
	else if (newNumerator>MUSICAL_MAX_NUMERATOR)
	{
		newNumerator = MUSICAL_MAX_NUMERATOR;
	}
//	assert(newNumerator>=MUSICAL_MIN_NUMERATOR);
//	assert(newNumerator<=MUSICAL_MAX_NUMERATOR);
	duration.setNumerator(newNumerator);
// ACHTUNG modified by  kf for WCHT ...
//	assert(duration>MIN_DURATION);
  //## end ARMusicalEvent::setNumerator%858535960.body
}

void ARMusicalEvent::setPoints( int pointCount )
{
	assert(duration > MIN_DURATION);
//	assert(pointCount > 0);
	assert(pointCount < 4);

	mPoints = pointCount;
	TYPE_DURATION d (duration);

	for(int i=1,n=1;i<=mPoints;i++)
	{
		n <<= 1;
		duration += d * TYPE_DURATION( 1, n );
	}
}

/** \brief Returns the number of duration dots of a note or rest.
*/
int ARMusicalEvent::getPoints() const
{
	return mPoints;
}

/** \brief Sets the points of the event without changing the duration.
*/
void ARMusicalEvent::setPointsNoDurationChange( int pointCount )
{
	mPoints = pointCount;
}

/** \brief Determines whether two events can be merged with the merge-tag
*/
bool ARMusicalEvent::CanBeMerged(const ARMusicalEvent *ev2)
{
	// this makes a simple type-case
	// sufficient for rests ...
	return !(typeid(*this).operator!=(typeid(*ev2)));
}
