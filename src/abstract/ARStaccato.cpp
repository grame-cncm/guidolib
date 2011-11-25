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
#include "ARStaccato.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

ListOfTPLs ARStaccato::ltpls(1);

void ARStaccato::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,type,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				if (str->getValue() == std::string("heavy"))
					type = HEAVY;
				else type = REGULAR;
			}
			delete str;
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARStaccato::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kStaccato);
}

void ARStaccato::print() const
{
}

void ARStaccato::PrintName(std::ostream &os) const
{
	os << "\\stacc";
	if (getRange()) os << "(";
}

void ARStaccato::PrintParameters(std::ostream &) const
{
}

bool ARStaccato::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() && !strcmp("\\staccEnd",s))
		return 1;
	return 0;
}
