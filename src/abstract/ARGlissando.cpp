/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

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

	mParSet = false;
	fWavy = false;
}

ARGlissando::ARGlissando(const ARGlissando * glissando)	: ARMTParameter(-1,glissando)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	dx1 = dy1 = dx2 = dy2 = fThickness = 0;
	fLineStyle = NULL;

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	mParSet = false;
	fWavy = glissando->fWavy;

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

void ARGlissando::print() const
{
}


void ARGlissando::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		lstrs.AddTail( "U,dx1,0,o;U,dy1,0,o;"
			             "U,dx2,0,o;U,dy2,0,o;"
			             "U,thickness,0.3,o;S,lineStyle,line,o" );
		
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
			if (dx1->TagIsSet())
				mParSet = true;

			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy1);
			if (dy1->TagIsSet())
				mParSet = true;

			dx2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx2);
			if (dx2->TagIsSet())
				mParSet = true;

			dy2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy2);
			if (dy2->TagIsSet())
				mParSet = true;


			fThickness = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(fThickness);
			if (fThickness->TagIsSet())
				mParSet = true;

			fLineStyle = TagParameterString::cast(rtpl->RemoveHead());
			assert(fLineStyle);
			if (fLineStyle->TagIsSet()) 
				mParSet = true;

			string wavyLine ("wavy");
			if (wavyLine == fLineStyle->getValue())
			{
				fWavy = true;	
			}
			else
			{
				fWavy = false;
			}
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
	{
		tpl->AddTail(dx1->getCopy());
	}
	if (dy1 && dy1->TagIsSet())
	{
		tpl->AddTail(dy1->getCopy());
	}
	if (dx2 && dx2->TagIsSet())
	{
		tpl->AddTail(dx2->getCopy());
	}
	if (dy2 && dy2->TagIsSet())
	{
		tpl->AddTail(dy2->getCopy());
	}
	if (fThickness && fThickness->TagIsSet())
	{
		tpl->AddTail(fThickness->getCopy());
	}
	if (fLineStyle && fLineStyle->TagIsSet())
	{
		tpl->AddTail(fLineStyle->getCopy());
	}
	if (color && color->TagIsSet())
	{
		tpl->AddTail(color->getCopy());
	}

	return tpl;
}
	

void ARGlissando::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kGlissando);
}


bool ARGlissando::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\GlissandoEnd",s))
		return 1;
	return 0;
}

