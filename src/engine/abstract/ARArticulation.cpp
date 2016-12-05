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
#include <string>

#include "ARArticulation.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"


const char* kAboveStr = "above";
const char* kBelowStr = "below";

ListOfTPLs ARArticulation::ltpls(1);

void ARArticulation::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,position,,o");
		CreateListOfTPLs(ltpls,lstrs);

	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,& rtpl);

	if (ret==0 && rtpl)
	{
		// we found a match!
		TagParameterString * ppos = TagParameterString::cast(rtpl->RemoveHead());
		assert(ppos);
		if (ppos->TagIsSet()) {
			string posStr = ppos->getValue();
			if (posStr == kAboveStr) {
				fPosition = kAbove;
			}
			else if (posStr == kBelowStr) {
				fPosition = kBelow;
			}
			else cerr << "Guido Warning: '" << posStr << "': incorrect articulation position" << endl;
		}
		delete ppos;
		delete rtpl;
	}
	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARArticulation::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kAccent);
}
