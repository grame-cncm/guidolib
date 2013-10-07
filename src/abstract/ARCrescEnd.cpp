/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "TagParameterList.h"
#include "ARCrescEnd.h"


// Parameters for crescBegin (as spec tells us ...)
// ... 
void ARCrescEnd::setTagParameterList(TagParameterList & tpl)
{
	// the end-tags do not have Parameters!
	tpl.RemoveAll();
}

void ARCrescEnd::PrintName(std::ostream & os) const
{ 
	os << " //crescEnd"; 
}

