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

#include <string.h>
#include <iostream>

#include "ARAccelerando.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

using namespace std;

ListOfTPLs ARAccelerando::ltpls(1);

ARAccelerando::~ARAccelerando()
{
	delete s1;
	delete s2;

	// delete TagParameterPointer ...
}

void ARAccelerando::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,tempo,,o;S,abstempo,,o"
			));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
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

void ARAccelerando::print() const
{
}

void ARAccelerando::PrintName(ostream &os) const
{
	if (getRange())
		os << "\\accel";
	else 
		os << "\\accelBegin";
}

void ARAccelerando::PrintParameters(ostream &os) const
{
		//	"S,tempo,,o;S,abstempo,,o"
	int first = 1;
	if (s1 && s1->TagIsSet())
	{
		if (first)
		{
			os << "<";
			first = 0;
		}


		os << "\"" << s1->getValue() << "\"";

	}
	if (s2 && s2->TagIsSet())
	{
		if (first)
		{
			os << "<";
			first = 0;
		}
		else 
			os << ",";
		os << "\"" << s2->getValue() << "\"";

	}
	if (!first)
		os << ">";
}


bool ARAccelerando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() &&
		!strcmp("\\accelEnd",s))
		return 1;
	return 0;
}
