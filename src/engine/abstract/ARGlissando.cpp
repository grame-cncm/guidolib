/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>

#include "ARGlissando.h"

#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TimeUnwrap.h"

using namespace std;

ListOfTPLs ARGlissando::ltpls(1);

ARGlissando::ARGlissando()
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	fDx1       = 0;
    fDy1       = 0;
    fDx2       = 0;
    fDy2       = 0;
    fThickness = 0;
    fFill      = false;
	//fWavy      = false;
}

ARGlissando::ARGlissando(const ARGlissando * glissando)	: ARMTParameter(-1, glissando)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
    fDx1       = glissando->fDx1;
    fDy1       = glissando->fDy1;
    fDx2       = glissando->fDx2;
    fDy2       = glissando->fDy2;
    fThickness = glissando->fThickness;
    fFill      = glissando->fFill;
	//fWavy      = glissando->fWavy;

//    if (glissando->color)
//        color = TagParameterString::cast(glissando->color->getCopy());

    // dx/dy ?
}

void ARGlissando::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		lstrs.AddTail( "U,dx1,0,o;U,dy1,0,o;"
			"U,dx2,0,o;U,dy2,0,o;"
			"S,fill,false,o;U,thickness,0.3,o"
			//";S,lineStyle,line,o"
            );
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
            fDx1 = f->getValue();
            delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
            fDy1 = f->getValue();
            delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
            fDx2 = f->getValue();
            delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
            fDy2 = f->getValue();
            delete f;

            TagParameterString *s = TagParameterString::cast(rtpl->RemoveHead());
            fFill = s->getBool();
            delete s;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
            fThickness = f->getValue();
            delete f;

			/*s = TagParameterString::cast(rtpl->RemoveHead());
			string wavyLine("wavy");
			if (wavyLine == s->getValue())
				fWavy = true;*/
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();

	return;
}

void ARGlissando::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kGlissando);
}

bool ARGlissando::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\glissandoEnd",s))
		return 1;

	return 0;
}

void ARGlissando::printName(std::ostream& os) const
{
    os << "ARGlissando";
}

void ARGlissando::printGMNName(std::ostream& os) const
{
    os << "\\glissando";
}

void ARGlissando::printParameters(std::ostream& os) const
{
    os << "dx1: "       << fDx1       << "; ";
    os << "dy1: "       << fDy1       << "; ";
    os << "dx2: "       << fDx2       << "; ";
    os << "dy2: "       << fDy2       << "; ";
    os << "fill: "      << fFill      << "; ";
    os << "thickness: " << fThickness << "; ";

    ARMusicalTag::printParameters(os);
}
