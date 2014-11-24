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
#include <string.h>

#include "ARDisplayDuration.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARDisplayDuration::ltpls(1);

ARDisplayDuration::ARDisplayDuration(const ARDisplayDuration * dspdur)
						: ARMTParameter(-1,dspdur)
{
	rangesetting = ONLY;

	num = denom = ndots = 0;
	
	if (dspdur->num)
		num = TagParameterInt::cast(dspdur->num->getCopy());

	if (dspdur->denom)
		denom = TagParameterInt::cast(dspdur->denom->getCopy());

	if (dspdur->ndots)
		ndots = TagParameterInt::cast(dspdur->ndots->getCopy());
}

ARDisplayDuration::ARDisplayDuration() 
{
    rangesetting = ONLY;

    dots = 0;
    num = denom = ndots = NULL;
}

ARDisplayDuration::~ARDisplayDuration()
{
	delete num;
	delete denom;
	delete ndots;
}

void ARDisplayDuration::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("I,n,,r;I,d,,r;I,ndots,0,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// GuidoPos pos = rtpl->GetHeadPosition();

			num = TagParameterInt::cast(rtpl->RemoveHead());
			assert(num);

			denom = TagParameterInt::cast(rtpl->RemoveHead());
			assert(denom);

			dur.setNumerator(num->getValue());
			dur.setDenominator(denom->getValue());

			ndots = TagParameterInt::cast(rtpl->RemoveHead());
			assert(ndots);

			dots = ndots->getValue();
		}

		delete rtpl;
	}
		
	
	tpl.RemoveAll();
}

ARMusicalObject *ARDisplayDuration::Copy() const
{
	return new ARDisplayDuration(this);
}

bool ARDisplayDuration::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() && !strcmp("\\dispDurEnd",s))
		return 1;
	return 0;
}

void ARDisplayDuration::setDisplayDuration(const TYPE_DURATION & tmp)
{
	dots = 0;
	TYPE_DURATION mydur (tmp);
	mydur.normalize();

	if (mydur.getNumerator() == 1)
		dots = 0;
	else if (mydur.getNumerator() == 3 && mydur.getDenominator() > 1) {
		dots = 1;
		mydur.setNumerator(1);
		int denom = mydur.getDenominator();
		mydur.setDenominator(denom/2);
	}	
	else if (mydur.getNumerator() == 7 && mydur.getDenominator() > 1) {
		dots = 2;
		mydur.setNumerator(1);
		int denom = mydur.getDenominator();
		mydur.setDenominator(denom/4);
	}
	else {
		// ASSERT(FALSE);
		//TRACE("dispduration of unknown length");
	}

	dur = mydur;		
}

void ARDisplayDuration::printName(std::ostream& os) const
{
    os << "ARDisplayDuration";
    ARMusicalTag::printName(os);
}

void ARDisplayDuration::printParameters(std::ostream& os) const
{
    if (num)
        os << "n: " << num->getValue() << "; ";
    else
        os << "n: " << dur.getNumerator() << "; ";

    if (denom)
        os << "d: " << denom->getValue() << "; ";
    else
     os << "d: " << dur.getDenominator() << "; ";

    if (ndots)
        os << "ndots: " << ndots->getValue() << ";";
    else
        os << "ndots: " << dots << ";";

    ARMusicalTag::printParameters(os);
}
