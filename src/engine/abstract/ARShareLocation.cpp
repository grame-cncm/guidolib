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
#include "ARShareLocation.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARShareLocation::ltpls(1);

ARShareLocation::ARShareLocation(const ARShareLocation * sloc)
: ARMTParameter(-1,sloc)
{
    rangesetting = ONLY;
}

ARShareLocation::~ARShareLocation()
{
}

ARMusicalObject * ARShareLocation::Copy() const
{
	return new ARShareLocation(this);
}

void ARShareLocation::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( ""));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// GuidoPos pos = rtpl->GetHeadPosition();
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

void ARShareLocation::printName(std::ostream& os) const
{
    os << "ARShareLocation";
    
    ARMusicalTag::printName(os);
}

void ARShareLocation::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}


