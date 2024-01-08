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

#include "ARRepeatBegin.h"
#include "ARRepeatEnd.h"
#include "TimeUnwrap.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

ARRepeatBegin::ARRepeatBegin()
{
	setupTagParameters (gMaps->sARRepeatMap);
	dur.setNumerator(-1);
}

void ARRepeatBegin::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kRepeatBegin);
}

void ARRepeatBegin::setRepeatEnd(const ARRepeatEnd *myr)
{
	if (dur.getNumerator() == -1)
		dur = myr->getRelativeTimePosition() - getRelativeTimePosition();
}

bool ARRepeatBegin::hidden () const
{
	return getParameter<TagParameterString>(kHiddenStr, true)->getBool();
}

