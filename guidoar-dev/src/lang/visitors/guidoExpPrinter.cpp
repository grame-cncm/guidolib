/*
  GUIDO Library
  Copyright (C) 2008  Grame

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

#ifndef __guidoExpPrinter__
#define __guidoExpPrinter__

#include <iostream>

#include "guidoExpPrinter.h"
#include "guidoexpression.h"
#include "guidoScoreExpr.h"
#include "guidoNamedExpr.h"
#include "tree_browser.h"

using namespace std;
using namespace guido;

//#define printDebug
#ifdef printDebug
#define debug(msg)	cout << " -> " << msg << " pending: " << fPendingOp.size() << ": "; stackPrint()
#else
#define debug(msg)
#endif

namespace guidolang 
{

//______________________________________________________________________________
void guidoExpPrinter::stackPrint () 
{
	stack<const char *> s;
	int size = fPendingOp.size();
	for (int i=0; i< size; i++) {
		const char * str = fPendingOp.top();
		fPendingOp.pop();
		s.push(str);
		cout << (str ? str : "0") << " ";		
	}
	for (int i=0; i< size; i++) {
		const char * str = s.top();
		s.pop();
		fPendingOp.push(str);
	}
	cout << endl;
}

//______________________________________________________________________________
void guidoExpPrinter::checkOp (bool pop) 
{
	if (fPendingOp.size()) {
		const char * op = fPendingOp.top();
		if (op) fOut << op << " ";
		if (pop || op) fPendingOp.pop();
	}
}

//______________________________________________________________________________
void guidoExpPrinter::push (const char* opString) 
{
	fPendingOp.push(opString);
	fPendingOp.push(0);
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void guidoExpPrinter::visitStart (Sguidoexpression& exp)
{ 
	if (fInNamed) return;
	debug("guidoExpPrinter::visitStart Sguidoexpression " << exp->getName());
	checkOp ();
	push(exp->getName().c_str());
}

//______________________________________________________________________________
void guidoExpPrinter::visitStart (SguidoGroupedExpr& exp)
{ 
	debug("guidoExpPrinter::visitStart SguidoGroupedExpr");
	checkOp (false);
	fOut << "(" ;
}

//______________________________________________________________________________
void guidoExpPrinter::visitEnd (SguidoGroupedExpr& exp)
{ 
	fOut << ")" << endl;
}

//______________________________________________________________________________
void guidoExpPrinter::visitStart (SguidoNamedExpr& exp)
{ 
	debug("guidoExpPrinter::visitStart SguidoNamedExpr");
	checkOp ();
	fOut << exp->getName() << endl;
	fInNamed = true;
}

//______________________________________________________________________________
void guidoExpPrinter::visitEnd (SguidoNamedExpr& exp)
{ 
	fInNamed = false;
}

//______________________________________________________________________________
void guidoExpPrinter::visitStart( SguidoIdentExpr& exp)
{
	// don't print the identifier but let the enclosed expression be printed
}

//______________________________________________________________________________
void guidoExpPrinter::visitStart( SguidoAbstractExpr& exp)
{
	if (fInNamed) return;
	debug("guidoExpPrinter::visitStart SguidoAbstractExpr");
	checkOp ();
	fOut << "#";
	push(exp->getName().c_str());
}

//______________________________________________________________________________
void guidoExpPrinter::visitStart (SguidoScoreExpr& exp) 
{
	if (fInNamed) return;
	debug("guidoExpPrinter::visitStart SguidoScoreExpr");
	checkOp ();
	fOut << exp->getScore() << endl;
}


} // namespace

#endif
