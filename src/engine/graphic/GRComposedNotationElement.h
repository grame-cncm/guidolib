#ifndef GRComposedNotationElement_H
#define GRComposedNotationElement_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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


