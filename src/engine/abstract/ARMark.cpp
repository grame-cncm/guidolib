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
#include "ARMark.h"
// #include "ARFactory.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARMark::ltpls(1);

ARMark::ARMark()
{
	rangesetting = NO;
}

ARMark::~ARMark()
{
}

void ARMark::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,text,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			text = TagParameterString::cast(rtpl->RemoveHead());
			assert(text);

		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARMark::printName(std::ostream& os) const
{
    os << "ARMark";
}

void ARMark::printGMNName(std::ostream& os) const
{
    os << "\\mark";
}

void ARMark::printParameters(std::ostream& os) const
{
    if (text)
        os << "text: \"" << text->getValue() << "\"; ";

    ARMusicalTag::printParameters(os);
}
