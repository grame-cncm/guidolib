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

#include "ARRitardando.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

static const TagParameterMap sARRitardandoMap (kARRitardandoParams);

ARRitardando::ARRitardando()
{
	setupTagParameters (sARRitardandoMap);
	rangesetting = ONLY;
    tempo    = 0;
    abstempo = 0;
}

void ARRitardando::setTagParameters (const TagParameterMap& params)
{
	ARFontAble::setupTagParameters(params);
	tempo = getParameter<TagParameterString>(kTempoStr, true);
	abstempo = getParameter<TagParameterString>(kAbsTempoStr, true);
}

bool ARRitardando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;

	if (!getRange() && !strcmp("\\ritEnd",s))
		return true;
	return false;
}

