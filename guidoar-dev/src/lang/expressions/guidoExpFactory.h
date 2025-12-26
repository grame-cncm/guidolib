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

#ifndef __guidoExpFactory__
#define __guidoExpFactory__

#include <string>
#include <map>

#include "functor.h"
#include "guidoExprTypes.h"
#include "guidoelement.h"
#include "singleton.h"

namespace guidolang 
{

typedef guido::rfunctor<Sguidoexpression> NewNodeFunctor;

/*!
\brief A guido language expressions factory.
*/
class export guidoExpFactory : public singleton<guidoExpFactory>
{
	private:
		std::map<std::string, NewNodeFunctor*>	fMap;

    public:
				 guidoExpFactory();
		virtual ~guidoExpFactory() {}

		Sguidoexpression		create(const std::string& name) const;	
		Sguidoexpression		create(guido::Sguidoelement& score) const;	
		Sguidoexpression		create(const std::string& name, Sguidoexpression& e) const;	
		Sguidoexpression		createNamed(const std::string& name, Sguidoexpression& e) const;	
		Sguidoexpression		create(const std::string& name, Sguidoexpression& e1, Sguidoexpression& e2) const;	
		Sguidoexpression		createAbstract(const std::string& name, Sguidoexpression& e1, Sguidoexpression& e2) const;	
};

} // namespace

#endif
