#ifndef ARRest_H
#define ARRest_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMusicalEvent.h"

/** \brief Abstract representation of the rest event.
*/
class ARRest : public ARMusicalEvent 
{
public:
			 ARRest (const TYPE_DURATION & durationOfRest);
			 ARRest (const TYPE_TIMEPOSITION & starttimeOfRest, const TYPE_DURATION & durationOfRest);
			 ARRest (int theNumerator, int theDenominator);
		     ARRest (const ARRest &arrest);
    virtual ~ARRest();

	virtual ARMusicalObject * Copy() const;

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void browse(TimeUnwrap& mapper) const;

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARRest() { return this; }
    /****************************************/
};

#endif


