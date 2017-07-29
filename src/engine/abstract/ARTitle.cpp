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

#include "ARTitle.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

using namespace std;


ARTitle::ARTitle()
{
	setupTagParameters (gMaps->sARTitleMap);
	clearTagDefaultParameter(kTextStr);		// this is to avoid a warning regarding inherited required parameter for ARText
	rangesetting = NO;
}

const char * ARTitle::getName() const
{ 
	const TagParameterString* name = getParameter<TagParameterString>(kNameStr);
	return name ? name->getValue() : 0;
}

const char * ARTitle::getPageFormat() const
{
	const TagParameterString* pf = getParameter<TagParameterString>(kPageFormatStr, true);
	return pf ? pf->getValue() : 0;
}
