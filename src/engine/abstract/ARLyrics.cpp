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

#include "ARLyrics.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;

static const TagParameterMap sARLyricsMap (kARLyricsParams);

ARLyrics::ARLyrics(bool autopos) : ARFontAble()
{
	setupTagParameters (sARLyricsMap);

	relativeTimePosition.set(-1,1);
	setDuration ( DURATION_0 );
	rangesetting = ONLY;
	fAutoPos = autopos;
}

//--------------------------------------------------------------------------
void ARLyrics::setTagParameters (const TagParameterMap& params)
{
	ARFontAble::setTagParameters (params);
	fText = getParameter<TagParameterString>(kTextStr, true)->getValue();
	string autopos = getParameter<TagParameterString>(kAutoposStr, true)->getValue();
	if (autopos == "on") fAutoPos = true;
}
