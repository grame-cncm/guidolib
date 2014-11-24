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

#include "ARNewSystem.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "GuidoDefs.h"	// for kVirtualToCm
#include "ListOfStrings.h"


ListOfTPLs ARNewSystem::ltpls(1);

void ARNewSystem::setTagParameterList(TagParameterList & tpl)
{
		if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "U,dy,0,o"));
		CreateListOfTPLs(ltpls,lstrs);

	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
			mDy =  TagParameterFloat::cast(rtpl->RemoveHead());

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();

}

void ARNewSystem::printName(std::ostream& os) const
{
    os << "ARNewSystem";
    ARMusicalTag::printName(os);
}

void ARNewSystem::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}
