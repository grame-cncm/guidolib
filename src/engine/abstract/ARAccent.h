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
