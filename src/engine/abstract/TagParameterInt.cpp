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

#include <cstdlib>	// for atoi
#include "TagParameterInt.h"


/** \brief returns true is successful 
*/
bool TagParameterInt::copyValue(const TagParameter * tp )
{
	const TagParameterInt * tpi = TagParameterInt::cast(tp);
	if (tpi)
	{
		fValue = tpi->fValue;
		return true;
	}
	return false;
}


void TagParameterInt::print(std::ostream& out)
{
	TagParameter::print (out);
	const char* u = TagIsUnitTag() ? getUnit() : "";
	out << getValue() << u;
}
