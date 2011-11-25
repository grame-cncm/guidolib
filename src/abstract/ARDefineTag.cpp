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
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "ARDefineTag.h"

ListOfTPLs ARDefineTag::ltpls(1);

ARDefineTag::ARDefineTag()
{
	tagparameters = NULL;
	tagname = NULL;
	ggstext = NULL;
}

ARDefineTag::~ARDefineTag()
{
	delete tagparameters;
	delete tagname;
	delete ggstext;
}

void ARDefineTag::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "S,name,,r;S,parameters,,r;S,ggstext,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{			
			tagname = TagParameterString::cast(rtpl->RemoveHead());
			assert(tagname);
			
			tagparameters = TagParameterString::cast(rtpl->RemoveHead());
			assert(tagparameters);
			
			ggstext = TagParameterString::cast(rtpl->RemoveHead());
			assert(ggstext);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARDefineTag::PrintName(std::ostream & os) const
{
	os << "\\defineTag";
}

void ARDefineTag::PrintParameters(std::ostream & os) const
{
	if (!(tagname && tagparameters && ggstext))
		return;
	os << "<\"" << tagname->getValue()
		<< "\",\""
		<< tagparameters->getValue()
		<< "\",\""
		<< ggstext->getValue()
		<< "\"> ";
}

