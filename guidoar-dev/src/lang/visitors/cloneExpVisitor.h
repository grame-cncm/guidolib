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

#ifndef __cloneExpVisitor__
#define __cloneExpVisitor__

#include <ostream>

#include "export.h"
#include "guidoExprTypes.h"
#include "guidoexpression.h"
#include "visitor.h"
#include "tree_browser.h"

namespace guidolang
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to print the gmn description
*/
class export cloneExpVisitor : 
	public guido::tree_browser<guidoexpression>,
	public guido::visitor<Sguidoexpression>,
	public guido::visitor<SguidoScoreExpr>
{
    public:
				 cloneExpVisitor() { set(this); }
       	virtual ~cloneExpVisitor() {}
              
		virtual Sguidoexpression clone(const Sguidoexpression&);

	protected:		
		virtual void push	(const Sguidoexpression& exp, bool stack=true);

		virtual void visitStart ( Sguidoexpression&);
		virtual void visitEnd	( Sguidoexpression&);
		virtual void visitStart ( SguidoScoreExpr&);
		virtual void visitEnd	( SguidoScoreExpr&);

		std::stack<Sguidoexpression> fStack;
};

/*! @} */

} // namespace

#endif
