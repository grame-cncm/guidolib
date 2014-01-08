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

#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"

#include "ARCrescendo.h"

ListOfTPLs ARCrescendo::ltpls(1);

ARCrescendo::ARCrescendo() : dx1(0), dx2(0), dy(0), deltaY(75), thickness(4)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

ARCrescendo::ARCrescendo(const ARCrescendo * crescendo) : ARMTParameter(-1, crescendo)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

    if (crescendo)
    {
        dynamicMarking = crescendo->getDynamicMarking();
        dx1            = crescendo->getDx1();
        dx2            = crescendo->getDx2();
        dy             = crescendo->getDy();
        deltaY         = crescendo->getDeltaY();
        thickness      = crescendo->getThickness();
    }
    else
    {
        //Failure
    }
}

ARCrescendo::~ARCrescendo()
{
}

void ARCrescendo::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		lstrs.AddTail("S,dm,,o;U,dx1,0,o;U,dx2,0,o;U,dy,0,o;U,deltaY,3,o;U,thickness,0.16,o");
		
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
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARCrescendo::print() const 
{
}

bool ARCrescendo::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;

	if ((getRange() == false ) && !strcmp("\\crescEnd",s))
		return true;
	return false;
}

void ARCrescendo::PrintName(std::ostream & os) const
{
	if (getRange())
		os << "\\cresc";
	else
		os << "\\crescBegin";
}

