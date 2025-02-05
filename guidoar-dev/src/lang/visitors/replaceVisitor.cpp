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
#include "guidoExpression.h"
#include "guidoScoreExpr.h"
#include "replaceVisitor.h"
#include "tree_browser.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
Sguidoexpression replaceVisitor::replace(const Sguidoexpression& exp, const Sguidoexpression& target, const Sguidoexpression& with)
{ 
	fCopy = true;
	fMatched = false;
	fTarget = target;
	fIdent = with;
	return clone(exp);
}

//______________________________________________________________________________
void replaceVisitor::replace(const Sguidoexpression& ident)
{ 
	push (ident, false);	// push the replacement
	fCopy = false;			// prevent copying subtree
	stop();
	fMatched = true;
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void replaceVisitor::visitStart( Sguidoexpression& exp )
{
	if (*exp == fTarget) {		// target expression found ?
		replace (fIdent);
	}
	else cloneExpVisitor::visitStart(exp);
}

//______________________________________________________________________________
void replaceVisitor::visitEnd  ( Sguidoexpression& exp )
{ 
	if (!fCopy) {
		fCopy = true;
		stop(false);
	}
	else cloneExpVisitor::visitEnd (exp);
}

//______________________________________________________________________________
void replaceVisitor::visitStart( SguidoIdentExpr& exp )
{
	push(exp, false);		// don't duplicate identificators
	stop();
}

//______________________________________________________________________________
void replaceVisitor::visitEnd( SguidoIdentExpr& exp )
{
	stop(false);
}

//______________________________________________________________________________
void replaceVisitor::visitStart( SguidoScoreExpr& exp )
{
	SguidoScoreExpr scoreExp = dynamic_cast<guidoScoreExpr*> ((guidoexpression*)fTarget);
	if (scoreExp && fStretchScore) {
		Sguidoexpression e= exp;
		Sguidoexpression transp = guidoExpFactory::instance().create ("transp", fTarget, e);
		Sguidoexpression stretch = guidoExpFactory::instance().create ("stretch", transp, e);
		push (stretch, false);	// push stretch(transp) as replacement
		fMatched = true;
	}
	else if (*exp == fTarget) {		// target expression found ?
		push (fIdent, false);	// push the replacement
		fMatched = true;
	}
	else cloneExpVisitor::visitStart(exp);
}

}
