/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
 
 
#include <iostream>
#include "ARHarmonic.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"


void ARHarmonic::printName(std::ostream& os) const
{
    os << "ARHarmonic";
}

void ARHarmonic::printGMNName(std::ostream& os) const
{
    os << "\\harmonic";
}

void ARHarmonic::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}
