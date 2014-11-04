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

	setDuration ( Fraction(theNumerator,theDenominator) );
}


ARMusicalEvent::ARMusicalEvent(const TYPE_TIMEPOSITION & relativeTimePositionOfEvent,
	const TYPE_DURATION & durationOfEvent)
  : ARMusicalObject(relativeTimePositionOfEvent)
{
	mPoints = 0;
	assert(durationOfEvent.getNumerator() >= MUSICAL_MIN_NUMERATOR);
	const int tmp = durationOfEvent.getDenominator();
	if (tmp < MUSICAL_MIN_DENOMINATOR)
		assert(durationOfEvent.getDenominator() >= MUSICAL_MIN_DENOMINATOR);
	// so now what .... take it out there
	setDuration ( durationOfEvent );
}

ARMusicalEvent::ARMusicalEvent(const TYPE_DURATION & durationOfEvent)
{
	setDuration ( durationOfEvent );
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
	TYPE_DURATION d (getDuration());
	d.setDenominator(newDenominator);
	setDuration(d);
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

	TYPE_DURATION d (getDuration());
	d.setNumerator(newNumerator);
	setDuration(d);

// ACHTUNG modified by  kf for WCHT ...
//	assert(duration>MIN_DURATION);
  //## end ARMusicalEvent::setNumerator%858535960.body
}

void ARMusicalEvent::setPoints( int pointCount )
{
	TYPE_DURATION d (getDuration());

	assert(d > MIN_DURATION);
//	assert(pointCount > 0);
	assert(pointCount < 4);

	mPoints = pointCount;
	TYPE_DURATION tmp (d);

	for(int i=1,n=1;i<=mPoints;i++)
	{
		n <<= 1;
		d += tmp * TYPE_DURATION( 1, n );
	}
	setDuration(d);
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
