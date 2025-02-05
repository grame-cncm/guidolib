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

#ifndef __replaceVisitor__
#define __replaceVisitor__

#include <ostream>

#include "cloneExpVisitor.h"

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
class export replaceVisitor : 
	public cloneExpVisitor,
	public guido::visitor<SguidoIdentExpr>
{
    public:
				 replaceVisitor(bool createStretch=false) : fCopy(true), fMatched(false), fStretchScore(createStretch) {}
       	virtual ~replaceVisitor() {}
		
		virtual Sguidoexpression replace(const Sguidoexpression& exp, const Sguidoexpression& target, const Sguidoexpression& with);
		bool	matched()		{ return fMatched; }

	protected:
		Sguidoexpression	fTarget, fIdent;
		bool	fCopy, fMatched, fStretchScore;          
		virtual bool copy  () const	{ return fCopy; }

		virtual void visitStart ( Sguidoexpression&);
		virtual void visitEnd	( Sguidoexpression&);
		virtual void visitStart ( SguidoScoreExpr&);
		virtual void visitStart ( SguidoIdentExpr&);
		virtual void visitEnd	( SguidoIdentExpr&);
		
		void replace(const Sguidoexpression& ident);
};

/*! @} */

} // namespace

#endif
