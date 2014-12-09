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

#include <cstring>
#include <iostream>

#include "ARBeam.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARBeam::ltpls(1);

ARBeam::ARBeam()
{
	rangesetting = ONLY;
	dx1 = dx2 = dx3 = dx4 = NULL;
	dy1 = dy2 = dy3 = dy4 = NULL;
	feathered = false;
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

void ARBeam::printName(std::ostream& os) const
{
    os << "ARBeam";
}

void ARBeam::printGMNName(std::ostream& os) const
{
    os << "\\beam";
}

void ARBeam::printParameters(std::ostream& os) const
{
    /* TODO */

    ARMusicalTag::printParameters(os);
}
