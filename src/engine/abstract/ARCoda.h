#ifndef ARCoda_H
#define ARCoda_H

/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARJump.h"
#include "TimeMapper.h"

/*@mkdoc

@group:Repeat Signs

@tagname:\coda
@tagalias:
@tagtype:P
@tagnotation:the code sign
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

*/

/** \brief Coda
*/
class ARCoda : public ARJump
{
	public:	
					 ARCoda() : ARJump ("[coda] Coda") {}
		virtual 	~ARCoda() {}

		virtual const char*	getTagName() const		{ return "ARCoda"; };
		virtual std::string getGMNName() const		{ return "\\coda"; };

		virtual void browse(TimeUnwrap& mapper) const { mapper.AtPos (this, TimeUnwrap::kCoda); }

		ARMusicalObject *isARCoda() 				{ return this; }
};

#endif
