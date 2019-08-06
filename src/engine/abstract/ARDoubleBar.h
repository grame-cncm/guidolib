#ifndef ARDoubleBar_H
#define ARDoubleBar_H

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

@tagname:\doubleBar
@tagalias:
@tagtype:P
@tagnotation:a double bar line
@tagdesc
@tagend

@paramdesc
The **\doubleBar** tag supports the same parameters than the **\bar** tag.

See the [Barlines](/examples/barlines/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARDoubleBar : public ARBar  
{
	public:
				 ARDoubleBar() {}
		virtual ~ARDoubleBar() {}

		virtual const char*	getTagName() const		{ return "ARDoubleBar"; };
		virtual std::string getGMNName() const		{ return "\\doubleBar"; };
};

#endif
