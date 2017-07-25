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

#include "ARComposer.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"


static const TagParameterMap sARComposerMap (kARComposerParams);

ARComposer::ARComposer()
{
	setupTagParameters (sARComposerMap);
	clearTagDefaultParameter(kTextStr);		// this is to avoid a warning regarding inherited required parameter for ARText
	name         = 0;
    pageformat   = 0;
    rangesetting = NO;
}

void ARComposer::setTagParameters (const TagParameterMap& params)
{
	ARText::setTagParameters(params);
	name = getParameter<TagParameterString>(kNameStr, true);
	pageformat = getParameter<TagParameterString>(kPageFormatStr, true);
}

const char* ARComposer::getName() const			{ return name ? name->getValue() : 0; }
const char* ARComposer::getPageFormat() const	{ return pageformat ? pageformat->getValue() : 0; }



