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

#include "ARText.h"

#include "ARLyrics.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;



ARText::ARText (const std::string& p_txt, float, bool isLyric)
	: fIsLyric(isLyric), fIsAutoPos(false)
{
	setupTagParameters (isLyric ? gMaps->sARLyricsMap : gMaps->sARTextMap);
	fText = p_txt;
	STagParameterPtr p (new TagParameterString (fText));
	p->setName(kTextStr);
	addTagParameter(p);
	STagParameterPtr fs (new TagParameterFloat ("9pt"));
	fs->setName(kFSizeStr);
	addTagParameter(fs);
	rangesetting = RANGEDC;
}

ARText::ARText() : fIsLyric(false), fIsAutoPos(false)
{
	setupTagParameters (gMaps->sARTextMap);

	relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
	setDuration( DURATION_0 );
	rangesetting         = RANGEDC;
}

void ARText::setTagParameters (const TagParameterMap& params)
{
	ARFontAble::setTagParameters(params);
	if (!isLyric()) {	// text is set by the constructor for lyrics
		const TagParameterString* p = getParameter<TagParameterString>(kTextStr);
		if (p) fText = p->getValue();
	}
}

void ARText::copyLyricsParams(const ARLyrics * lyrics)
{
	copyParameters (lyrics->getTagParameters());
	setTagParameters(lyrics->getTagParameters());
}
