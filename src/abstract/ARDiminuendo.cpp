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

#include <iostream>
#include <cstring>

#include "TagParameterList.h"
#include "TagParameterFloat.h"

#include "ARDiminuendo.h"

ListOfTPLs ARDiminuendo::ltpls(1);

ARDiminuendo::ARDiminuendo() : dx1(0), dx2(0), dy(0), deltaY(75), thickness(4)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

ARDiminuendo::ARDiminuendo(const ARDiminuendo * diminuendo) : ARMTParameter(-1, diminuendo)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

    if (diminuendo)
    {
        dx1       = diminuendo->getDx1();
        dx2       = diminuendo->getDx2();
        dy        = diminuendo->getDy();
        deltaY    = diminuendo->getDeltaY();
        thickness = diminuendo->getThickness();
    }
    else
    {
        //Failure
    }
}

ARDiminuendo::~ARDiminuendo()
{
}

void ARDiminuendo::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		lstrs.AddTail("U,dx1,0,o;U,dx2,0,o;U,dy,0,o;U,deltaY,3,o;U,thickness,0.16,o"
			//"U,dx2,0,o;U,dy2,0,o;"
			//"S,fill,false,o;U,thickness,0.3,o;"
			//"S,lineStyle,line,o"
            );
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
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

void ARDiminuendo::print() const 
{
}

bool ARDiminuendo::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;

	if ((getRange() == false ) && !strcmp("\\dimEnd",s))
		return true;
	return false;
}

void ARDiminuendo::PrintName(std::ostream & os) const
{
	if (getRange())
		os << "\\dim";
	else
		os << "\\dimBegin";
}

