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
#include "ARAccent.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"


// --------------------------------------------------------------------------
void ARAccent::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kAccent);
}

void ARAccent::printName(std::ostream& os) const
{
	os << "ARAccent";
}

void ARAccent::printGMNName(std::ostream& os) const
{
    os << "\\accent";
}

void ARAccent::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}
