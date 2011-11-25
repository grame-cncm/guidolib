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

#include "ARInstrument.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARInstrument::ltpls(1);

ARInstrument::ARInstrument()
{
	s1 = 0;
	s2 = 0;
	transp = 0;
}

ARInstrument::~ARInstrument()
{
	delete s1;
	delete s2;
	delete transp;
}
void ARInstrument::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail (( "S,name,,r;S,midiname,,o;S,transp,,o"));
		CreateListOfTPLs(ltpls,lstrs);

	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			s1 = TagParameterString::cast(rtpl->RemoveHead());

			s2 = TagParameterString::cast(rtpl->RemoveHead());

			transp = TagParameterString::cast(rtpl->RemoveHead());
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

const char* ARInstrument::getName() const
{
	if (!s1) return NULL;
	return s1->getValue();
}

const char* ARInstrument::getSecondName() const
{
	if (!s2) return NULL;
	return s2->getValue();
}

void ARInstrument::print() const
{
}

std::ostream & ARInstrument::operator<<(std::ostream &os) const
{
	return os << "\\instr<\"" << getName() << "\">";
}

