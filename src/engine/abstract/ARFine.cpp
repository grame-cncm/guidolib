/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "ARFine.h"
#include "TimeUnwrap.h"

ARFine::ARFine() : ARJump ("Fine") {}
ARFine::~ARFine() {}

void ARFine::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kFine);
}

void ARFine::printName(std::ostream& os) const
{
    os << "ARFine";
}

void ARFine::printGMNName(std::ostream& os) const
{
    os << "\\fine";
}
