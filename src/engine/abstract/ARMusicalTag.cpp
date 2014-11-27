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

#include <sstream>
#include <iostream>

#include "secureio.h"
#include "kf_ivect.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterRGBColor.h"
#include "ListOfStrings.h"

#include "ARMusicalTag.h"

ARMusicalTag::ARMusicalTag(const TYPE_TIMEPOSITION & tp, const ARMusicalTag * copy)
  	: ARMusicalObject(tp), id(-1), isAuto(0), rangesetting(NO), error(0), mHasRange(false), mIsInHeader(false)
{
	assoc = DC;
	color = 0;
    rgbColor = 0;
	mDx = mDy = size = 0;
	if (copy)
	{
		if (copy->getColor())
			color = TagParameterString::cast(copy->getColor()->getCopy());
        
        if (copy->getRGBColor())
            rgbColor = TagParameterRGBColor::cast(copy->getRGBColor()->getCopy());
		
		if (copy->getDX())
			mDx = TagParameterFloat::cast(copy->getDX()->getCopy());
		
		if (copy->getDY())
			mDy = TagParameterFloat::cast(copy->getDY()->getCopy());
		
		if (copy->getSize())
			size = TagParameterFloat::cast(copy->getSize()->getCopy());

		rangesetting = copy->rangesetting;
		mHasRange = copy->mHasRange;
	}
};

ARMusicalTag::ARMusicalTag(int pid, const ARMusicalTag * copy) 
  : 
  id(pid),
  rangesetting(NO) ,
  error(0),
  mHasRange(false),
mIsInHeader(false)
{
	assoc = DC; // don't care ...
	isAuto = 0;
	color = NULL;
    rgbColor = NULL;
	mDx = mDy = size = 0;
	if (copy)
	{
		if (copy->getColor())
			color = TagParameterString::cast(
				copy->getColor()->getCopy());
        
        if (copy->getRGBColor())
            rgbColor = TagParameterRGBColor::cast(
                copy->getRGBColor()->getCopy());

		if (copy->getDX())
			mDx = TagParameterFloat::cast(
			copy->getDX()->getCopy());

		if (copy->getDY())
			mDy = TagParameterFloat::cast(
			copy->getDY()->getCopy());

		if (copy->getSize())
			size = TagParameterFloat::cast(
			copy->getSize()->getCopy());

		rangesetting = copy->rangesetting;
		mHasRange = copy->mHasRange;
	}
}


ARMusicalTag::~ARMusicalTag()
{
	delete color;
    if (rgbColor) delete rgbColor;
	delete mDx;
	delete mDy;
	delete size;
}

// (JB) rewrited with std::ostringstream class instead of deprecated ostrstream.
std::ostream & ARMusicalTag::operator <<(std::ostream & os) const
{
	PrintName(os);
	
	std::ostringstream mystr;	// was ostrstream (deprecated)	
	PrintParameters(mystr);

	size_t count = mystr.str().size();	// was mystr.pcount()
	const char * tmpp = mystr.str().c_str();

	char * cp = NULL;
	if (tmpp)
	{
		cp = new char[count+1];
		if (count) strncpy(cp,tmpp,count);
		cp[count] = 0;
	}

// (JB)	mystr.rdbuf()->freeze(0);

	bool hasmoreparams = false;
	if (cp && count > 0 )
	{		
		cp[count] = 0;

		// now we get rid of the  closing bracket.
		for( ; cp[count] != '>' ; count--) { }

		if (count > 0)
		{
			cp[count] = 0;
			hasmoreparams = true;
		}
// 		os << "<" << &(cp[1]) << "> ";
	}

	std::ostringstream mystr2;
	bool isfirst = true;
	// now we have to look at the "default"-tag-parameters ...
	if (mDx && mDx->TagIsSet())
	{
		if (!isfirst)
			mystr2 << ",";
		else
			isfirst = false;

		mystr2 << "dx=" << mDx->getUnitValue() << mDx->getUnit();
	}

	if (mDy && mDy->TagIsSet())
	{
		if (!isfirst)
			mystr2 << ",";
		else
			isfirst = false;

		mystr2 << "dy=" << mDy->getUnitValue() << mDy->getUnit();
	}

	if (color && color->TagIsSet())
	{	
		if (!isfirst)
			mystr2 << ",";
		else
			isfirst = false;

		mystr2 << "color=\"" << color->getValue() << "\"";

	}
	if (size && size->TagIsSet())
	{		
		if (!isfirst)
			mystr2 << ",";
		else
			isfirst = false;

		mystr2 << "size=" << size->getValue(); 
	}

	if (!isfirst)
	{
		const char * tmpp = mystr2.str().c_str();	// now we use ostringstream.
		if (hasmoreparams)
			os << "<" << &(cp[1]) <<  "," << tmpp << ">";
		else
			os << "<" << tmpp << ">";
	}
	else if (hasmoreparams)
	{
		os << "<" << &(cp[1]) << ">";
	}
	
	delete [] cp;

	if (id != -1)
		os << ":" << id;
	if (getRange())
		os << "(";
	return os << " ";
}

void ARMusicalTag::PrintName(std::ostream & ) const
{	
}

void ARMusicalTag::PrintParameters(std::ostream & ) const
{

}

void ARMusicalTag::setRGBColor (unsigned char red, unsigned char green, unsigned char blue)
{
    //if (rgbColor) delete rgbColor;
    rgbColor = new TagParameterRGBColor(red, green, blue);
}


