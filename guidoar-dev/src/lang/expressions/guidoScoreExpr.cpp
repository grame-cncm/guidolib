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
#include <sstream>

#include "guidoNamedExpr.h"
#include "guidoScoreExpr.h"
#include "guidoScoreValue.h"
#include "normalizeOperation.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
// guidoScoreExpr
//______________________________________________________________________________
void guidoScoreExpr::acceptIn(basevisitor& v) {
	if (visitor<SguidoScoreExpr>* p = dynamic_cast<visitor<SguidoScoreExpr>*>(&v)) {
		SguidoScoreExpr ge = this;
		p->visitStart (ge);
	}
	else guidoexpression::acceptIn(v);
}

//______________________________________________________________________________
void guidoScoreExpr::acceptOut(basevisitor& v) {
	if (visitor<SguidoScoreExpr>* p = dynamic_cast<visitor<SguidoScoreExpr>*>(&v)) {
		SguidoScoreExpr ge = this;
		p->visitEnd (ge);
	}
	else guidoexpression::acceptOut(v);
}

//______________________________________________________________________________
void guidoScoreExpr::print(ostream& os) {
	Sguidoelement score = getScore();
	if (score) os << getScore();
	else cerr << "warning: guidoScoreExpr with null score" << endl;
}

//______________________________________________________________________________
bool guidoScoreExpr::operator ==(const Sguidoexpression& elt) const
{
	SguidoNamedExpr e = dynamic_cast<guidoNamedExpr*>((guidoexpression*)elt);
	if (e) return *this == e->getArg(0);

	SguidoScoreExpr score = dynamic_cast<guidoScoreExpr*>((guidoexpression*)elt);
	if (!score) return false;

	// matching is made on normalized score and by comparing the resulting gmn strings
	normalizeOperation norm;
	stringstream scoreStr;	scoreStr << norm( getScore() );
	stringstream eltStr;	eltStr 	<< norm( score->getScore() );
	return scoreStr.str() == eltStr.str();
}

} // namespace
