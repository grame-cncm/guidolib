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
#include "ARAccent.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"


ListOfTPLs ARAccent::ltpls(1);

void ARAccent::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail((""
			// "S,text,,r;U,dy,-1,o"
			));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,& rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// Get The TagParameters ...
			// text = 
			//	TagParameterString::cast(rtpl->RemoveHead());
			//assert(text);

		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARAccent::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kAccent);
}

void ARAccent::print() const
{
}

void ARAccent::PrintName(std::ostream & os) const
{
	os << "\\accent";
	if (getRange()) os << "(";

}

void ARAccent::PrintParameters(std::ostream & ) const
{
}


