/*
  GUIDO Library
  Copyright (C) 2006-2008  Grame

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

#ifndef __guidoEval__
#define __guidoEval__

#include "export.h"
#include "guidoExprTypes.h"
#include "visitor.h"

namespace guidolang 
{

/*! \brief base class for the evaluation functors */
class export guidoEval : 
	public guido::visitor<Sguidoexpression>,
	public guido::visitor<SguidoNamedExpr>,
	public guido::visitor<SguidoGroupedExpr>,
	public guido::visitor<SguidoAbstractExpr>,
	public guido::visitor<SguidoApplyExpr>,
	public guido::visitor<SguidoTranspExpr>,
	public guido::visitor<SguidoStretchExpr>,
	public guido::visitor<SguidoSeqExpr>,
	public guido::visitor<SguidoParExpr>,
	public guido::visitor<SguidoHeadExpr>,
	public guido::visitor<SguidoTailExpr>,
	public guido::visitor<SguidoTopExpr>,
	public guido::visitor<SguidoBottomExpr>,
	public guido::visitor<SguidoIdentExpr>,
	public guido::visitor<SguidoScoreExpr>
{
	private:
		Sguidovalue	fValue;
		SguidoEnv	fEnv;
		
	public:
				 guidoEval() {}
		virtual ~guidoEval() {}
		
		Sguidovalue eval(Sguidoexpression, SguidoEnv);

	protected:
		virtual void evalBinary( Sguidoexpression e, Sguidovalue& v1, Sguidovalue& v2);

		virtual void visitStart( Sguidoexpression&);
		virtual void visitStart( SguidoNamedExpr&);
		virtual void visitStart( SguidoGroupedExpr&);
		virtual void visitStart( SguidoAbstractExpr&);
		virtual void visitStart( SguidoApplyExpr&);
		virtual void visitStart( SguidoTranspExpr&);
		virtual void visitStart( SguidoStretchExpr&);
		virtual void visitStart( SguidoSeqExpr&);
		virtual void visitStart( SguidoParExpr&);
		virtual void visitStart( SguidoHeadExpr&);
		virtual void visitStart( SguidoTailExpr&);
		virtual void visitStart( SguidoTopExpr&);
		virtual void visitStart( SguidoBottomExpr&);
		virtual void visitStart( SguidoIdentExpr&);
		virtual void visitStart( SguidoScoreExpr&);
};

} // namespace

#endif
