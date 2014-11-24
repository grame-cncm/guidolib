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

#include "ARInstrument.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARInstrument::ltpls(1);

ARInstrument::ARInstrument()
{
	name = 0;
	transp = 0;
}

ARInstrument::~ARInstrument()
{
	delete name;
	delete transp;
}
void ARInstrument::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail (( "S,name,,r;S,transp,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			name   = TagParameterString::cast(rtpl->RemoveHead());
			transp = TagParameterString::cast(rtpl->RemoveHead());
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

const char* ARInstrument::getName() const
{
	if (!name)
        return NULL;

	return name->getValue();
}

void ARInstrument::printName(std::ostream& os) const
{
    os << "ARInstrument";
    ARMusicalTag::printName(os);
}

void ARInstrument::printParameters(std::ostream& os) const
{
    if (name)
        os << "name: \"" << name->getValue() << "\"; ";

    if (transp)
        os << "transp: " << transp->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}
