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
#include "ARRest.h"
#include "TimeUnwrap.h"

ARRest::ARRest(const TYPE_DURATION & durationOfRest)
  : ARMusicalEvent(MIN_TIMEPOSITION,(TYPE_DURATION) durationOfRest)
{
}

ARRest::ARRest(const TYPE_TIMEPOSITION & starttimeOfRest,
			   const TYPE_DURATION & durationOfRest)
  : ARMusicalEvent((TYPE_TIMEPOSITION) starttimeOfRest,(TYPE_DURATION) durationOfRest)
{
}

ARRest::ARRest(int theNumerator, int theDenominator)
  : ARMusicalEvent(theNumerator,theDenominator)
{
}

ARRest::ARRest(const ARRest &arrest)
: ARMusicalEvent( (const ARMusicalEvent &) arrest)
{
}

ARRest::~ARRest()
{
}

ARMusicalObject * ARRest::Copy() const
{
	return new ARRest(*this);
}

void ARRest::browse(TimeUnwrap& mapper) const
{
//	std::cout << getRelativeTimePosition() << " ARRest::browse duration: " << getDuration() << std::endl;
	mapper.AtPos (this, TimeUnwrap::kRest);
}

void ARRest::print() const
{
}

std::ostream & ARRest::operator<<(std::ostream &os) const
{
	return (os << "_*" << 
		duration.getNumerator() << "/" <<
		duration.getDenominator() << " ");
}






