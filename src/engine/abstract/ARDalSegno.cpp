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
#include "ARDalSegno.h"
#include "TimeUnwrap.h"

ARDalSegno::ARDalSegno() : ARJump ("D.S.") {}
ARDalSegno::~ARDalSegno() {}

void ARDalSegno::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDalSegno);
}

void ARDalSegno::printName(std::ostream& os) const
{
    os << "ARDalSegno";
    ARMusicalTag::printName(os);
}

ARDalSegnoAlFine::ARDalSegnoAlFine() : ARJump ("D.S. al Fine") {}
ARDalSegnoAlFine::~ARDalSegnoAlFine() {}

void ARDalSegnoAlFine::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDalSegnoAlFine);
}

void ARDalSegnoAlFine::printName(std::ostream& os) const
{
    os << "ARDalSegnoAlFine";
    ARMusicalTag::printName(os);
}
