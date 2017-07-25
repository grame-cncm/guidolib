/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>

#include "ARGlissando.h"

#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"
#include "TimeUnwrap.h"

using namespace std;

static const TagParameterMap sARGlissandoMap (kARGlissandoParams);

ARGlissando::ARGlissando()
{
	setupTagParameters (sARGlissandoMap);

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	fDx1       = 0;
    fDy1       = 0;
    fDx2       = 0;
    fDy2       = 0;
    fThickness = 0;
    fFill      = false;
	//fWavy      = false;
}

ARGlissando::ARGlissando(const ARGlissando * glissando)	: ARMTParameter(-1, glissando)
{
	setupTagParameters (sARGlissandoMap);
	copyParameters (glissando->getTagParameters());

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
    fDx1       = glissando->fDx1;
    fDy1       = glissando->fDy1;
    fDx2       = glissando->fDx2;
    fDy2       = glissando->fDy2;
    fThickness = glissando->fThickness;
    fFill      = glissando->fFill;
	//fWavy      = glissando->fWavy;
}


void ARGlissando::setTagParameters (const TagParameterMap& params)
{
	fDx1 = getParameter<TagParameterFloat>(kDx1Str, true)->getValue();
	fDy1 = getParameter<TagParameterFloat>(kDy1Str, true)->getValue();
	fDx2 = getParameter<TagParameterFloat>(kDx2Str, true)->getValue();
	fDy2 = getParameter<TagParameterFloat>(kDy2Str, true)->getValue();
	fFill = getParameter<TagParameterString>(kFillStr, true)->getBool();
	fThickness = getParameter<TagParameterFloat>(kThicknesStr, true)->getValue();
}


void ARGlissando::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kGlissando);
}

bool ARGlissando::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\glissandoEnd",s))
		return 1;

	return 0;
}

