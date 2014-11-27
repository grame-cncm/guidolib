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
	
	dx1 = dy1 = dx2 = dy2 = fThickness = 0;
	fLineStyle = NULL;

	mWavy   = false;
}

ARGlissando::ARGlissando(const ARGlissando * glissando)	: ARMTParameter(-1, glissando)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	dx1 = dy1 = dx2 = dy2 = fThickness = 0;
	fLineStyle = NULL;

	if(glissando->dx1)
		TagParameterFloat::cast( glissando->dx1->getCopy());
	if(glissando->dy1)
		TagParameterFloat::cast( glissando->dy1->getCopy());
	if(glissando->dx2)
		TagParameterFloat::cast( glissando->dx2->getCopy());
	if(glissando->dy2)
		TagParameterFloat::cast( glissando->dy2->getCopy());
	if(glissando->fThickness)
		TagParameterFloat::cast( glissando->fThickness->getCopy());
	if(glissando->fLineStyle)
		TagParameterString::cast( glissando->fLineStyle->getCopy());
	if(glissando->color)
		TagParameterString::cast( glissando->color->getCopy());
}

ARGlissando::~ARGlissando(void)
{
	delete dx1;
	delete dy1;
	delete dx2;
	delete dy2;
	delete fThickness;
	delete fLineStyle;
}

void ARGlissando::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		lstrs.AddTail( "U,dx1,0,o;U,dy1,0,o;"
			"U,dx2,0,o;U,dy2,0,o;"
			"S,fill,false,o;U,thickness,0.3,o;"
			"S,lineStyle,line,o");
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			dx1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx1);

			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy1);

			dx2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx2);

			dy2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy2);

            fill = TagParameterString::cast(rtpl->RemoveHead());
			assert(fill);

			fThickness = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(fThickness);

			fLineStyle = TagParameterString::cast(rtpl->RemoveHead());
			assert(fLineStyle);
			string wavyLine ("wavy");
			if (wavyLine == fLineStyle->getValue())
				mWavy = true;
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

TagParameterList * ARGlissando::getTagParameterList() const
{
	TagParameterList * tpl = new TagParameterList(1);

	if (dx1 && dx1->TagIsSet())
		tpl->AddTail(dx1->getCopy());

	if (dy1 && dy1->TagIsSet())
		tpl->AddTail(dy1->getCopy());

	if (dx2 && dx2->TagIsSet())
		tpl->AddTail(dx2->getCopy());

	if (dy2 && dy2->TagIsSet())
		tpl->AddTail(dy2->getCopy());

	if (fThickness && fThickness->TagIsSet())
		tpl->AddTail(fThickness->getCopy());

	if (fLineStyle && fLineStyle->TagIsSet())
		tpl->AddTail(fLineStyle->getCopy());

	if (color && color->TagIsSet())
		tpl->AddTail(color->getCopy());

    if (fill && fill->TagIsSet())
        tpl->AddTail(fill->getCopy());

	return tpl;
}

bool ARGlissando::isFill() {
    bool isFill = false;
    
    if (fill && fill->TagIsSet()) {
        if (!strcmp(fill->getValue(), "true"))
            isFill = true;
    }

    return isFill;
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
    if (dx1)
        os << "dx1: " << dx1->getValue() << "; ";

    if (dy1)
        os << "dy1: " << dy1->getValue() << "; ";

    if (dx2)
        os << "dx2: " << dx2->getValue() << "; ";

    if (dy2)
        os << "dy2: " << dy2->getValue() << "; ";

    if (fill)
        os << "fill: " << fill->getValue() << "; ";

    if (fThickness)
        os << "thickness: " << fThickness->getValue() << "; ";

    if (fLineStyle)
        os << "lineStyle: " << fLineStyle->getValue() << ";";

    ARMusicalTag::printParameters(os);
}