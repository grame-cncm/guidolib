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

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "guidoExpFactory.h"

#include "guidoexpression.h"
#include "guidoScoreExpr.h"
#include "guidoNamedExpr.h"

using namespace std; 
using namespace guido; 

namespace guidolang
{

//______________________________________________________________________________
template<int elt>
class newNodeFunctor : public rfunctor<Sguidoexpression> {
	public:
		Sguidoexpression operator ()() { return guidonode<elt>::create(); }
};

//______________________________________________________________________________
guidoExpFactory::guidoExpFactory()
{
	fMap["."]			= new newNodeFunctor<kAbstract>;
	fMap["@"]			= new newNodeFunctor<kApply>;
	fMap[":"]			= new newNodeFunctor<kSeqOp>;
	fMap["/"]			= new newNodeFunctor<kParOp>;
	fMap["-|"]			= new newNodeFunctor<kHeadOp>;
	fMap["|-"]			= new newNodeFunctor<kTailOp>;
	fMap["-/"]			= new newNodeFunctor<kTopOp>;
	fMap["/-"]			= new newNodeFunctor<kTopOp>;
	fMap["transp"]		= new newNodeFunctor<kTransp>;
	fMap["stretch"]		= new newNodeFunctor<kStretch>;
 	fMap["ident"]		= new newNodeFunctor<kIdent>;
 	fMap["group"]		= new newNodeFunctor<kGrouped>;
}

//______________________________________________________________________________
Sguidoexpression guidoExpFactory::create(const string& name) const
{ 
	map<std::string, NewNodeFunctor*>::const_iterator i = fMap.find( name );
	if (i != fMap.end()) {
		NewNodeFunctor* f= i->second;
		if (f) {
			Sguidoexpression elt = (*f)();
			if (elt) elt->setName(name);
			return elt;
		}
	}
	cerr << "Sguidoelement factory::create called with unknown element \"" << name << "\"" << endl;
	return 0;
}

//______________________________________________________________________________
Sguidoexpression guidoExpFactory::create(guido::Sguidoelement& score) const
{
	SguidoScoreExpr exp = guidoScoreExpr::create (score);
	if (exp) exp->setName("gmn");
	return exp;
}

//______________________________________________________________________________
Sguidoexpression guidoExpFactory::create(const std::string& name, Sguidoexpression& e) const
{
	Sguidoexpression exp = create (name);
	if (exp) exp->push(e);
	return exp;
}

//______________________________________________________________________________
Sguidoexpression guidoExpFactory::createNamed(const std::string& name, Sguidoexpression& e) const
{
	Sguidoexpression exp = guidoNamedExpr::create();
	if (exp) {
		if (exp) exp->setName(name);
		exp->push(e);
	}
	return exp;
}

//______________________________________________________________________________
Sguidoexpression guidoExpFactory::create(const std::string& name, Sguidoexpression& e1, Sguidoexpression& e2) const
{
	Sguidoexpression exp = create (name);
	if (exp) {
		exp->push(e1);
		exp->push(e2);
	}
	return exp;
}

//______________________________________________________________________________
Sguidoexpression guidoExpFactory::createAbstract(const std::string& name, Sguidoexpression& e1, Sguidoexpression& e2) const
{
	Sguidoexpression exp = create (name);
	Sguidoexpression ident = create("ident", e1);
	Sguidoexpression e = e2->replace(e1,ident);
	if (exp && ident && e) {
#if 0
		exp->push(e1);
		exp->push(e2);
#else
		exp->push(ident);
		exp->push(e);
#endif
	}
	return exp;
}

} // namespace
