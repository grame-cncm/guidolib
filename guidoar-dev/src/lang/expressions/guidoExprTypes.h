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

#ifndef __guidoExpTypes__
#define __guidoExpTypes__

#include "guidoExprEnum.h"
#include "gar_smartpointer.h"

namespace guidolang 
{

class guidoexpression;
class guidoScoreExpr;
class guidoNamedExpr;
template<int T> class guidonode;

typedef guido::SMARTP<guidoexpression>			Sguidoexpression;
typedef guido::SMARTP<guidonode<kAbstract> > 	SguidoAbstractExpr;
typedef guido::SMARTP<guidonode<kApply> >		SguidoApplyExpr;
typedef guido::SMARTP<guidonode<kTransp> >		SguidoTranspExpr;
typedef guido::SMARTP<guidonode<kStretch> >		SguidoStretchExpr;
typedef guido::SMARTP<guidonode<kSeqOp> >		SguidoSeqExpr;
typedef guido::SMARTP<guidonode<kParOp> >		SguidoParExpr;
typedef guido::SMARTP<guidonode<kHeadOp> >		SguidoHeadExpr;
typedef guido::SMARTP<guidonode<kTailOp> >		SguidoTailExpr;
typedef guido::SMARTP<guidonode<kTopOp> >		SguidoTopExpr;
typedef guido::SMARTP<guidonode<kBottomOp> >	SguidoBottomExpr;
typedef guido::SMARTP<guidonode<kIdent> >		SguidoIdentExpr;
typedef guido::SMARTP<guidonode<kGrouped> >		SguidoGroupedExpr;

typedef guido::SMARTP<guidoScoreExpr>			SguidoScoreExpr;
typedef guido::SMARTP<guidoNamedExpr>			SguidoNamedExpr;

class guidoEnv;
typedef guido::SMARTP<guidoEnv>				SguidoEnv;
class guidovalue;
typedef guido::SMARTP<guidovalue>			Sguidovalue;


} // namespace

#endif
