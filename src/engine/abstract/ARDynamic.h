#ifndef ARDynamic_H
#define ARDynamic_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"


/** \brief the base class for ARCrescendo and ARDiminuendo
*/
class ARDynamic : public ARMTParameter, public ARPositionTag
{
public:
				 ARDynamic();
				 ARDynamic(const ARDynamic* dynamic);
    virtual		~ARDynamic() {}

    virtual void setTagParameterList(TagParameterList & tlist);
	virtual void printParameters(std::ostream& os) const;

    const NVstring &getDynamicMarking() const { return dynamicMarking; }
    const float     getDx1()            const { return dx1; }
	const float     getDx2()            const { return dx2; }
    const float     getDy()             const { return dy; }
	const float     getDeltaY()         const { return deltaY; }
    const float     getThickness()      const { return thickness; }
	bool			autoPos()			const { return fAutoPos; }

protected:
    NVstring dynamicMarking;
    float    dx1;
	float    dx2;
    float    dy;
	float    deltaY;
    float    thickness;
	bool	 fAutoPos;

    static ListOfTPLs ltpls;
};

#endif
