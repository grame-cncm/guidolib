/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
