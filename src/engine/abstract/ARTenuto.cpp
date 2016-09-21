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
#include "ARTenuto.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

// --------------------------------------------------------------------------
void ARTenuto::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kTenuto);
}

void ARTenuto::printName(std::ostream& os) const
{
	os << "ARTenuto";
}

void ARTenuto::printGMNName(std::ostream& os) const
{
    os << "\\tenuto";
}

void ARTenuto::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}


