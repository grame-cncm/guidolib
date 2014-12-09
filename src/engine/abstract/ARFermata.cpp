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


ListOfTPLs ARFermata::ltpls(1);

ARFermata::ARFermata()
{
	rangesetting = RANGEDC;
	type = REGULAR;
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
					type = SHORT;
				else if (longstr == str->getValue())
					type = LONG;
				else
                    type = REGULAR;
			}

			delete str;
		
			str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);

			if (str->TagIsSet() && (below == str->getValue()))
				position = BELOW;
			else 
                position = ABOVE;
			
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
    switch (type) {
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

    switch (position) {
        case BELOW:
            os << "below";
            break;
        case ABOVE:
            os << "above";
            break;
    }

    os << ";";

    ARMusicalTag::printParameters(os);
}

