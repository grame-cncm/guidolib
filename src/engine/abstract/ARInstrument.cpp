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
	s1 = 0;
	s2 = 0;
	transp = 0;
}

ARInstrument::~ARInstrument()
{
	delete s1;
	delete s2;
	delete transp;
}
void ARInstrument::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail (( "S,name,,r;S,midiname,,o;S,transp,,o"));
		CreateListOfTPLs(ltpls,lstrs);

	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			s1 = TagParameterString::cast(rtpl->RemoveHead());

			s2 = TagParameterString::cast(rtpl->RemoveHead());

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
	if (!s1) return NULL;
	return s1->getValue();
}

const char* ARInstrument::getSecondName() const
{
	if (!s2) return NULL;
	return s2->getValue();
}

void ARInstrument::print(int &indent) const
{
}

std::ostream & ARInstrument::operator<<(std::ostream &os) const
{
	return os << "\\instr<\"" << getName() << "\">";
}

