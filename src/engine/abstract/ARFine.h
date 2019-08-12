#ifndef __ARFine__
#define __ARFine__

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARJump.h"
#include "TimeUnwrap.h"

/*@mkdoc

@group:Repeat Signs

@tagname:\fine
@tagalias:
@tagtype:P
@tagnotation:Fine
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

*/


/** \brief Fine
*/
class ARFine : public ARJump
{
public:	
				 ARFine() : ARJump ("Fine") {}
	virtual 	~ARFine() {}

		virtual const char*	getTagName() const		{ return "ARFine"; };
		virtual std::string getGMNName() const		{ return "\\fine"; };

		virtual void browse (TimeUnwrap& mapper) const
				{ mapper.AtPos (this, TimeUnwrap::kFine); }
};

#endif
