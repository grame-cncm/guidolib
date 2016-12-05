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
#include <string.h>
#include "ARStaccato.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

extern const char* kAboveStr;
extern const char* kBelowStr;


ListOfTPLs ARStaccato::ltpls(1);

void ARStaccato::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,type,,o;S,position,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret==0 && rtpl)
	{
		// we found a match!
		TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
		assert(str);
		if (str->TagIsSet())
		{
			if (str->getValue() == std::string("heavy"))
				type = HEAVY;
			else
				type = REGULAR;
		}
		delete str;
		
		TagParameterString * ppos = TagParameterString::cast(rtpl->RemoveHead());
		assert(ppos);
		if (ppos->TagIsSet()) {
			string posStr = ppos->getValue();
			if (posStr == kAboveStr) {
				fPosition = ARStaccato::kAbove;
			}
			else if (posStr == kBelowStr) {
				fPosition = ARStaccato::kBelow;
			}
			else cerr << "Guido Warning: " << posStr << ": incorrect staccato position" << endl;
		}
		delete ppos;
		delete rtpl;
	}
	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARStaccato::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kStaccato);
}

bool ARStaccato::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() && !strcmp("\\staccEnd",s))
		return 1;
	return 0;
}

void ARStaccato::printName(std::ostream& os) const
{
    os << "ARStaccato";
}

void ARStaccato::printGMNName(std::ostream& os) const
{
    os << "\\staccato";
}

void ARStaccato::printParameters(std::ostream& os) const
{
    switch (type) {
    case HEAVY:
        os << "heavy";
        break;
    case REGULAR:
        os << "regular";
        break;
    }

    os << ";";

    ARMusicalTag::printParameters(os);
}
