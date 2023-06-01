/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "ARDisplayDuration.h"
#include "TagParameterStrings.h"
#include "TagParameterInt.h"


ARDisplayDuration::ARDisplayDuration(const ARDisplayDuration * dspdur)
						: ARMTParameter(-1,dspdur)
{
	setupTagParameters (gMaps->sARDisplayDurationMap);
	rangesetting = ONLY;

	copyParameters (dspdur->getTagParameters());
	setTagParameters (dspdur->getTagParameters());
}

ARDisplayDuration::ARDisplayDuration() 
{
	setupTagParameters (gMaps->sARDisplayDurationMap);
    rangesetting = ONLY;
    fDots = 0;
}

void ARDisplayDuration::setTagParameters (const TagParameterMap& params)
{
	const TagParameterInt* n = getParameter<TagParameterInt>(kNStr);
	const TagParameterInt* d = getParameter<TagParameterInt>(kDStr);
	if (n && d) fDuration.set (n->getValue(), d->getValue());
	const TagParameterInt* dots = getParameter<TagParameterInt>(kNDotsStr, true);
	fDots = dots->getValue();
}

bool ARDisplayDuration::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\dispDurEnd",s))
		return 1;
	return 0;
}

void ARDisplayDuration::addTagParameters (const TYPE_DURATION & dur, int numdots)
{
	STagParameterPtr n (new TagParameterInt (dur.getNumerator()));
	n->setName("n");
	addTagParameter(n);
	STagParameterPtr d (new TagParameterInt (dur.getDenominator()));
	d->setName("d");
	addTagParameter(d);
	STagParameterPtr p (new TagParameterInt (numdots));
	p->setName("ndots");
	addTagParameter(p);

}

void ARDisplayDuration::setDisplayDuration(const TYPE_DURATION & tmp, int numdots) {
	fDuration = tmp;
	fDots = numdots;
	addTagParameters (tmp, numdots);
}


void ARDisplayDuration::setDisplayDuration(const TYPE_DURATION & tmp)
{
	fDots = 0;
	TYPE_DURATION mydur (tmp);
	mydur.normalize();

	if (mydur.getNumerator() == 1)
		fDots = 0;
	else if (mydur.getNumerator() == 3 && mydur.getDenominator() > 1) {
		fDots = 1;
		mydur.setNumerator(1);
		int denom = mydur.getDenominator();
		mydur.setDenominator(denom/2);
	}	
	else if (mydur.getNumerator() == 7 && mydur.getDenominator() > 3) {
		fDots = 2;
		mydur.setNumerator(1);
		int denom = mydur.getDenominator();
		mydur.setDenominator(denom/4);
	}
	else {
		// ASSERT(FALSE);
		//TRACE("dispduration of unknown length");
	}
	fDuration = mydur;
	addTagParameters (fDuration, fDots);
}
