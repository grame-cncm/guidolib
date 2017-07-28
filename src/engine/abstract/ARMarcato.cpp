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

#include "ARMarcato.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

#include "TimeUnwrap.h"

using namespace std;

static const TagParameterMap sARMarcatoMap (kARMarcatoParams);

ARMarcato::ARMarcato()
{
	setupTagParameters (sARMarcatoMap);
	rangesetting = ONLY; //RANGEDC;
    fPosition = NOTSET;
}

//--------------------------------------------------------------------------
void ARMarcato::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* p = getParameter<TagParameterString>(kPositionStr);
	if (p) {
		string pos = p->getValue();
		if (pos == "below") fPosition = BELOW;
		if (pos == "above") fPosition = ABOVE;
	}
}

// --------------------------------------------------------------------------
void ARMarcato::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kMarcato);
}


