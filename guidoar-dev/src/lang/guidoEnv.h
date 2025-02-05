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

#ifndef __guidoEnv__
#define __guidoEnv__

#include <map>

#include "guidoexpression.h"
#include "guidovalue.h"
#include "gar_smartpointer.h"

namespace guidolang 
{
/*
class guidoexpression;
typedef guido::SMARTP<guidoexpression> 	Sguidoexpression;
*/
class guidoEnv;
typedef guido::SMARTP<guidoEnv> 	SguidoEnv;

/*!
\brief The guido language environment definition.
*/
class guidoEnv : public guido::smartable
{
	private:
		typedef std::map<Sguidoexpression, Sguidovalue>	Associations;
		std::map<Sguidoexpression, Sguidovalue>	fAssociations;
		
    protected:
				 guidoEnv() {}
				 guidoEnv(Sguidoexpression& e, Sguidovalue& v) { bind(e, v); }
		virtual ~guidoEnv() {}

	public:
        static SguidoEnv create()	{ guidoEnv * o = new guidoEnv(); assert(o!=0); return o; }
        static SguidoEnv create(Sguidoexpression& e, Sguidovalue& v)
									{ guidoEnv * o = new guidoEnv(e, v); assert(o!=0); return o; }

		SguidoEnv	bind (Sguidoexpression& e, Sguidovalue& v)	{ fAssociations[e] = v; return this; }
		void		clear()										{ fAssociations.clear(); }
		int			size() const								{ return fAssociations.size(); }
		
		Sguidovalue value(Sguidoexpression& e) { 
			std::pair<Associations::const_iterator, Associations::const_iterator> i = fAssociations.equal_range(e);
			if (i.first != i.second) return (*i.first).second;
			return 0;
		}
		
		void		print(std::ostream& os)	const { 
			os << "env size:" << size() << " : ";
			Associations::const_iterator i = fAssociations.begin();
			while (i != fAssociations.end()) {
				os << i->first << "(" << (void*)i->first << "):" << (void*)(i->second) << " ";
				i++;
			}
		}
};

inline std::ostream& operator << (std::ostream& os, const SguidoEnv& env) { env->print(os); return os; }

} // namespace

#endif
