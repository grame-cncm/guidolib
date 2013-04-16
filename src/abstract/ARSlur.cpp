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
#include <string.h>
#include "ARSlur.h"

#include "TimeUnwrap.h"

ARSlur::ARSlur(const ARSlur * slr) : ARBowing(slr)
{
}

ARSlur::ARSlur()
{
}

ARSlur::~ARSlur()
{
}

ARMusicalObject * ARSlur::Copy() const
{
	return new ARSlur(this);
}


// --------------------------------------------------------------------------
void ARSlur::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kSlur);
}

void ARSlur::print() const
{
}

void ARSlur::PrintName(std::ostream &os) const
{
	os << "\\slur";
	if (!getRange())
		os <<"Begin";
}

bool ARSlur::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\slurEnd",s))
		return 1;
	return 0;

}
