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
#include "ARTHead.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARTHead::ltpls(1);

ARTHead::ARTHead(int st , ARTHead * p_savehead, ARTHead * p_copyhead )
	: ARMTParameter(-1,p_copyhead)
{
	rangesetting = RANGEDC;
	headstate = (ARTHead::HEADSTATE) st;
	savehead = p_savehead;

	if (p_copyhead)
	{
		headstate = p_copyhead->getHeadState();
	}
}

ARTHead::ARTHead(const ARTHead & arthead)
{
	headstate = arthead.headstate;
	
	savehead = NULL;
}

ARMusicalObject * ARTHead::Copy() const
{
	return new ARTHead(*this);
}

void ARTHead::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( ""));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);

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
		// failure...
	}

	tpl.RemoveAll();
}

std::ostream & ARTHead::operator<<(std::ostream & os) const
{
	switch (headstate)
	{
		case NORMAL:
		case NOTSET:	os << "\\headsNormal";		break;
		case REVERSE: 	os << "\\headsReverse";		break;
		case CENTER: 	os << "\\headsCenter";		break;
		case RIGHT: 	os << "\\headsRight";		break;
		case LEFT: 		os << "\\headsLeft";		break;
	}
	if (getRange())
		os << "(";

	return os << " ";
}

