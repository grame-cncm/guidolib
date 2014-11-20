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
#include <cstring>	// for strcmp
#include "ARTie.h"
#include "TimeUnwrap.h"

ARTie::ARTie() : ARBowing()
{
}

ARTie::~ARTie()
{
}

void ARTie::print(std::ostream& os) const
{
    os << "ARTie";

	if (!getRange())
		os <<": Begin";

    os << ";" << std::endl;
}

// --------------------------------------------------------------------------
void ARTie::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kTie);
}

void ARTie::PrintName(std::ostream & os) const
{
	os << "\\tie";

	if (!getRange())
		os <<"Begin";
}

bool ARTie::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\tieEnd",s))
		return 1;
	return 0;
}
