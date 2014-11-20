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

void ARABreak::print(std::ostream& os) const
{
    os << "ARABreak;" << std::endl;
}