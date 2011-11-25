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
#include "ARRitardando.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARRitardando::ltpls(1);

ARRitardando::ARRitardando() : ARMTParameter()
{
	rangesetting = ONLY;
	s1 = 0;
	s2 = 0;
}

ARRitardando::~ARRitardando() 
{
	delete s1;
	delete s2;
};

void ARRitardando::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			( "S,tempo,,o;S,abstempo,,o" ));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// Get The TagParameters ...
			s1 = TagParameterString::cast(rtpl->RemoveHead());
			//assert(text);
			s2 = TagParameterString::cast(rtpl->RemoveHead());
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARRitardando::print() const
{
}

void ARRitardando::PrintName(std::ostream &os) const
{
	os << "\\rit";
	if (!getRange())
		os << "Begin";
//	if (getRange()) os << "(";

}
void ARRitardando::PrintParameters(std::ostream & ) const
{
}


bool ARRitardando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;

	if (!getRange() && !strcmp("\\ritEnd",s))
		return true;
	return false;
}
