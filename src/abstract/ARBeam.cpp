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

#include <cstring>
#include <iostream>

#include "ARBeam.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARBeam::ltpls(1);

ARBeam::ARBeam()
{
	rangesetting = ONLY;
	dx1 = dx2 = dx3 = dx4 = NULL;
	dy1 = dy2 = dy3 = dy4 = NULL;
}

ARBeam::~ARBeam()
{
	delete dx1;
	delete dx2;
	delete dx3;
	delete dx4;
	delete dy1;
	delete dy2;
	delete dy3;
	delete dy4;
}

bool ARBeam::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\beamEnd",s))
		return true;
	return false;
}

void ARBeam::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"U,dy,7hs,o"));
		lstrs.AddTail(
			(
			"U,dy1,7hs,o;U,dy2,7hs,o"));
		lstrs.AddTail(
			(
			"U,dx1,0hs,o;U,dy1,0hs,o;U,dx2,0hs,o;U,dy2,1hs,o;"
			"U,dx3,0hs,o;U,dy3,0hs,o;U,dx4,0hs,o;U,dy4,1hs,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// this is the simple dy match 
			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
		}
		else if (ret == 1)
		{
			// this is simple dy1 and dy2 match
			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
			dy3 = TagParameterFloat::cast(rtpl->RemoveHead());
		}
		else if (ret == 2)
		{
			// then, we now the match for
			// the first ParameterList
			// GuidoPos pos = rtpl->GetHeadPosition();

			dx1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx1);
	
			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy1);
	
			dx2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx2);
	
			dy2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy2);
	
			dx3 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx3);
	
			dy3 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy3);
	
			dx4 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx4);
	
			dy4 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy4);

		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	// this sets the parameters ...
	tpl.RemoveAll();
}

bool ARBeam::isGuidoSpecBeam() const
{
	const bool oneset = (dy1 && dy1->TagIsSet());
	const bool twoset = (dy2 && dy2->TagIsSet());
//	int threeset =  (dy3 && dy3->TagIsSet());
	const bool fourset = (dy4 && dy4->TagIsSet());
	if (oneset && !twoset && !fourset)	// threeset ? 
		return true;
	return false;
}

void ARBeam::PrintName(std::ostream & os) const
{
	os << "\\beam";
	if (!getRange())
		os << "Begin";
}

