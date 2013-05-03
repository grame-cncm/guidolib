#ifndef ARTArticulation_H
#define ARTArticulation_H

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

#include "ARMTParameter.h"
//#include "ARMusicalObject.h"
#include "ARPositionTag.h"

/** \brief (unused !)
*/

class ARTArticulation : public ARMTParameter, public ARPositionTag	// public ARMusicalObject,
{
  public:
	  
	  				ARTArticulation() 	{ rangesetting = ONLY; }
	 virtual		~ARTArticulation();

	 virtual void 	setTagParameterList( TagParameterList & tl );


  protected:

};

#endif


