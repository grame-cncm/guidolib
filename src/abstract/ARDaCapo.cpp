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
ARDaCapo::~ARDaCapo()   {}
void ARDaCapo::PrintName( std::ostream & os ) const  { os << "\\daCapo"; }

void ARDaCapo::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCapo);
}

ARDaCapoAlFine::ARDaCapoAlFine() : ARJump ("D.C. al Fine") {}
ARDaCapoAlFine::~ARDaCapoAlFine()   {}
void ARDaCapoAlFine::PrintName( std::ostream & os ) const  { os << "\\daCapoAlFine"; }

void ARDaCapoAlFine::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCapoAlFine);
}
