/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

void ARDefineTag::print(std::ostream& os) const
{
    os << "ARDefineTag;" << std::endl;
}

