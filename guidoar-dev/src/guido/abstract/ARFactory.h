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

#ifndef __ARFactory__
#define __ARFactory__

#include <string>
#include <map>
#include "arexport.h"
#include "functor.h"
#include "singleton.h"
#include "ARTypes.h"


/*!
\addtogroup abstract
@{
*/

namespace guido 
{

typedef functor<Sguidotag,long>	NewTagFunctor;


//______________________________________________________________________________
/*!
\brief A factory for creating GAR objects
*/
class gar_export ARFactory : public singleton<ARFactory>{

	typedef std::map<std::string, NewTagFunctor*>	TFunctors;
	TFunctors fMap;
	public:
				 ARFactory();
		virtual ~ARFactory();

		Sguidotag	createTag(const std::string& elt, long id=0) const;	
		SARMusic	createMusic() const;	
		SARVoice	createVoice() const;	
		SARChord	createChord() const;	
		SARNote		createNote(const std::string& name) const;	
};

} // namespace

/*! @} */

#endif
