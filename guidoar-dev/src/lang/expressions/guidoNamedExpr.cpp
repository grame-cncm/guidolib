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

#include "guidoNamedExpr.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
// guidoNamedExpr
//______________________________________________________________________________
void guidoNamedExpr::acceptIn(basevisitor& v) {
	if (visitor<SguidoNamedExpr>* p = dynamic_cast<visitor<SguidoNamedExpr>*>(&v)) {
		SguidoNamedExpr ge = this;
		p->visitStart (ge);
	}
	else guidoexpression::acceptIn(v);
}

//______________________________________________________________________________
void guidoNamedExpr::acceptOut(basevisitor& v) {
	if (visitor<SguidoNamedExpr>* p = dynamic_cast<visitor<SguidoNamedExpr>*>(&v)) {
		SguidoNamedExpr ge = this;
		p->visitEnd (ge);
	}
	else guidoexpression::acceptOut(v);
}

//______________________________________________________________________________
bool guidoNamedExpr::operator ==(const Sguidoexpression& elt) const
{
	SguidoNamedExpr e = dynamic_cast<guidoNamedExpr*>((guidoexpression*)elt);
	Sguidoexpression named = getArg(0);
	Sguidoexpression compareTo = e ? elt->getArg(0) : elt;
	return *named == compareTo;	
}

} // namespace
