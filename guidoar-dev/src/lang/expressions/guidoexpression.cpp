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

#include "guidoexpression.h"
#include "guidoEnv.h"
#include "guidoEval.h"
#include "guidoExpPrinter.h"
#include "replaceVisitor.h"
#include "tree_browser.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
// guidoexpression
//______________________________________________________________________________
void guidoexpression::acceptIn(basevisitor& v) {
	if (visitor<Sguidoexpression>* p = dynamic_cast<visitor<Sguidoexpression>*>(&v)) {
		Sguidoexpression ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
Sguidoexpression guidoexpression::replace(const Sguidoexpression& exp, const Sguidoexpression& with)
{
	replaceVisitor rv;
	Sguidoexpression e = rv.replace(this, exp, with);
	return e;
}

//______________________________________________________________________________
Sguidovalue guidoexpression::eval(SguidoEnv env)
{
	guidoEval ev;
	return ev.eval (this, env);
}

/*
//______________________________________________________________________________
Sguidovalue guidoexpression::suspend(SguidoEnv env)
{
	return guidoEvalSusp::create (this, env);
}
*/

//______________________________________________________________________________
void guidoexpression::acceptOut(basevisitor& v) {
	if (visitor<Sguidoexpression>* p = dynamic_cast<visitor<Sguidoexpression>*>(&v)) {
		Sguidoexpression ge = this;
		p->visitEnd (ge);
	}
}

//______________________________________________________________________________
Sguidoexpression guidoexpression::getArg(unsigned int n) const { 
	ctree<guidoexpression>::const_literator i = lbegin();
	do {
		if (i == lend()) break;
		if (n == 0) return *i;
		i++;
		n--;
	} while (n >= 0);
	return 0;
}

//______________________________________________________________________________
void guidoexpression::basicprint(ostream& os) 
{
	Sguidoexpression arg1 = getArg(0);
	Sguidoexpression arg2 = getArg(1);
	if (arg1) {
		os << arg1;
		if (arg2)
			 os << getName() << arg2;
	}
}

//______________________________________________________________________________
void guidoexpression::print(ostream& os) 
{
	switch (getType()) {
		case kGrouped:
			os << "(" << getArg(0) <<  ")";
			break;
		case kAbstract:
			os << "#";
			basicprint(os);
			break;
		case kNamed:
			os << getName();
			break;
		case kIdent:
			os << getArg(0);
			break;
		default:
			basicprint (os);
	}
/*
	guidoExpPrinter gev(os);
	tree_browser<guidoexpression> browser(&gev);
	browser.browse(*this);
*/
}

//______________________________________________________________________________
ostream& operator << (ostream& os, const Sguidoexpression& elt) {
	elt->print(os);
	return os;
}

} // namespace
