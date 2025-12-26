/*
  GUIDO Library
  Copyright (C) 2006  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "guidosusp.h"
#include "exceptions.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
// guidosusp
//______________________________________________________________________________
Sguidovalue guidosusp::getValue() {
	if (!fValue) {
		Sguidovalue val = force();
		if (!val) throw (newException (kNullValue));
		fValue = val;
	}
	return fValue;
}

//______________________________________________________________________________
void guidosusp::acceptIn(basevisitor& v) {
	if (visitor<Sguidosusp>* p = dynamic_cast<visitor<Sguidosusp>*>(&v)) {
		Sguidosusp ge = this;
		p->visitStart (ge);
	}
	else guidovalue::acceptIn(v);
}

//______________________________________________________________________________
void guidosusp::acceptOut(basevisitor& v) {
	if (visitor<Sguidosusp>* p = dynamic_cast<visitor<Sguidosusp>*>(&v)) {
		Sguidosusp ge = this;
		p->visitEnd (ge);
	}
	else guidovalue::acceptOut(v);
}

} // namespace
