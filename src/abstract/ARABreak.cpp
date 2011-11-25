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

#include "ARABreak.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARABreak::ltpls(1);

ARABreak::ARABreak()
{
	mPageBreakState = ON;
	mSystemBreakState = ON;
}

ARABreak::~ARABreak()
{
}

void ARABreak::PrintName(std::ostream & os) const
{
	os << "\\autoBreak";
}

void ARABreak::PrintParameters(std::ostream & os) const
{
	os << "<";

	if (mPageBreakState == ON)
		os << "pageBreak=\"on\",";
	else
		os << "pageBreak=\"off\",";

	if (mSystemBreakState == ON)
		os << "systemBreak=\"on\">";
	else
		os << "systemBreak=\"off\">";
}
	
void ARABreak::setTagParameterList(TagParameterList & tpl)
{

	if (ltpls.empty())
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,page,on,o;S,system,on,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			std::string off ("off");
			if (str->TagIsSet())
			{
				if (off == str->getValue())
					mPageBreakState = OFF;
			}
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				if (off == str->getValue())
					mSystemBreakState = OFF;
			}
			delete str;

		}

		delete rtpl;
	}
	// all Parameters are ignored ...
	// issue warning...
	tpl.RemoveAll();
	return;


}
