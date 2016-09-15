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
// #include "ARFactory.h"
#include "ARBar.h"
#include "ARMusicalTag.h"

#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARBar::ltpls(1);

ARBar::ARBar(const TYPE_TIMEPOSITION &timeposition)
	: ARMTParameter(timeposition)
{
	measureNumber               = 0;
    measureNumberDisplayed      = false;
    measureNumberDisplayedIsSet = false;

	numDx = 0;
	numDy = 0;
}

ARBar::ARBar() : ARMTParameter()
{
	measureNumber               = 0;
    measureNumberDisplayed      = kNoNum;
    measureNumberDisplayedIsSet = false;

	numDx = 0;
	numDy = 0;
}

ARBar::~ARBar() // does nothing
{
}

void ARBar::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			("S,displayMeasNum,false,o;U,numDx,0,o;U,numDy,0,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb

            TagParameterString *s = TagParameterString::cast(rtpl->RemoveHead());
			bool display = false;
			s->getBool(display);
            if (s->TagIsSet()) {
				measureNumberDisplayed = display ? kNumAll : kNoNum;
                measureNumberDisplayedIsSet = true;
			}
            delete s;

			// - dx/dy for measure number
			TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
			numDx = f->getValue();
            delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
			numDy = f->getValue();
            delete f;
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARBar::printName(std::ostream& os) const
{
    os << "ARBar";
}

void ARBar::printGMNName(std::ostream& os) const
{
    os << "\\bar";
}

void ARBar::printParameters(std::ostream& os) const
{
    os << "measureNumber: " << measureNumber << "; ";
    os << "measureNumberDisplayed: " << measureNumberDisplayed << "; ";
    os << "numDx: " << numDx << "; ";
    os << "numDy: " << numDy << ";";

    ARMusicalTag::printParameters(os);
}