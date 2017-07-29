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

#include "ARAccidental.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;


ARAccidental::ARAccidental()
{
	setupTagParameters (gMaps->sARAccidentalMap);
	rangesetting = ONLY;
}

ARAccidental::ARAccidental(const ARAccidental & acc)
{
	setupTagParameters (gMaps->sARAccidentalMap);
	copyParameters (acc.getTagParameters());
	setTagParameters (acc.getTagParameters());
}

ARAccidental::Style ARAccidental::getStyle() const	
{ 
	const TagParameterString* param = getParameter<TagParameterString>(kStyleStr);
	if (param) {
		string style = param->getValue();
		if (style == "cautionary")
			return kCautionary;
		if (style == "none")
			return kNone;
	}
	return kUnknown;
}
