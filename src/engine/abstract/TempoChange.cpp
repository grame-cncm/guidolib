/*
  GUIDO Library
  Copyright (C) 2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "TagParameterFloat.h"
#include "TempoChange.h"

using namespace std;


void TempoChange::setTagParameters (const TagParameterMap& params)
{
	ARFontAble::setTagParameters (params);

	const char* before = getParameter<TagParameterString>(kBeforeStr, true)->getValue();
	const char* after  = getParameter<TagParameterString>(kAfterStr, true)->getValue();

	fDx2 = getParameter<TagParameterFloat>(kDx2Str, true)->getValue();

	FormatStringParser p;
	fBefore.clear();
	p.parse (before, fBefore);
	fAfter.clear();
	p.parse (after, fAfter);
}
