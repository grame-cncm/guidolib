/*
	GUIDO Library
	Copyright (C) 2004  Grame

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

#include <string.h>

#include <iostream>
using namespace std;

#include "ARVolta.h"

#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"
#include "TimeUnwrap.h"

ListOfTPLs ARVolta::ltpls(1);

ARVolta::ARVolta()
{
	rangesetting = ONLY;
}

ARVolta::~ARVolta()
{

}

void ARVolta::browse(TimeUnwrap& mapper) const
{
//	std::cout << getRelativeTimePosition() << " ARVolta::browse: " << mPass << std::endl;
	mapper.AtPos (this, TimeUnwrap::kVolta);
}

bool ARVolta::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\voltaEnd",s))
		return true;
	return false;
}

void ARVolta::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty()) {
		ListOfStrings lstrs;
		lstrs.AddTail( "S,pass,,r;S,m,,r;S,format,,o" );
		CreateListOfTPLs(ltpls,lstrs);
	}
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if( ret >= 0 && rtpl ) {
		if( ret == 0 ) {
			TagParameterString * tps = TagParameterString::cast(rtpl->RemoveHead());
			if (tps) mPass = tps->getValue();
			tps = TagParameterString::cast(rtpl->RemoveHead());
			if (tps) mMark = tps->getValue();
			tps = TagParameterString::cast(rtpl->RemoveHead());
			if (tps) mFormat = tps->getValue();
		}	
		delete rtpl;
	}
	tpl.RemoveAll();
}

void ARVolta::PrintName( std::ostream & os ) const
{
	os << "\\volta";
}

std::ostream & ARVolta::operator<<(std::ostream & os) const
{
	PrintName(os);
	if (getRange())
		os << "(";
	return os << " ";
}


