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

#include "guidoEvalSusp.h"
#include "exceptions.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
// guidoEvalSusp
//______________________________________________________________________________
guidoEvalSusp::guidoEvalSusp(Sguidoexpression e, SguidoEnv env) 
	: fExp(e), fEnv(env) {}

Sguidovalue guidoEvalSusp::create(Sguidoexpression e, SguidoEnv env)
	{ suspCreate("guidoEvalSusp"); guidoEvalSusp * o = new guidoEvalSusp(e,env); assert(o!=0); return o; }

//______________________________________________________________________________
void guidoEvalSusp::print(ostream& os) 
{
	os << "guidoEvalSusp";
}

//______________________________________________________________________________
Sguidovalue	guidoEvalSusp::force()
{
	suspForce("guidoEvalSusp");
	return fExp->eval(fEnv);
}

//______________________________________________________________________________
void guidoEvalSusp::acceptIn(basevisitor& v) {
	if (visitor<SguidoEvalSusp>* p = dynamic_cast<visitor<SguidoEvalSusp>*>(&v)) {
		SguidoEvalSusp ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoEvalSusp::acceptOut(basevisitor& v) {
	if (visitor<SguidoEvalSusp>* p = dynamic_cast<visitor<SguidoEvalSusp>*>(&v)) {
		SguidoEvalSusp ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
