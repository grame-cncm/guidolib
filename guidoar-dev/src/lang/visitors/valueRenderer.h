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

#ifndef __valueVisitor__
#define __valueVisitor__

#include <ostream>

#include "guidoValueTypes.h"
#include "guidoelement.h"
#include "visitor.h"

namespace guidolang
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A values visitor
*/
class export valueRenderer : 
	public guido::visitor<Sguidovalue>,
	public guido::visitor<SguidoClosureValue>,
	public guido::visitor<SguidoApplyValue>,
	public guido::visitor<SguidoSeqValue>,
	public guido::visitor<SguidoScoreValue>,
	public guido::visitor<SguidoMixValue>,
	public guido::visitor<SguidoEvalSusp>
{
    public:
				 valueRenderer() : fLimit (20) {}
       	virtual ~valueRenderer() {}
		
		guido::Sguidoelement render(const Sguidoexpression&);
		guido::Sguidoelement render(const Sguidovalue&);

	protected:
		virtual void visitStart ( Sguidovalue&);
		virtual void visitStart	( SguidoSeqValue&);
		virtual void visitStart ( SguidoScoreValue&);
		virtual void visitStart	( SguidoMixValue&);
		virtual void visitStart	( SguidoEvalSusp&);
		virtual void visitStart	( SguidoClosureValue&);
		virtual void visitStart	( SguidoApplyValue&);
		
		unsigned int fPos, fLimit;
		guido::Sguidoelement fScore;
};

/*! @} */

} // namespace

#endif
