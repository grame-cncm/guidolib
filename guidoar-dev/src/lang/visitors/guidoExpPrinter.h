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

#ifndef __guidoExpReader__
#define __guidoExpReader__

#include <ostream>
#include <stack>

#include "guidoExprTypes.h"
#include "visitor.h"

namespace guidolang 
{

//______________________________________________________________________________
/*!
\brief	a guido language expression printer
*/
class export guidoExpPrinter : 
	public guido::visitor<Sguidoexpression>,
	public guido::visitor<SguidoNamedExpr>,
	public guido::visitor<SguidoGroupedExpr>,
	public guido::visitor<SguidoAbstractExpr>,
	public guido::visitor<SguidoIdentExpr>,
	public guido::visitor<SguidoScoreExpr>
{ 
	private:
		bool	fInNamed;
		std::ostream&	fOut;					///< the output stream 
		std::stack<const char *> fPendingOp;	///< the pending operations names

	public:
				 guidoExpPrinter(std::ostream& out) : fInNamed(false), fOut(out) {}
		virtual ~guidoExpPrinter() {}

	protected:
		virtual void visitStart( Sguidoexpression&);
		virtual void visitStart( SguidoNamedExpr&);
		virtual void visitEnd  ( SguidoNamedExpr&);
		virtual void visitStart( SguidoGroupedExpr&);
		virtual void visitEnd  ( SguidoGroupedExpr&);
		virtual void visitStart( SguidoAbstractExpr&);
		virtual void visitStart( SguidoScoreExpr&);
		virtual void visitStart( SguidoIdentExpr&);
		
				void checkOp (bool pop=true);	// check if operation need to be flushed
				void push (const char *);		// push operation name on a stack
				void stackPrint ();
};

} // namespace

#endif
