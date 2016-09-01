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
    fVoiceNum = arrest.getVoiceNum(); // Added to fix a bug during rest copy (in doAutoBarlines)
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

void ARRest::printName(std::ostream& os) const
{
    os << "ARRest";
}

void ARRest::printGMNName(std::ostream& os) const
{
    os << "_" << "*" << getDuration().getNumerator() << "/" << getDuration().getDenominator();
}

void ARRest::printParameters(std::ostream& os) const
{
    os << "duration: " << getDuration().getNumerator() << "/" << getDuration().getDenominator() << "; ";
}






