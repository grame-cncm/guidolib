#ifndef ARSpecial_H
#define ARSpecial_H
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

#include "ARMTParameter.h"

/** \brief not yet documented
*/
class ARSpecial : // public ARMusicalObject,
	public ARMTParameter  
{
public:
             ARSpecial();
    virtual ~ARSpecial();

    virtual void setTagParameterList(TagParameterList &tlist);

    virtual void printName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const;

    float 		dx;
    float 		dy;
    float 		space;
    float 		scale;
    unsigned char mDrawChar;	// may become int...

protected:
    static ListOfTPLs ltpls;
};

#endif
