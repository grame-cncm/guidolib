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

#include "ARFermata.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "TimeUnwrap.h"

using namespace std;

ListOfTPLs ARFermata::ltpls(1);

extern const char* kBelowStr;

ARFermata::ARFermata()
{
	rangesetting = RANGEDC;
	fType = REGULAR;
	fPosition = ARArticulation::kAbove;
}

ARFermata::~ARFermata()
{
}

void ARFermata::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs;
		lstrs.AddTail("S,type,short,o;S,position,above,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			std::string shortstr ("short");
			std::string longstr ("long");
			std::string below ("below");

			if (str->TagIsSet()) {
				if (shortstr == str->getValue())
					fType = SHORT;
				else if (longstr == str->getValue())
					fType = LONG;
				else
                    fType = REGULAR;
			}

			delete str;
		
			str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet()) {
				string posStr = str->getValue();
				if (posStr == kBelowStr) {
					fPosition = ARArticulation::kBelow;
				}
				else cerr << posStr << ": incorrect staccato position" << endl;
			}

//			if (str->TagIsSet() && (below == str->getValue()))
//				fPosition = BELOW;
//			else 
//                fPosition = ABOVE;
			
			delete str;
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARFermata::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kFermata);
}

void ARFermata::printName(std::ostream& os) const
{
    os << "ARFermata";
}

void ARFermata::printGMNName(std::ostream& os) const
{
    os << "\\fermata";
}

void ARFermata::printParameters(std::ostream& os) const
{
    switch (fType) {
        case SHORT:
            os << "short";
            break;
        case LONG:
            os << "long";
            break;
        case REGULAR:
            os << "regular";
            break;
    }

    os << "; position: ";

    switch (fPosition) {
        case kBelow:
            os << "below";
            break;
        case kAbove:
            os << "above";
            break;
    }

    os << ";";

    ARMusicalTag::printParameters(os);
}

