#ifndef ARAccent_H
#define ARAccent_H

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

#include "ARArticulation.h"
#include "TimeUnwrap.h"

/*@mkdoc

@group:Articulations

@tagname:\accent
@tagalias:\acc
@tagtype:R
@tagnotation:accent sign
@tagdesc
@tagend

@params:
@param:position:string:above or below:*none*:true
@paramdesc
Accent position is determined by the stem direction: the accent should be placed on the opposite side of the stem. When no stem is present, the accent is placed above.

See the [Articulations](/examples/articulations/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARAccent : public ARArticulation
{		
	public:
				 ARAccent()  {}
		virtual ~ARAccent()  {};

		virtual const char*	getTagName () const		{ return "ARAccent"; };
		virtual std::string getGMNName () const		{ return "\\accent"; };

		virtual void	browse(TimeUnwrap& mapper) const { mapper.AtPos (this, TimeUnwrap::kAccent); }
};

#endif
