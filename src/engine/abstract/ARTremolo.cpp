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
#include "ARTremolo.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARTremolo::ltpls(1);

ARTremolo::~ARTremolo() 
{
	delete mDur;
};

void ARTremolo::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail((
			"I,dur,32,o"
			// "S,text,,r;U,dy,-1,o"
			));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARTremolo::print() const
{
}

void ARTremolo::PrintName(std::ostream & os) const
{
	os << "\\trem";

}

void ARTremolo::PrintParameters(std::ostream & os) const
{
	if (mDur && mDur->TagIsSet())
	{
		os << "<dur=" << mDur->getValue() << ">";
	}
	if (getRange()) os << "( ";
}


