/*
  GUIDO Library
  Copyright (C) 2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARNotations.h"

using namespace std;

string ARNotations::getGMNName() const		{
	switch (fType) {
		case kPedalBegin:	return "\\pedalOn";
		case kPedalEnd:		return "\\pedalOff";
		default: return "unknown";
	}
};
