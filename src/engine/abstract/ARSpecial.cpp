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

#include "ARSpecial.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

static const TagParameterMap sARSpecialMap (kARSpecialParams);

ARSpecial::ARSpecial() : mDrawChar (0)
{
	setupTagParameters (sARSpecialMap);
}

//--------------------------------------------------------------------------
unsigned char ARSpecial::string2char (const char* str) const
{
	int num;

	if (str[0] == '\\') {	// it is a decimal or octal value
		if (str[1] == 'x') {
			// its a hex number
			if (sscanf(&(str[2]), "%x", &num) != 1)
				return 0;
		}
		else if (str[1] == 'o') {  // its an octal number
			if (sscanf(&(str[2]), "%o", &num) != 1)
				return 0;
		}
		else if (sscanf(&(str[1]), "%d", &num) != 1)
			return 0;

		if ((num > 0) && (num <= 255))
			return (unsigned char) num;	// (JB) was (char)num
		else
			return 0;

	}
	else
		return (unsigned char) str[0];
}

//--------------------------------------------------------------------------
void ARSpecial::setTagParameters (const TagParameterMap& params)
{
	ARMTParameter::setTagParameters (params);
	const TagParameterString * p = getParameter<TagParameterString>(kCharStr);
	if (p) mDrawChar = string2char(p->getValue());
}

