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
#include "ARCue.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARCue::fTagParametersList(1);

ARCue::ARCue() : name (0)		{ rangesetting = ONLY; }
ARCue::~ARCue()					{ delete name; }

void ARCue::setTagParameterList(TagParameterList& tpl)
{
	if (fTagParametersList.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,name,,o");
		CreateListOfTPLs(fTagParametersList,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(fTagParametersList, tpl, &rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0) {
			delete name;
			name = TagParameterString::cast(rtpl->RemoveHead());
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARCue::printName(std::ostream& os) const
{
    os << "ARCue";
}

void ARCue::printGMNName(std::ostream& os) const
{
    os << "\\cue";
}

void ARCue::printParameters(std::ostream& os) const
{
    if (name)
        os << "name: \"" << name->getValue() << "\"; ";

    ARMusicalTag::printParameters(os);
}
