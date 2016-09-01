#ifndef ARSecondGlue_H
#define ARSecondGlue_H

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

#include "ARMusicalTag.h"
// #include "ARMusicalObject.h"


/** \brief not yet documented
*/
class ARSecondGlue : public ARMusicalTag
		// , public ARMusicalObject
{
public:
                 ARSecondGlue() {}
    virtual	    ~ARSecondGlue() {}

    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const;

    virtual bool IsStateTag() const { return true; }

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARSecondGlue() { return this; }
    /****************************************/
};

#endif

