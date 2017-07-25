/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARPizzicato.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

using namespace std;

extern const char* kAboveStr;
extern const char* kBelowStr;


static const TagParameterMap sARPizzicatoMap (kARPizzicatoParams);

ARPizzicato::ARPizzicato()
{
	setupTagParameters (sARPizzicatoMap);

	rangesetting = ONLY;
	fType = LEFTHAND;
	fPosition = kAbove;
}

//--------------------------------------------------------------------------
void ARPizzicato::setTagParameters (const TagParameterMap& params)
{
	ARArticulation::setTagParameters (params);
	const TagParameterString* p = getParameter<TagParameterString>(kTypeStr);
	if (p) {
		string type = p->getValue();
		if (type == "buzz")
			fType = BUZZ;
		else if (type == "snap" || type == "bartok")
			fType = SNAP;
		else if (type == "fingernail")
			fType = FINGERNAIL;
	}
}
