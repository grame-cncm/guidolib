
/*
  GUIDO Library
  Copyright (C) 2020 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMMRest.h"
#include "TagParameterInt.h"

ARMMRest::ARMMRest()
{
	setupTagParameters (gMaps->sARMMRestMap);
	rangesetting = ONLY;
}

int ARMMRest::getMeasuresCount() const
{
	const TagParameterInt * count = getParameter<TagParameterInt>(kCountStr);
	return count ? count->getValue() : 0;
}
