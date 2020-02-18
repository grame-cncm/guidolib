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

#include "ARFingering.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

using namespace std;


ARFingering::ARFingering(int pos) : fPosition(pos)
{
	setupTagParameters (gMaps->sARFingeringMap);
	rangesetting = ONLY;
}

void ARFingering::scanText(const string& text)
{
	size_t i = 0;
	size_t len = text.length();
	string part;
	while (i < len) {
		if (text[i] == ',') {
			fFingerings.push_back (part);
			part.clear();
		}
		else part += text[i];
		i++;
	}
	if (part.size()) fFingerings.push_back (part);
}

void ARFingering::setTagParameters (const TagParameterMap& params)
{
	ARText::setTagParameters (params);
	const TagParameterString* pos = getParameter<TagParameterString>(kPositionStr);
	if (pos) {
		string posStr = pos->getValue();
		if (posStr == kAboveStr)		fPosition = kAbove;
		else if (posStr == kBelowStr)	fPosition = kBelow;
		else cerr << "Guido Warning: '" << posStr << "': incorrect fingering position value: " << posStr << endl;
	}
	if (fSize && !getParameter<TagParameterFloat>(kFSizeStr)) fFontSize = fSize;
	scanText (getText());
}


