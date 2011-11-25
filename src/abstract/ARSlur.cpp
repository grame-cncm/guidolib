/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
