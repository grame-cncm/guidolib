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
#include "ARFermata.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"
#include "TagParameterString.h"

#include "TimeUnwrap.h"


ListOfTPLs ARFermata::ltpls(1);

ARFermata::ARFermata()
{
	rangesetting = RANGEDC;
	type = REGULAR;
}

ARFermata::~ARFermata()
{
}

void ARFermata::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs;
		lstrs.AddTail("S,type,short,o;S,position,above,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			std::string shortstr ("short");
			std::string longstr ("long");
			std::string below ("below");
			if (str->TagIsSet())
			{
				if (shortstr == str->getValue())
					type = SHORT;
				else if (longstr == str->getValue())
					type = LONG;
				else type = REGULAR;
			}
			delete str;
		
			str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet() && (below == str->getValue()))
			{
				position = BELOW;
			}
			else  position = ABOVE;
			
			delete str;
		}

		delete rtpl;
	}

	tpl.RemoveAll();

}

// --------------------------------------------------------------------------
void ARFermata::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kFermata);
}


void ARFermata::PrintName(std::ostream &os) const
{
	os << "\\fermata";
}
void ARFermata::PrintParameters(std::ostream &) const
{
	// is this correct?
	//if (getRange())
	//	os << "(";
}

