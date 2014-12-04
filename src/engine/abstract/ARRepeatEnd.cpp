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

#include "ARRepeatEnd.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "TimeUnwrap.h"

ListOfTPLs ARRepeatEnd::ltpls(1);

ARRepeatEnd::ARRepeatEnd(ARRepeatBegin * curbeg)
{ 
	repbeg       = curbeg;
	rangesetting = RANGEDC;
}

void ARRepeatEnd::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARRepeatEnd::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kRepeatEnd);
}

void ARRepeatEnd::printName(std::ostream& os) const
{
    os << "ARRepeatEnd";
}

void ARRepeatEnd::printGMNName(std::ostream& os) const
{
    os << "\\repeatEnd";
}

void ARRepeatEnd::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}