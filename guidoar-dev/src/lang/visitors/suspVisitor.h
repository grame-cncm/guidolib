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

#ifndef __suspVisitor__
#define __suspVisitor__

#include <ostream>

#include "guidoSuspTypes.h"
#include "visitor.h"

namespace guidolang
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A susps visitor
*/
class export suspVisitor : 
	public guido::visitor<Sguidosusp>,
	public guido::visitor<SguidoApplySusp>,
	public guido::visitor<SguidoClosureSusp>,
	public guido::visitor<SguidoSeqSusp>,
	public guido::visitor<SguidoParSusp>
{
    public:
				 suspVisitor() {}
       	virtual ~suspVisitor() {}
		
		Sguidovalue force (const Sguidovalue&);

	protected:
		virtual void visitStart ( Sguidovalue&);
		virtual void visitStart ( Sguidosusp&);
		virtual void visitStart ( SguidoApplySusp&);
		virtual void visitStart	( SguidoClosureSusp&);
		virtual void visitStart	( SguidoSeqSusp&);
		virtual void visitStart	( SguidoParSusp&);
		
		Sguidovalue fValue;
};

/*! @} */

} // namespace

#endif
