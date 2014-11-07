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
