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
#include "ARAccol.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARAccol::ltpls(1);

ARAccol::ARAccol()
{
	nid = NULL;
	sid = range = type = NULL;
}

ARAccol::~ARAccol()
{
	delete nid;
	delete sid;
	delete range;
	delete type;
}

void ARAccol::PrintName(std::ostream & os) const
{
	os << "\\accol";
}

void ARAccol::PrintParameters(std::ostream & ) const
{
	// todo !
}

void ARAccol::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,id,,r;S,range,,r;S,type,standard,o"));
		lstrs.AddTail(
			(
			"I,id,,r;S,range,,r;S,type,standard,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			sid = TagParameterString::cast(rtpl->RemoveHead());
			nid = NULL;

			range = TagParameterString::cast(rtpl->RemoveHead());
			type = TagParameterString::cast(rtpl->RemoveHead());
		}
		else if (ret == 1)
		{
			nid = TagParameterInt::cast(rtpl->RemoveHead());
			range = TagParameterString::cast(rtpl->RemoveHead());
			type = TagParameterString::cast(rtpl->RemoveHead());
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}
