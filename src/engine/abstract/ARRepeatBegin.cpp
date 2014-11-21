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

#include "ARRepeatBegin.h"
#include "ARRepeatEnd.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "TimeUnwrap.h"

ListOfTPLs ARRepeatBegin::ltpls(1);

void ARRepeatBegin::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl)
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

void ARRepeatBegin::browse(TimeUnwrap& mapper) const
{
//	std::cout << getRelativeTimePosition() << " ARRepeatBegin::browse" << std::endl;
	mapper.AtPos (this, TimeUnwrap::kRepeatBegin);
}

void ARRepeatBegin::setRepeatEnd(const ARRepeatEnd *myr)
{
	if (dur.getNumerator() == -1)
		dur = myr->getRelativeTimePosition() - getRelativeTimePosition();
}

std::ostream & ARRepeatBegin::operator<<(std::ostream & os) const
{
	os << "\\repeatBegin";

	return (os << " ");
}

void ARRepeatBegin::print(std::ostream& os) const
{    
    os << "ARRepeatBegin;" << std::endl;
}