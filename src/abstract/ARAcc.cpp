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


#include "ARAcc.h"
//#include "ARFactory.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARAcc::ltpls(1);

ARAcc::ARAcc()
{
	rangesetting = ONLY;
	mStyle = NULL;
}

ARAcc::ARAcc(const ARAcc & acc)
{
	if (acc.mStyle) mStyle = new TagParameterString(acc.mStyle->getValue());
	else			mStyle = NULL;
}

ARAcc::~ARAcc()		{ delete mStyle; }

void ARAcc::PrintName(std::ostream & os) const
{
	os << "\\acc";
}

const char* ARAcc::getStyleStr() const	
{ 
	return mStyle ? mStyle->getValue() : "none"; 
}

ARAcc::Style ARAcc::getStyle() const	
{ 
	std::string str("cautionary");
	if (mStyle && (str == mStyle->getValue())) 
		return kCautionary;
	return kNone;
}

void ARAcc::PrintParameters(std::ostream & os) const
{
	if (mStyle)
		os << "<style=\"" << mStyle->getValue() << "\">";
}

void ARAcc::setTagParameterList(TagParameterList & tpl)
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

