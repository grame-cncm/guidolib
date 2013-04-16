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
	
	dx1 = dy1 = dx2 = dy2 = thickness = 0;
	lineStyle = NULL;

	mParSet = false;
	wavy = false;
}

ARGlissando::ARGlissando(const ARGlissando * glissando)	: ARMTParameter(-1,glissando)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	dx1 = dy1 = dx2 = dy2 = thickness = 0;
	lineStyle = NULL;

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	mParSet = false;
	wavy = glissando->wavy;

	if(glissando->dx1)
		TagParameterFloat::cast( glissando->dx1->getCopy());
	if(glissando->dy1)
		TagParameterFloat::cast( glissando->dy1->getCopy());
	if(glissando->dx2)
		TagParameterFloat::cast( glissando->dx2->getCopy());
	if(glissando->dy2)
		TagParameterFloat::cast( glissando->dy2->getCopy());
	if(glissando->thickness)
		TagParameterFloat::cast( glissando->thickness->getCopy());
	if(glissando->lineStyle)
		TagParameterString::cast( glissando->lineStyle->getCopy());
	if(glissando->color)
		TagParameterString::cast( glissando->color->getCopy());
}

ARGlissando::~ARGlissando(void)
{
	delete dx1;
	delete dy1;
	delete dx2;
	delete dy2;
	delete thickness;
	delete lineStyle;
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
			             "U,thickness,0.1,o;S,lineStyle,line,o" );
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if(color)
		cout<<"color "<<color->getValue()<<endl;
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			dx1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx1);
			if (dx1->TagIsSet())
				mParSet = true;

			cout<<"dx1 = "<<dx1->getValue()<<endl;

			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy1);
			if (dy1->TagIsSet())
				mParSet = true;
	
			cout<<"dy1 = "<<dy1->getValue()<<endl;

			dx2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx2);
			if (dx2->TagIsSet())
				mParSet = true;

			cout<<"dx2 = "<<dx2->getValue()<<endl;

			dy2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy2);
			if (dy2->TagIsSet())
				mParSet = true;

			cout<<"dy2 = "<<dy2->getValue()<<endl;


			thickness = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(thickness);
			if (thickness->TagIsSet())
				mParSet = true;

			cout<<"thickness = "<<thickness->getValue()<<endl;

			lineStyle = TagParameterString::cast(rtpl->RemoveHead());
			assert(lineStyle);
			if (lineStyle->TagIsSet()) 
				mParSet = true;

			string wavyLine ("wavy");
			if (wavyLine == lineStyle->getValue())
			{
				wavy = true;	
			}
			else
			{
				wavy = false;
			}
		}
			if(color)
		cout<<"color "<<color->getValue()<<endl;

		delete rtpl;
			if(color)
		cout<<"color "<<color->getValue()<<endl;
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
	if (thickness && thickness->TagIsSet())
	{
		tpl->AddTail(thickness->getCopy());
	}
	if (lineStyle && lineStyle->TagIsSet())
	{
		tpl->AddTail(lineStyle->getCopy());
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

