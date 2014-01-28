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
#include "ARMarcato.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

ListOfTPLs ARMarcato::ltpls(1);

ARMarcato::ARMarcato()
{
	rangesetting = RANGEDC;
    position = NOTSET;
}

ARMarcato::~ARMarcato()
{
}

void ARMarcato::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,position,,o");
        CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
            TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
            assert(str);

            std::string below ("below");
            std::string above ("above");

            if (str->TagIsSet() && (below == str->getValue()))
			{
				position = BELOW;
			}
			else if (str->TagIsSet() && (above == str->getValue()))
            {
                position = ABOVE;
            }

            delete str;

			// Get The TagParameters ...
			// text = 
			//	TagParameterString::cast(rtpl->RemoveHead());
			//assert(text);

		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARMarcato::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kMarcato);
}

void ARMarcato::print() const
{
}

void ARMarcato::PrintName(std::ostream &os) const
{
	os << "\\marcato";
	if (getRange()) os << "(";

}
void ARMarcato::PrintParameters(std::ostream &) const
{
}


