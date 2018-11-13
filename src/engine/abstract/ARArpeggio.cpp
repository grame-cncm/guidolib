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

#include <string>
#include <iostream>

#include "ARArpeggio.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"


ARArpeggio::ARArpeggio()
{
	setupTagParameters (gMaps->sARArpeggioMap);
	rangesetting = ONLY;
	fDirection = kUnknown;
}

bool ARArpeggio::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\arpeggioEnd",s))
		return true;
	return false;
}

void ARArpeggio::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* p = getParameter<TagParameterString>(kDirectionStr);
	if (p) {
		const std::string val (p->getValue());
		if (val == "up") fDirection = kUp;
		else if (val == "down") fDirection = kDown;
	}
}
