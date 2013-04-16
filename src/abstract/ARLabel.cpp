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
#include "ARLabel.h"
// #include "ARFactory.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARLabel::ltpls(1);

ARLabel::ARLabel()
{
	rangesetting = RANGEDC;
}

ARLabel::~ARLabel()
{
}

void ARLabel::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "S,text,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
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

void ARLabel::PrintName(std::ostream &os) const
{
	os << "\\Label";
}

void ARLabel::PrintParameters(std::ostream &os) const
{
	if (!text) return;
		
	os << "<text=\"" << text->getValue() << "\">";
}

