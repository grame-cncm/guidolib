/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARJump.h"

#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"

using namespace std;

static const TagParameterMap sARJumpMap (kARJumpParams);

ARJump::ARJump(string mark) {
	setupTagParameters (sARJumpMap);

	FormatStringParser p;
	p.parse (mark.c_str(), mMark);
}

void ARJump::setTagParameters (const TagParameterMap& params)
{
	string mark = getParameter<TagParameterString>(kMStr, true)->getValue();
	if (mark.size()) {
		FormatStringParser p;
		mMark.clear();
		p.parse (mark.c_str(), mMark);
	}
	mID = getParameter<TagParameterInt>(kIDStr, true)->getValue();
}
