#ifndef ARPossibleBreak_H
#define ARPossibleBreak_H

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
class ARPossibleBreak :  public ARMusicalTag // , public ARMusicalObject
{
public:
                 ARPossibleBreak() { }
    virtual     ~ARPossibleBreak() { }

    virtual void printName(std::ostream& os) const { os << "ARPossibleBreak"; }
    virtual void printParameters(std::ostream& os) const {}

    virtual float getValue()                { return value; }
    virtual void  setValue(float newValue) { value = newValue; }

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARPossibleBreak() { return this; }
    /****************************************/

protected:
    float value;
};


#endif
