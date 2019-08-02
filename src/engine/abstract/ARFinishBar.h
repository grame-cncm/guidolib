#ifndef ARFinishBar_H
#define ARFinishBar_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARBar.h"


/*@mkdoc

@group:Barlines

@tagname:\endBar
@tagalias:
@tagtype:P
@tagnotation:an end bar line
@tagdesc
@tagend

@paramdesc
@paramend

*/

/** \brief not yet documented
*/
class ARFinishBar: public ARBar  
{
	public:
				 ARFinishBar() { rangesetting = NO; }

		virtual ~ARFinishBar() {}

		virtual const char*	getTagName() const		{ return "ARFinishBar"; };
		virtual std::string getGMNName() const		{ return "\\endBar"; };

   		 ARMusicalObject *isARFinishBar()			{ return this; }
};

#endif
