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
#include "ARDoubleBar.h"

ARDoubleBar::ARDoubleBar()
{
}

ARDoubleBar::~ARDoubleBar()
{
}

void ARDoubleBar::print(std::ostream& os) const
{
    os << "ARDoubleBar;" << std::endl;
}

/* ostream & ARDoubleBar::operator<<(ostream &os) const
{
	return os << "\\doublebar ";
} */

void ARDoubleBar::PrintName(std::ostream &os) const
{
	os << "\\doublebar ";
} 
