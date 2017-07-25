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

#include "TagParameter.h"

void TagParameter::set( const TagParameter & in )
{
	fName = in.fName;
	fFlag = in.fFlag;
	fRequired = in.fRequired;
}

void TagParameter::print(std::ostream& out)
{
#if 0
	out << fName << " flag: " << fFlag << " required: " << fRequired;
#else
	if (fName.size()) out << fName << "=";
#endif
}

