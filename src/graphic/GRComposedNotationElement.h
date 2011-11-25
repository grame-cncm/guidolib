#ifndef GRComposedNotationElement_H
#define GRComposedNotationElement_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/

#ifdef VC6
#pragma warning (disable : 4786)
#endif

//#include "NEList.h"
#include "NEList.h"
//#include "GRNotationElement.h"
#include "GRNotationElement.h"


/** \brief not yet documented
*/
class GRComposedNotationElement : virtual public GRNotationElement
{

  public:

      ~GRComposedNotationElement();

      const NEList * get_elements() const;
      void set_elements(NEList *const  value);


  private:

      NEList *elements;
};


inline const NEList * GRComposedNotationElement::get_elements() const
{
	return elements;
}

inline void GRComposedNotationElement::set_elements(NEList * const value)
{
	elements = value;
}





#endif


