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


#include "ARAccidental.h"
//#include "ARFactory.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARAccidental::ltpls(1);

ARAccidental::ARAccidental()
{
	rangesetting = ONLY;
	mStyle = NULL;
}

ARAccidental::ARAccidental(const ARAccidental & acc)
{
	if (acc.mStyle) mStyle = new TagParameterString(acc.mStyle->getValue());
	else			mStyle = NULL;
}

ARAccidental::~ARAccidental()		{ delete mStyle; }

void ARAccidental::PrintName(std::ostream & os) const
{
	os << "\\acc";
}

ARAccidental::Style ARAccidental::getStyle() const	
{ 
	std::string str("cautionary");
	if (mStyle && (str == mStyle->getValue())) 
		return kCautionary;
	return kNone;
}

void ARAccidental::PrintParameters(std::ostream & os) const
{
	if (mStyle)
		os << "<style=\"" << mStyle->getValue() << "\">";
}

void ARAccidental::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "S,style,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			mStyle = TagParameterString::cast(rtpl->RemoveHead());
			assert(mStyle);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARAccidental::print(std::ostream& os) const
{
    os << "ARAccidental: ";

    if (mStyle)
        os << "style: " << mStyle->getValue() << "; ";

    os << std::endl;
}

