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

#include "ARComposer.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"


ListOfTPLs ARComposer::ltpls(1);


ARComposer::ARComposer()
{
	name = pageformat = NULL;
	rangesetting = NO;
}


ARComposer::~ARComposer()
{
	delete name;
	delete pageformat;
}



void ARComposer::print() const
{
}

void ARComposer::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,name,,r;"
			"S,pageformat,53,o"));
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
			name =  TagParameterString::cast(rtpl->RemoveHead());
			assert(name);

			// pageformat
			pageformat = TagParameterString::cast(rtpl->RemoveHead());
			assert(pageformat);

			if (!textformat)
				textformat = new TagParameterString("rb");

			if (fsize && fsize->TagIsNotSet())
			{
				fsize->setValue(14);
				fsize->setUnit("pt");
			}
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

std::ostream & ARComposer::operator<<(std::ostream & os) const
{
	return os << "\\composer<\"" << name << "\"> ";
}

const char* ARComposer::getName() const			{ return name ? name->getValue() : 0; }
const char* ARComposer::getPageFormat() const	{ return pageformat ? pageformat->getValue() : 0; }



