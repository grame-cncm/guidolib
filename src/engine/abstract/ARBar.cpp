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
	barnumber = -1; // not specified
	measureNumber = 0;
    measureNumberDisplayed = NULL;

	numDx = 0;
	numDy = 0;
}


ARBar::ARBar() : ARMTParameter()
{
	barnumber = -1; // not specified
	measureNumber = 0;
    measureNumberDisplayed = NULL;

	numDx = 0;
	numDy = 0;
}

void ARBar::setMeasureNumberDisplayed(bool display) {
    delete measureNumberDisplayed;
    measureNumberDisplayed = (display ? new TagParameterString("true") : new TagParameterString("false"));
    measureNumberDisplayed->pflag = TagParameter::SETBYNAME;
}

ARBar::~ARBar() // does nothing
{
}

void ARBar::print(std::ostream& os) const
{    
    os << "ARBar: barnumber: " << barnumber << "; measureNumber: " << measureNumber << "; ";

    if (measureNumberDisplayed)
        os << "measureNumberDisplayed: " << measureNumberDisplayed->getValue() << "; ";

    os << "numDx: " << numDx << "; numDy: " << numDy << "; ";

    os << std::endl;
}

void ARBar::PrintName(std::ostream &os) const
{
	os << "\\bar";
}

void ARBar::PrintParameters(std::ostream &os) const
{
	if (barnumber!=-1)
	{
		os << "<" << barnumber << ">";
	}
}

void ARBar::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			("I,number,-1,o;S,displayMeasNum,false,o;U,numDx,0,o;U,numDy,0,o"));
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
            TagParameterInt * tpi =  TagParameterInt::cast(rtpl->RemoveHead());
			assert(tpi);
			if (tpi->pflag != TagParameter::NOTSET)
				barnumber = tpi->getValue();
            delete tpi;

            measureNumberDisplayed = TagParameterString::cast(rtpl->RemoveHead());

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