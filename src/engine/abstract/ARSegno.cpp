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

#include "ARSegno.h"
#include "TimeUnwrap.h"

ARSegno::ARSegno() : ARJump ("[segno]") {}
ARSegno::~ARSegno() {}

void ARSegno::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kSegno);
}

void ARSegno::printName(std::ostream& os) const
{
    os << "ARSegno";
    ARMusicalTag::printName(os);
}
