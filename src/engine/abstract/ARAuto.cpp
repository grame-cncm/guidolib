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

using namespace std;

ListOfTPLs ARAuto::ltpls(1);

ARAuto::ARAuto()
{
//	fNumparset              = 0;
	fEndBarState            = ON;
	fPageBreakState         = ON;
	fSystemBreakState       = ON;
	fClefKeyMeterOrderState = ON;
	fStretchLastLineState   = OFF;
	fStretchFirstLineState  = OFF;
	fLyricsAutoPos		    = OFF;
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
			"S,stretchFirstLine,off,o;"
			"S,lyricsAutoPos,off,o"));
		lstrs.AddTail(("S,autoEndBar,on,o;"
			"S,autoPageBreak,on,o;"
			"S,autoSystemBreak,on,o;"
			"S,autoClefKeyMeterOrder,on,o;"
			"S,autoStretchLastLine,off,o;"
			"S,autoStretchFirstLine,off,o;"
			"S,autoLyricsPos,off,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0 || ret == 1) {
			const string off("off");
			const string on("on");

			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fEndBarState =  (off == str->getValue()) ? OFF : ON;
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fPageBreakState = (off == str->getValue()) ? OFF : ON;
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fSystemBreakState = (off == str->getValue()) ? OFF : ON;
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fClefKeyMeterOrderState = (off == str->getValue()) ? OFF : ON;
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fStretchLastLineState = (on == str->getValue()) ? ON : OFF;
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fStretchFirstLineState = (on == str->getValue()) ? ON : OFF;
			delete str;

			str =  TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fLyricsAutoPos = (on == str->getValue()) ? ON : OFF;
			delete str;
		}
		delete rtpl;
	}
	// all Parameters are ignored ...
	// issue warning...
	tpl.RemoveAll();
	return;
}

void ARAuto::printName(std::ostream& os) const		{ os << "ARAuto"; }
void ARAuto::printGMNName(std::ostream& os) const	{ os << "\\auto"; }

void ARAuto::printParameters(std::ostream& os) const
{
    os << "autoEndBar: " << (fEndBarState == ON ? "on" : "off") << "; ";
    ARMusicalTag::printParameters(os);
}
