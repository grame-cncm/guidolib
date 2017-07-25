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

#include <string.h>

#include "ARDynamic.h"

#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;

static const TagParameterMap sARDynamicMap (kARDynamicParams);

//--------------------------------------------------------------------------------------
ARDynamic::ARDynamic() : fDx1(0), fDx2(0), /*fDy(0),*/ fDeltaY(75), fThickness(4), fAutoPos(false)
{
	setupTagParameters (sARDynamicMap);	
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

//--------------------------------------------------------------------------------------
ARDynamic::ARDynamic(const ARDynamic * dyn) : ARMTParameter(-1, dyn),
	fDx1(0), fDx2(0), fDeltaY(75), fThickness(4), fAutoPos(false)
{
	setupTagParameters (sARDynamicMap);

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

    if (dyn) {
        fDx1            = dyn->getDx1();
        fDx2            = dyn->getDx2();
        fDeltaY         = dyn->getDeltaY();
        fThickness      = dyn->getThickness();
		fAutoPos	   = dyn->fAutoPos;
    }
}

//--------------------------------------------------------------------------------------
float ARDynamic::getDy(float curlspace) const{ return getDY()->getValue (curlspace); }

//--------------------------------------------------------------------------------------
void ARDynamic::setTagParameters(const TagParameterMap & params)
{
	fDx1 = getParameter<TagParameterFloat>(kDx1Str, true)->getValue();
	fDx2 = getParameter<TagParameterFloat>(kDx2Str, true)->getValue();
	fDeltaY  = getParameter<TagParameterFloat>(kDeltaYStr, true)->getValue();
	fThickness = getParameter<TagParameterFloat>(kThicknesStr, true)->getValue();
	fAutoPos = getParameter<TagParameterString>(kAutoposStr, true)->getBool();
}

