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

#ifndef __guidoexpression__
#define __guidoexpression__

#include <string>

#include "export.h"
#include "ctree.h"
#include "functor.h"
#include "guidoExprEnum.h"
#include "gar_smartpointer.h"
#include "visitable.h"
#include "visitor.h"

namespace guidolang 
{

class guidoexpression;
class guidovalue;
class guidoEnv;

typedef guido::SMARTP<guidoexpression> 	Sguidoexpression;
typedef guido::SMARTP<guidovalue>		Sguidovalue;
typedef guido::SMARTP<guidoEnv>			SguidoEnv;

//______________________________________________________________________________
/*!
\brief The base class for guido language expressions.
*/
class export guidoexpression : public guido::ctree<guidoexpression>, public guido::visitable
{
    protected:
		std::string	fName;
		int			fType;

				 guidoexpression(int type) : fType(type) {}
		virtual ~guidoexpression() {}
		virtual	void	basicprint(std::ostream& os);

	public:		
		virtual const std::string&	getName() const		{ return fName; }
		virtual int					getType() const		{ return fType; }

		virtual void		acceptIn(guido::basevisitor& visitor);
		virtual void		acceptOut(guido::basevisitor& visitor);
		virtual	void		print(std::ostream& os);
		
		virtual Sguidoexpression	replace (const Sguidoexpression& exp, const Sguidoexpression& with);
//		virtual Sguidovalue			suspend(SguidoEnv env);
		virtual Sguidovalue			eval(SguidoEnv env);

		//________________________________________________________________________
		virtual Sguidoexpression getArg(unsigned int n) const;
		virtual bool operator ==(const Sguidoexpression& i) const		{ return false; }
		virtual bool operator !=(const Sguidoexpression& i) const		{ return !(*this == i); }
		
		void setName(const std::string& name)	{ fName = name; }
};

export std::ostream& operator << (std::ostream& os, const Sguidoexpression& elt);

//______________________________________________________________________________
/*
\brief	A template class to type all guido expressions with integers.
*/
template <int elt> class guidonode : public guidoexpression
{
	public:
		static guido::SMARTP<guidonode<elt> > create()
			{ guidonode<elt>* o = new guidonode<elt>(elt); assert(o!=0); return o; }
		
		virtual void acceptIn(guido::basevisitor& v) {
			if (guido::visitor<guido::SMARTP<guidonode<elt> > >* p = dynamic_cast<guido::visitor<guido::SMARTP<guidonode<elt> > >*>(&v)) {
				guido::SMARTP<guidonode<elt> > sptr = this;
				p->visitStart(sptr);
			}
			else guidoexpression::acceptIn(v);
		}
        virtual void acceptOut(guido::basevisitor& v) {
			if (guido::visitor<guido::SMARTP<guidonode<elt> > >* p = dynamic_cast<guido::visitor<guido::SMARTP<guidonode<elt> > >*>(&v)) {
				guido::SMARTP<guidonode<elt> > sptr = this;
				p->visitEnd(sptr);
			}
			else guidoexpression::acceptOut(v);
		}

 		virtual bool operator ==(const Sguidoexpression& e) const { 
			if (e->getType() == kNamed)
				return *this == e->getArg(0); 

			if (dynamic_cast<guidonode<elt>*>((guidoexpression*)(e))) {
				for (int i=0; i < size(); i++) {
					Sguidoexpression argi = getArg(i);
					Sguidoexpression eargi = e->getArg(i);
					if (!argi || !eargi || (argi != eargi)) 
						return false;
				}
				return true;
			}
			return false;		
		}

	protected:
				 guidonode(int type) : guidoexpression(type) {}
		virtual ~guidonode() {}

};

} // namespace

#endif
