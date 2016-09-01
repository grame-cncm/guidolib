/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "ARPizzicato.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARPizzicato::ltpls(1);

void ARPizzicato::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs;
		lstrs.AddTail("S,type,lefthand,o");
		CreateListOfTPLs(ltpls, lstrs);
	}
	
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	
	if (ret>=0 && rtpl)
	{
		//we found a match !
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				std::string val(str->getValue());
				if (val == "buzz")
					type = BUZZ;
				else if (val == "snap" || val == "bartok")
					type = SNAP;
				else if (val == "fingernail")
					type = FINGERNAIL;
				else
                    type = LEFTHAND;
			}
			else
                type = LEFTHAND;
			
			delete str;
		}
		
		delete rtpl;
	}

	tpl.RemoveAll();
}

void ARPizzicato::printName(std::ostream& os) const
{
    os << "ARPizzicato";
}

void ARPizzicato::printGMNName(std::ostream& os) const
{
    os << "\\pizzicato";
}

void ARPizzicato::printParameters(std::ostream& os) const
{
    switch (type) {
    case BUZZ:
        os << "buzz";
        break;
    case SNAP:
        os << "snap";
        break;
    case FINGERNAIL:
        os << "fingernail";
        break;
    default:
        os << "lefthand";
        break;
    }

    os << ";";

    ARMusicalTag::printParameters(os);
}
