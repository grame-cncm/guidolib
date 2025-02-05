/*

  MusicXML Library
  Copyright (C) 2006,2007  Grame

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

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include <iostream>

#include "guidoExpFactory.h"
#include "guidoexpression.h"
#include "guidoScoreExpr.h"
#include "guidoNamedExpr.h"
#include "cloneExpVisitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
Sguidoexpression cloneExpVisitor::clone(const Sguidoexpression& exp)
{ 
	if (exp) {
		stop(false);
		browse (*exp);
		if (fStack.size()) {
			Sguidoexpression copy = fStack.top();
			fStack.pop();
			return copy;
		}
	}
	return 0;
}

//______________________________________________________________________________
void cloneExpVisitor::push( const Sguidoexpression& exp, bool stack )
{
	if (fStack.empty())
		fStack.push(exp);
	else fStack.top()->push(exp);
	if (stack) fStack.push (exp);	
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void cloneExpVisitor::visitStart( Sguidoexpression& exp )
{
	if (exp->getType() == kNamed) {
		Sguidoexpression named = exp->getArg(0);
		push (guidoExpFactory::instance().createNamed(exp->getName(), named));
	}
	else push (guidoExpFactory::instance().create(exp->getName()));
}

//______________________________________________________________________________
void cloneExpVisitor::visitEnd  ( Sguidoexpression& exp )
{ 
	fStack.pop();
}
void cloneExpVisitor::visitEnd  ( SguidoScoreExpr& exp )		{}

//______________________________________________________________________________
void cloneExpVisitor::visitStart( SguidoScoreExpr& exp )
{
	Sguidoelement score = exp->getScore();
	push ( guidoExpFactory::instance().create( score ), false);
}

}
