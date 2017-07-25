#ifndef ARTenuto_H
#define ARTenuto_H

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


/** \brief the Tenuto articulation tag
*/
class ARTenuto : public ARArticulation
{		
	public:		
			 	 ARTenuto() {}
    	virtual ~ARTenuto() {};

		virtual const char*	getTagName() const		{ return "ARTenuto"; };
		virtual std::string getGMNName() const		{ return "\\tenuto"; };

		virtual void browse(TimeUnwrap& mapper) const { mapper.AtPos (this, TimeUnwrap::kTenuto); }
};

#endif
