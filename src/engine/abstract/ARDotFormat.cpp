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

#include "ARDotFormat.h"

//const ARDotFormat defaultARDotFormat(NULL,NULL);

ARDotFormat::ARDotFormat(const ARDotFormat *p_savedf, const ARDotFormat *copydf)
	: ARMTParameter(-1,copydf)
{
	// this probably should be changed to RANGEDC
	// but then we have to take care of saving state-information ....
	rangesetting = ARMusicalTag::RANGEDC;
	fSaveDF = p_savedf;
    if (copydf)
		copyParameters(copydf->getTagParameters());
}

