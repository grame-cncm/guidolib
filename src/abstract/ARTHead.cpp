/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

