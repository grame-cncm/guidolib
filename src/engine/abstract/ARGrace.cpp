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
#include "ARGrace.h"
// #include "ARFactory.h"
#include "TagParameterInt.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARGrace::ltpls(1);

ARGrace::ARGrace()
{
	num = NULL;
	rangesetting = ONLY;
}

ARGrace::~ARGrace()
{
	delete num;
}

void ARGrace::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "I,i,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			num = TagParameterInt::cast(rtpl->RemoveHead());
			assert(num);
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARGrace::printName(std::ostream& os) const
{
    os << "ARGrace";
    ARMusicalTag::printName(os);
}

void ARGrace::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}
