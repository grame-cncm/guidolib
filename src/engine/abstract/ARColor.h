#ifndef ARColor_H
#define ARColor_H

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

/** \brief The color tag. Ready to support the Alpha componenent for transparency (RGBA).
*/

class ARColor : public ARMTParameter 
{
public:
    ARColor() : colorR(0), colorG(0), colorB(0), colorA(0) { }

    virtual bool IsStateTag() const;
    virtual void setTagParameterList( TagParameterList & tpl );

    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const;

    float getColorR() const { return colorR; }
    float getColorG() const { return colorG; }
    float getColorB() const { return colorB; }
    float getColorA() const { return colorA; }

protected:
    static ListOfTPLs ltpls;
    float colorR;			// (JB) should be unsigned char components ?
    float colorG;
    float colorB;
    float colorA;
};

#endif
