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

#include <iostream>
#include <sstream>

#include "ARRest.h"
#include "TimeUnwrap.h"

using namespace std;

ARRest::ARRest(const ARRest &arrest)
: ARMusicalEvent( (const ARMusicalEvent &) arrest)
{
    fVoiceNum = arrest.getVoiceNum(); // Added to fix a bug during rest copy (in doAutoBarlines)
}

void ARRest::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kRest);
}

string ARRest::getGMNName() const
{
	stringstream s;
	s << "_*" << getDuration();
	return s.str();
}






