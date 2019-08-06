#ifndef __ARDaCoda__
#define __ARDaCoda__

/*
  GUIDO Library
  Copyright (C) 2004-2017  Grame

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

@tagname:\daCoda
@tagalias:
@tagtype:P
@tagnotation:Da Coda
@tagdesc
@tagend

*/

/** \brief to coda (\\daCoda tag)
*/
class ARDaCoda : public ARJump
{
	public:	
					 ARDaCoda() : ARJump ("to Coda [coda]") {}
		virtual 	~ARDaCoda() {}

		virtual const char*	getTagName() const		{ return "ARDaCoda"; };
		virtual std::string getGMNName() const		{ return "\\daCoda"; };

		virtual void browse(TimeUnwrap& mapper) const
				{ mapper.AtPos (this, TimeUnwrap::kDaCoda); }
};

#endif
