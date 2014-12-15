#ifndef ARCrescendo_H
#define ARCrescendo_H

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
#include "ARPositionTag.h"


/** \brief not yet documented
*/
class ARCrescendo : public ARMTParameter, public ARPositionTag
{
public:
    ARCrescendo();
    ARCrescendo(const ARCrescendo* crescendo);
    virtual		~ARCrescendo();

    virtual void setTagParameterList(TagParameterList & tlist);
    virtual bool MatchEndTag(const char * s);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

    const NVstring &getDynamicMarking() const { return dynamicMarking; }
    const float     getDx1()            const { return dx1; }
	const float     getDx2()            const { return dx2; }
    const float     getDy()             const { return dy; }
	const float     getDeltaY()         const { return deltaY; }
    const float     getThickness()      const { return thickness; }

protected:

    NVstring dynamicMarking;
    float    dx1;
	float    dx2;
    float    dy;
	float    deltaY;
    float    thickness;

    static ListOfTPLs ltpls;
};

#endif
