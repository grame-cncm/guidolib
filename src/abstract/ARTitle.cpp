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
#include "ARTitle.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"


ListOfTPLs ARTitle::ltpls(1);


ARTitle::ARTitle()
{
	name = NULL;
	pageformat = NULL;
	rangesetting = NO;
}


ARTitle::~ARTitle()
{
	delete name;
	delete pageformat;
}

std::ostream & ARTitle::operator<<(std::ostream &os) const
{
	return os << "\\title<\"" << name << "\"> ";
}


const char * ARTitle::getName() 
{ 
	if (name == NULL)
		return NULL;
	else
		return name->getValue();
}

const char * ARTitle::getPageFormat()
{
	if (pageformat == NULL)
		return NULL;
	else
		return pageformat->getValue();
}

void ARTitle::print() const
{
}

void ARTitle::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail((
			"S,name,,r;"
			"S,pageformat,c2,o;S,textformat,cc,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList name
			delete name;
			delete pageformat;
			delete textformat;

			name = TagParameterString::cast(rtpl->RemoveHead());
			assert(name);

			// pageformat
			pageformat = TagParameterString::cast(rtpl->RemoveHead());
			assert(pageformat);	

			textformat = TagParameterString::cast(rtpl->RemoveHead());
			assert(textformat);
		}

		if (fsize && fsize->TagIsNotSet())
		{
			fsize->setValue(24);
			fsize->setUnit("pt");
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}
