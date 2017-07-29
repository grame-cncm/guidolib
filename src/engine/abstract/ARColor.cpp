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

#include "ARColor.h"
#include "TagParameterFloat.h"
#include "TagParameterStrings.h"


ARColor::ARColor() : fR(0), fG(0), fB(0), fA(0)
{
	setupTagParameters (gMaps->sARColorMap);
}

void ARColor::setTagParameters (const TagParameterMap& params)
{
	const TagParameterFloat * r = getParameter<TagParameterFloat>(kRedStr, true);
	const TagParameterFloat * g = getParameter<TagParameterFloat>(kGreenStr, true);
	const TagParameterFloat * b = getParameter<TagParameterFloat>(kBlueStr, true);
	const TagParameterFloat * a = getParameter<TagParameterFloat>(kAlphaStr, true);
	fR = r->getValue();
	fG = g->getValue();
	fB = b->getValue();
	fA = a->getValue();
}

