/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <iostream>
#include "ARCoda.h"
#include "TimeMapper.h"

ARCoda::ARCoda() : ARJump ("[coda] Coda") {}
ARCoda::~ARCoda() {}

void ARCoda::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kCoda);
}

void ARCoda::printName(std::ostream& os) const
{
    os << "ARCoda";
    ARMusicalTag::printName(os);
}
