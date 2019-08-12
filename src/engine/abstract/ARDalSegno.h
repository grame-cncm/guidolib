#ifndef __ARDalSegno__
#define __ARDalSegno__

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

@tagname:\dalSegno
@tagalias:
@tagtype:P
@tagnotation:Dal Segno
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

@tagname:\dalSegnoAlFine
@tagalias:
@tagtype:P
@tagnotation:Dal Segno al Fine
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

*/

/** \brief dal segno
*/
class ARDalSegno : public ARJump
{
	public:	
					 ARDalSegno() : ARJump ("D.S.") {}
		virtual 	~ARDalSegno() {}

		virtual const char*	getTagName() const		{ return "ARDalSegno"; };
		virtual std::string getGMNName() const		{ return "\\dalSegno"; };

		virtual void browse(TimeUnwrap& mapper) const
				{ mapper.AtPos (this, TimeUnwrap::kDalSegno); }
};

/** \brief dal segno al fine
*/
class ARDalSegnoAlFine : public ARJump
{
	public:	
					 ARDalSegnoAlFine() : ARJump ("D.S. al Fine") {}
		virtual 	~ARDalSegnoAlFine() {}

		virtual const char*	getTagName() const		{ return "ARDalSegnoAlFine"; };
		virtual std::string getGMNName() const		{ return "\\dalSegnoAlFine"; };

		virtual void browse(TimeUnwrap& mapper) const
				{ mapper.AtPos (this, TimeUnwrap::kDalSegnoAlFine); }
};

#endif
