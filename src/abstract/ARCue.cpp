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
#include "ARCue.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARCue::fTagParametersList(1);

ARCue::ARCue() : name (0)		{ rangesetting = ONLY; }
ARCue::~ARCue()					{ delete name; }

void ARCue::setTagParameterList(TagParameterList& tpl)
{
	if (fTagParametersList.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,name,,o");
		CreateListOfTPLs(fTagParametersList,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(fTagParametersList, tpl, &rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0) {
			delete name;
			name = TagParameterString::cast(rtpl->RemoveHead());
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}


void ARCue::PrintName(std::ostream & os) const
{
	os << "\\cue";
}

void ARCue::PrintParameters(std::ostream & os) const
{
	if (!name || name->TagIsNotSet()) return;
	os << "<name=\"" << name->getValue() << "\">";
}
