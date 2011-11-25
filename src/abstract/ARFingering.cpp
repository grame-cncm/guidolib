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
#include "ARFingering.h"
// #include "ARFactory.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

// ListOfTPLs ARFingering::ltpls(1);

ARFingering::ARFingering()
{
	rangesetting = ONLY;
}

ARFingering::~ARFingering()
{
}

const char * 
ARFingering::getTagFormat() const	 // (JB) new
{
	const char * const outFormat = "S,text,,r;U,dy,-2,o";
	return outFormat;
}

/* (JB) I did not find any specification for fingering tag
		format. I though is was usefull to have a dy, even for
		default positionning, which is certainly different from
		standard text. So, the fingering has now the same tag format
		than text, only the default value for dy changes
		(see getTagFormat())

void ARFingering::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "S,text,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	const int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			text = TagParameterString::cast (rtpl->RemoveHead());
			assert(text);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}
*/

void ARFingering::PrintName(std::ostream & os) const
{
		os << "\\fingering";
}

void ARFingering::PrintParameters(std::ostream & os) const
{
	if (!text) return;
		os << "<text=\"" << text->getValue() << "\">";
}

