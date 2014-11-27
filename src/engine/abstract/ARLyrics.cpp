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
#include "ARLyrics.h"
#include "GRDefine.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"

ListOfTPLs ARLyrics::ltpls(1);

ARLyrics::ARLyrics() : ARMTParameter()
{
	relativeTimePosition.set(-1,1);
	setDuration ( DURATION_0 );
	// dy = new TagParameterFloat(-6 * LSPACE/2);
	mText = NULL;
	mTextFormat = NULL;
	mFont = NULL;
	mFSize = NULL;
	mFAttrib = NULL;
	rangesetting = ONLY;
}

ARLyrics::~ARLyrics() 
{
	delete mText;
//	delete dy;	dy = 0; // done by base class.

	delete mTextFormat;
	delete mFont;
	delete mFSize;
	delete mFAttrib;
};

void ARLyrics::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,text,,r;U,dy,-3,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb
			// GuidoPos pos = rtpl->GetHeadPosition();

			delete mText;
			mText = TagParameterString::cast(rtpl->RemoveHead());
			assert(mText);

			delete mDy;
			mDy = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(mDy); 
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARLyrics::print(int &indent) const
{
}

void ARLyrics::PrintName(std::ostream &os) const
{
	os << "\\text";
}
void ARLyrics::PrintParameters(std::ostream &os) const
{
	if (mText || mDy)
	{
		// what about dy ?
		if (mText)
			os << "<\"" << mText->getValue() << "\"";

		os << ">";
	}
}


/** \brief Grabs the added Text-Parameters if a match has been found.
*/
int ARLyrics::MatchListOfTPLsWithTPL(const ListOfTPLs &ltpls,TagParameterList & tpl,
		TagParameterList ** rtpl)
{
	int ret = ARMusicalTag::MatchListOfTPLsWithTPL(ltpls,tpl,rtpl);
	if (ret >= 0)
	{
		TagParameterList * mytpl = * rtpl;
		// then we have to grab the values at the end ...

		// the last one is fattrib ...
		mFAttrib = TagParameterString::cast(mytpl->RemoveTail());
		assert(mFAttrib);
		if (mFAttrib->TagIsNotSet())
		{
			delete mFAttrib;
			mFAttrib = NULL;
		}

		// the size ...
		mFSize = TagParameterFloat::cast(mytpl->RemoveTail());
		assert(mFSize);
		// the font
		mFont = TagParameterString::cast(mytpl->RemoveTail());
		assert(mFont);

		// the textformat 
		mTextFormat = TagParameterString::cast(mytpl->RemoveTail());
		assert(mTextFormat);
	}

	return ret;
}

/** \brief Creates the ListOfTPLs if it is not already present.
	It adds the default TEXT-Parameters and then calls the default in ARMusicalTag.
*/
void ARLyrics::CreateListOfTPLs( ListOfTPLs & ltpl, ListOfStrings & lstrs)
{
/*	GuidoPos pos = lstrs.GetHeadPosition();
	while (pos)
	{
		NVstring * mystr = lstrs.GetNext(pos);

		if (mystr)
		{
			mystr->append(";S,textformat,ct,o;"
			"S,font,Times,o;U,fsize,12pt,o;"
			"S,fattrib,,o");	// TODO: replace "Times"
								// by the current text font.
		}
	}
*/
	// for each string, we create a tpl ...
	ListOfStrings::iterator ptr;
	for( ptr = lstrs.begin(); ptr != lstrs.end(); ++ptr )
	{
		NVstring & mystr = *ptr;
		mystr.append(";S,textformat,ct,o;"
			"S,font,Times,o;U,fsize,12pt,o;"
			"S,fattrib,,o");	// TODO: replace "Times"
								// by the current text font.
	}

	ARMusicalTag::CreateListOfTPLs(ltpl,lstrs);
}

const char* ARLyrics::getText() const
{ 
	return mText ? mText->getValue() : NULL;
}

const char* ARLyrics::getTextformat() const
{
	return mTextFormat ? mTextFormat->getValue() : 0;
}

const char* ARLyrics::getFont() const
{
	return mFont ? mFont->getValue() : 0;
}

const char* ARLyrics::getFAttrib() const
{
	return mFAttrib ? mFAttrib->getValue() : 0;
}

int ARLyrics::getFSize(float curLSPACE) const
{
	return mFSize ? (int) mFSize->getValue(curLSPACE) : 0;
}

