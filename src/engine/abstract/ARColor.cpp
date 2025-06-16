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
#include "TagParameterString.h"
#include "TagParameterStrings.h"


ARColor::ARColor()
{
	setupTagParameters (gMaps->sARColorMap);
}

void ARColor::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* color = getParameter<TagParameterString>(kColorStr);
	if (color) {
		unsigned char c[4];
		color->getRGB(c);
		fColor.Set(c[0], c[1], c[2], c[3]);
	}
}

