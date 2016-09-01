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
#include "ARDaCapo.h"
#include "TimeUnwrap.h"

ARDaCapo::ARDaCapo() : ARJump ("D.C.") {}
ARDaCapo::~ARDaCapo() {}

void ARDaCapo::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCapo);
}

void ARDaCapo::printName(std::ostream& os) const
{
    os << "ARDaCapo";
}

void ARDaCapo::printGMNName(std::ostream& os) const
{
    os << "\\daCapo";
}

ARDaCapoAlFine::ARDaCapoAlFine() : ARJump ("D.C. al Fine") {}
ARDaCapoAlFine::~ARDaCapoAlFine() {}

void ARDaCapoAlFine::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCapoAlFine);
}

void ARDaCapoAlFine::printName(std::ostream& os) const
{
    os << "ARDaCapoAlFine";
}

void ARDaCapoAlFine::printGMNName(std::ostream& os) const
{
    os << "\\daCapoAlFine";
}
