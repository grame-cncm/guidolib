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

#include <string.h>

#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"

#include "ARDynamic.h"

using namespace std;

ListOfTPLs ARDynamic::ltpls(1);

//--------------------------------------------------------------------------------------
ARDynamic::ARDynamic() : dynamicMarking(""), dx1(0), dx2(0), dy(0), deltaY(75), thickness(4), fAutoPos(false)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

//--------------------------------------------------------------------------------------
ARDynamic::ARDynamic(const ARDynamic * dyn) : ARMTParameter(-1, dyn),
	dynamicMarking(""), dx1(0), dx2(0), dy(0), deltaY(75), thickness(4), fAutoPos(false)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

    if (dyn)
    {
        dynamicMarking = dyn->getDynamicMarking();
        dx1            = dyn->getDx1();
        dx2            = dyn->getDx2();
        dy             = dyn->getDy();
        deltaY         = dyn->getDeltaY();
        thickness      = dyn->getThickness();
		fAutoPos	   = dyn->fAutoPos;
    }
}

//--------------------------------------------------------------------------------------
void ARDynamic::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		lstrs.AddTail("S,dm,,o;U,dx1,0,o;U,dx2,0,o;U,dy,0,o;U,deltaY,3,o;U,thickness,0.16,o;S,autopos,off,o");
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
            TagParameterString *s = TagParameterString::cast(rtpl->RemoveHead());
            dynamicMarking = s->getValue();
            delete s;

			TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
            dx1 = f->getValue();
            delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
            dx2 = f->getValue();
            delete f;

            f = TagParameterFloat::cast(rtpl->RemoveHead());
            dy = f->getValue();
            delete f;

            f = TagParameterFloat::cast(rtpl->RemoveHead());
            deltaY = f->getValue();
            delete f;

            f = TagParameterFloat::cast(rtpl->RemoveHead());
            thickness = f->getValue();
            delete f;

			TagParameterString* autopos = TagParameterString::cast(rtpl->RemoveHead());
			string autoStr = autopos->getValue();
			if (autoStr == "on") fAutoPos = true;
			delete autopos;
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}


//--------------------------------------------------------------------------------------
void ARDynamic::printParameters(std::ostream& os) const
{
    os << "dm: "        << dynamicMarking << "; ";
    os << "dx1: "       << dx1 << "; ";
    os << "dx2: "       << dx2 << "; ";
    os << "deltaY: "    << deltaY << "; ";
    os << "thickness: " << thickness << "; ";

    ARMusicalTag::printParameters(os);
}

