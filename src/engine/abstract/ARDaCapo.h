#ifndef __ARDaCapo__
#define __ARDaCapo__

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
#include "TimeUnwrap.h"

/*@mkdoc

@group:Repeat Signs

@tagname:\daCapo
@tagalias:
@tagtype:P
@tagnotation:Da Capo
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

@tagname:\daCapoAlFine
@tagalias:
@tagtype:P
@tagnotation:Da Capo al Fine
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.<br/>
See the [Brown](@EXAMPLES/brown/) example.
@tagend

*/

/** \brief da capo
*/
class ARDaCapo : public ARJump
{
	public:	
					 ARDaCapo() : ARJump ("D.C.") {}
		virtual 	~ARDaCapo() {}

		virtual const char*	getTagName() const		{ return "ARDaCapo"; };
		virtual std::string getGMNName() const		{ return "\\daCapo"; };

		virtual void browse(TimeUnwrap& mapper) const
					{ mapper.AtPos (this, TimeUnwrap::kDaCapo); }
};

/** \brief da capo al fine
*/
class ARDaCapoAlFine : public ARJump
{
	public:	
					 ARDaCapoAlFine() : ARJump ("D.C. al Fine") {}
		virtual 	~ARDaCapoAlFine() {}

		virtual const char*	getTagName() const		{ return "ARDaCapoAlFine"; };
		virtual std::string getGMNName() const		{ return "\\daCapoAlFine"; };

		virtual void browse(TimeUnwrap& mapper) const
					{ mapper.AtPos (this, TimeUnwrap::kDaCapoAlFine); }
};

#endif