void ARMusicalTag::setColor(const char * cp)
{
	delete color;
	color = new TagParameterString(cp);
}

void ARMusicalTag::setDX(TagParameterFloat * tpf)
{
	delete mDx;
	mDx = tpf;
}


void ARMusicalTag::setDY(TagParameterFloat * tpf)
{
	delete mDy;
	mDy = tpf;
}

void ARMusicalTag::setSize(float newsize)
{
	if (size)
		size->setValue(newsize);
	else
		size = new TagParameterFloat(newsize);	
}

void ARMusicalTag::setAllowRange(int pallow)
{
	if (pallow == 0)		rangesetting = NO;
	else if (pallow == 1)	rangesetting = RANGEDC;
	else if (pallow == -1)	rangesetting = ONLY;
}

bool ARMusicalTag::MatchEndTag(const char * endstr)
{
	if (getRange() && !strcmp(")",endstr))
		return true;
	return false;
}

//----------------------------------------------------------------------------------
/** \brief Creates a tag parameter list from a string.
*/
void ARMusicalTag::AddTagParametersList (ListOfTPLs & ltpl, std::string& str ) const
{
	if (str.length() > 0)
		str.append(";");
	str.append("S,color,black,o;" "U,dx,0,o;U,dy,0,o;F,size,1.0,o");

	// here, we need to create the tpl 
	TagParameterList * tpl = new TagParameterList(str);
	ltpl.AddTail(tpl);		
}

//----------------------------------------------------------------------------------
/** \brief Creates the ListOfTPLs if it is not already present.
*/
void ARMusicalTag::CreateListOfTPLs( ListOfTPLs & ltpls, ListOfStrings & lstrs)
{
	// for each string, we create a tpl ...
	ListOfStrings::iterator ptr;
	for( ptr = lstrs.begin(); ptr != lstrs.end(); ++ ptr )
	{
		AddTagParametersList (ltpls, *ptr);
	}
}

//----------------------------------------------------------------------------------
/** 
	This method is responsible for finding the
	correct TPL out of the List of TPLs that
	best matches the given TPL.
	It returns the number in the ListOfTPLs so that
	the calling function knows, which match has been made.
	If no match has been made, -1 is returned -> than
	the tagparameters can not be handled, the tag
	is ignored.
	the given TPL is adjusted, so that the
	order of elements and number of elements
	is correct (not specified optional elements
	are added and given their default value).

	works as follows: needs to check only those
	in the List, that have the correct number
	(including optional ones of course)
*/
int ARMusicalTag::MatchListOfTPLsWithTPL(const ListOfTPLs &ltpls, TagParameterList &tpl, TagParameterList **rtpl)
{
	
	KF_IVector<TagParameterList> vtpls(0);		// does not own the elements
	KF_IVector<TagParameterList> rtpls(1);		// The Resulting TagParameterLists ... (owns the results).
	GuidoPos pos = ltpls.GetHeadPosition();
	int i=0;
	while (pos)
	{
		vtpls.Set(i,ltpls.GetNext(pos));
		++i;
	}

	// now, we have a vector of tpls 
	// This is done, so that we can later tell, which one matches first/best.
	int max = vtpls.GetMaximum();
	for (i=0;i<=max;i++)
	{
		TagParameterList *tmptpl = vtpls.Get(i);
		if (tmptpl)
		{
			if (tmptpl->getMinimum() > tpl.GetCount())
				vtpls.Set(i,NULL);
			if (tmptpl->getMaximum() < tpl.GetCount())
				vtpls.Set(i,NULL);
		}
	}

	// now, we have to deal with the entries (all in parallel and parameter-wise)
	for (i=0;i<=max;i++)
	{
		TagParameterList * tmptpl1 = vtpls.Get(i);
		if (tmptpl1)
		{
			TagParameterList * tmptpl2 = tmptpl1->Match(tpl);
			if (tmptpl2)
			{
				rtpls.Set(i,tmptpl2);
				break;
			}
			else
			{
				vtpls.Set(i,NULL);
				rtpls.Set(i,NULL);
			}
		}
	}
	*rtpl = NULL;

	// now, we take the first matching one ... the rest is just discarded ...
	for (i=0;i<=max;i++)
	{
		TagParameterList *tmptpl = rtpls.Get(i);
		if (tmptpl)
		{
			// this is returned
			*rtpl = tmptpl;
			// so that it does not get deleted 
			rtpls.Set(i,NULL);
			break;
		}
	}

//	assert((*rtpl)->size());
	if (*rtpl != NULL)
	{
		// now, we have to grab the optional parameters ...
		delete size;
		size = TagParameterFloat::cast((*rtpl)->RemoveTail());
		assert(size);
		if (size->TagIsNotSet())
		{
			delete size;
			size = NULL;
		}

		delete mDy;
		mDy = TagParameterFloat::cast((*rtpl)->RemoveTail());
		assert(mDy);

		if (mDy->TagIsNotSet())
		{
			delete mDy;
			mDy = NULL;
		}	

		delete mDx;
		mDx = TagParameterFloat::cast((*rtpl)->RemoveTail());
		assert(mDx);
		if (mDx->TagIsNotSet())
		{
			delete mDx;
			mDx = NULL;
		}

		delete color;
		color = TagParameterString::cast((*rtpl)->RemoveTail());
		assert(color);
		if (color->TagIsNotSet())
		{
			delete color;
			color = NULL;
		}
		return i;
	}
	return -1;
}
