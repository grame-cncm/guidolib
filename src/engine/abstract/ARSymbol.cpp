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

#include "ARSymbol.h"

#include "TagParameterString.h"
#include "TagParameterStrings.h"
#include "TagParameterInt.h"


ARSymbol::ARSymbol() : ARMTParameter() //, size(1)
{
	setupTagParameters (gMaps->sARSymbolMap);

    rangesetting = RANGEDC;
	relativeTimePosition = TYPE_TIMEPOSITION(-1, 1);
	setDuration( DURATION_0 );
    currentARMusic = 0;
}

int ARSymbol::getFixedWidth() const
{
	const TagParameterInt* p = getParameter<TagParameterInt>(kWStr);
	return p ? p->getValue() : 0;
}

int ARSymbol::getFixedHeight() const
{
	const TagParameterInt* p = getParameter<TagParameterInt>(kHStr);
	return p ? p->getValue() : 0;
}

const char* ARSymbol::getPositionString() const
{
	const TagParameterString* p = getParameter<TagParameterString>(kPositionStr);
	return p ? p->getValue() : 0;
}
 
const char* ARSymbol::getSymbolPath() const
{
	const TagParameterString* p = getParameter<TagParameterString>(kFileStr);
	return p ? p->getValue() : 0;	
}


