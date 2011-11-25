/*
	GUIDO Librar
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

#include "ARAuto.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARAuto::ltpls(1);

ARAuto::ARAuto()
{
	numparset = 0;
	endBarState = ON;
	pageBreakState = ON;
	systemBreakState = ON;
	clefKeyMeterOrderState = ON;
	stretchLastLineState = OFF;
	stretchFirstLineState = OFF;
}

ARAuto::~ARAuto()
{ }
	
void ARAuto::setTagParameterList(TagParameterList& tpl)
{

	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,endBar,on,o;"
			"S,pageBreak,on,o;"
			"S,systemBreak,on,o;"
			"S,clefKeyMeterOrder,on,o;"
			"S,stretchLastLine,off,o;"
			"S,stretchFirstLine,off,o"));
		lstrs.AddTail(("S,autoEndBar,on,o;"
			"S,autoPageBreak,on,o;"
			"S,autoSystemBreak,on,o;"
			"S,autoClefKeyMeterOrder,on,o;"
			"S,autoStretchLastLine,off,o;"
			"S,autoStretchFirstLine,off,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}


	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0 || ret == 1)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			std::string off("off");
			std::string on("on");
			if (str->TagIsSet())
			{
				++numparset;
				if (off == str->getValue())
					endBarState = OFF;
			}
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				++numparset;
				if (off == str->getValue())
					pageBreakState = OFF;
			}
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				++numparset;
				if (off == str->getValue())
					systemBreakState = OFF;
			}
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				++numparset;
				if (off == str->getValue())
					clefKeyMeterOrderState = OFF;
			}
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				++numparset;
				if (on == str->getValue())
					stretchLastLineState = ON;
			}
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				++numparset;
				if (on == str->getValue())
					stretchFirstLineState = ON;
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

void ARAuto::PrintName(std::ostream & os) const
{
	os << "\\set";
}

void ARAuto::PrintParameters(std::ostream & os) const
{
	if (numparset)
	{
		int prev = 0;
		os << "<";
		
		if (endBarState == OFF)
		{
			os << "endBar=\"off\"";
			prev = 1;
		}
		if (pageBreakState == OFF)
		{

			if (prev) os << ",";
			os << "pageBreak=\"off\"";
			prev = 1;
		}
		
		if (systemBreakState == OFF)
		{ 
			if (prev) os << ",";
			os << "systemBreak=\"off\"";
			prev = 1;
		}

		if (clefKeyMeterOrderState == OFF)
		{ 
			if (prev) os << ",";
			os << "clefKeyMeterOrder=\"off\"";
			prev = 1;
		}
		if (stretchLastLineState == ON)
		{
			if (prev) os << ",";
			os << "stretchLastLine=\"on\"";
			prev = 1;
		}
		if (stretchFirstLineState == ON)
		{
			if (prev) os << ",";
			os << "stretchFirstLine=\"on\"";
			prev = 1;
		}
		os << ">";
	}
}
