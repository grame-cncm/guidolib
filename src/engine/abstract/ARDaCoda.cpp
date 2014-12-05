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
#include "ARDaCoda.h"
#include "TimeUnwrap.h"

ARDaCoda::ARDaCoda() : ARJump ("to Coda [coda]") {}
ARDaCoda::~ARDaCoda() {}

void ARDaCoda::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCoda);
}

void ARDaCoda::printName(std::ostream& os) const
{
    os << "ARDaCoda";
}

void ARDaCoda::printGMNName(std::ostream& os) const
{
    os << "\\daCoda";
}
