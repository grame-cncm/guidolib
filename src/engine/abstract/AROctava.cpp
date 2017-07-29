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

#include "AROctava.h"
#include "TagParameterStrings.h"
#include "TagParameterInt.h"


AROctava::AROctava(const AROctava * p_saveoct, const AROctava * copyoct)
				: ARMTParameter(-1, copyoct), fSaveoct(p_saveoct)
{
	setupTagParameters (gMaps->sAROctavaMap);

	rangesetting = RANGEDC;
	if (copyoct)
		setupTagParameters (copyoct->getTagParameters());
}

int AROctava::getOctava() const
{
	const TagParameterInt * oct = getTPOctava();
	return oct ? oct->getValue() : 0;
}

const TagParameterInt * AROctava::getTPOctava() const
{
	return getParameter<TagParameterInt>(kIStr);
}

