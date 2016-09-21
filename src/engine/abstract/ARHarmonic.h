#ifndef ARHarmonic_H
#define ARHarmonic_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARArticulation.h"

/*brief		The harmonic tag */
class ARHarmonic : public ARArticulation
{
public :
//			 ARHarmonic() : ARMTParameter() { rangesetting = ONLY; }
			 ARHarmonic() { fPosition = kAbove; }
	virtual ~ARHarmonic() {};
			
    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;
};

#endif
