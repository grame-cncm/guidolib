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
#include "ARShareStem.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARShareStem::ltpls(1);

ARShareStem::ARShareStem(const ARShareStem * stm)
: ARMTParameter(-1,stm)
{
	rangesetting = ONLY;
}
ARShareStem::~ARShareStem()
{
}

void ARShareStem::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			""));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
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

ARMusicalObject * ARShareStem::Copy() const
{
	return new ARShareStem(this);
}

void ARShareStem::printName(std::ostream& os) const
{
    os << "ARShareStem";
}

void ARShareStem::printGMNName(std::ostream& os) const
{
    os << "\\shareStem";
}

void ARShareStem::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}
