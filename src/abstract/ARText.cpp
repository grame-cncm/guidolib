/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <iostream>

#include "ARText.h"

#include "ARLyrics.h"
#include "GRDefine.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARText::ltpls(1);

ARText::ARText(const NVstring & p_txt, float /*p_offsety*/ )
{
	// ASSERT(FALSE);
	// dy = new TagParameterFloat(p_offsety);
	ypos = NULL;
	text = new TagParameterString(p_txt.c_str());
	textformat = NULL;
	font = NULL;
	fsize = NULL;
	fattrib = NULL;
	rangesetting = RANGEDC;
}

ARText::ARText() : ARMTParameter()	
{
	relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
	duration = DURATION_0;
	// dy = new TagParameterFloat(-6 * LSPACE/2);
	ypos = NULL;
	text = NULL;
	textformat = NULL;
	font = NULL;
	fsize = NULL;
	fattrib = NULL;
	rangesetting = RANGEDC;
}

ARText::~ARText() 
{
	delete text;
	delete textformat;
	delete font;
	delete fsize;
	delete fattrib;
	delete ypos;
};

const char * 
ARText::getTagFormat() const	 // (JB) new
{
	const char * const outFormat = "S,text,,r;U,dy,-1,o";
	return outFormat;
}

void 
ARText::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( getTagFormat());	
			//"S,text,,r;U,dy,-1,o")); 	
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			delete text;
			text = TagParameterString::cast(rtpl->RemoveHead());
			assert(text);

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

void ARText::print() const
{
}

void ARText::PrintName(std::ostream &os) const
{
	os << "\\text";
}

void ARText::PrintParameters(std::ostream &os) const
{
	if (text || mDy)
	{
		if (text)
			os << "<\"" << text->getValue();
		if (text && mDy) 
			os << "\",";
		if (mDy)
			os << mDy->getUnitValue() << mDy->getUnit();
		os << ">";
	}
}


/** \brief Grabs the added Text-Parametersif a match has been found.
*/
int ARText::MatchListOfTPLsWithTPL(const ListOfTPLs &ltpls,TagParameterList &tpl,
		TagParameterList **rtpl)
{
	int ret = ARMusicalTag::MatchListOfTPLsWithTPL(ltpls,tpl,rtpl);
	if (ret >= 0)
	{
		TagParameterList * mytpl = *rtpl;
		// then we have to grab the values at the end ...

		// the last one is fattrib ...
		fattrib = TagParameterString::cast(mytpl->RemoveTail());
		assert(fattrib);
		if (fattrib->TagIsNotSet())
		{
			delete fattrib;
			fattrib = 0;
		}

		// the size ...
		fsize = TagParameterFloat::cast(mytpl->RemoveTail());
		assert(fsize);
		// the font
		font = TagParameterString::cast(mytpl->RemoveTail());
		assert(font);

		// the textformat 
		textformat = TagParameterString::cast(mytpl->RemoveTail());
		assert(textformat);
		if (textformat->TagIsNotSet())
		{
			delete textformat;
			textformat = 0;
		}

		ypos = TagParameterFloat::cast(mytpl->RemoveTail());
		assert(ypos);
		if (ypos->TagIsNotSet())
		{
			delete ypos;
			ypos = 0;
		}
	}

	return ret;
}

/** \brief Creates the ListOfTPLs (tag parameters) if it is not already present.

	It adds the default TEXT-Parameters and then calls the default in ARMusicalTag.
*/
void ARText::CreateListOfTPLs( ListOfTPLs &ltpl,ListOfStrings & lstrs)
{	
	ListOfStrings::iterator ptr;
	for( ptr = lstrs.begin(); ptr != lstrs.end(); ++ ptr )
	{
		NVstring & mystr = *ptr;
		mystr.append(";U,ypos,0hs,o;S,textformat,cc,o;"
		"S,font,Times,o;U,fsize,12pt,o;"
		"S,fattrib,,o"); // TODO: replace "Times" ?
	}
	ARMusicalTag::CreateListOfTPLs(ltpl,lstrs);
}

void ARText::copyLyricsParams(const ARLyrics * lyrics)
{
	if (lyrics->mDx)
	{
		delete mDx;
		mDx = TagParameterFloat::cast(lyrics->mDx->getCopy());
	}
	if (lyrics->mDy)
	{
		delete mDy;
		mDy = 0;
		delete ypos;
		ypos = TagParameterFloat::cast(lyrics->mDy->getCopy());

	}
	if (lyrics->color)
	{
		delete color;
		color = TagParameterString::cast(lyrics->color->getCopy());
	}

	if (lyrics->mFAttrib)		setFAttrib(lyrics->mFAttrib);
	if (lyrics->mFont)			setFont(lyrics->mFont);
	if (lyrics->mFSize)			setFSize(lyrics->mFSize);
	if (lyrics->mTextFormat)	setTextformat(lyrics->mTextFormat);
}

void ARText::setTextformat(const TagParameterString * tf)
{
	delete textformat;
	textformat = TagParameterString::cast(tf->getCopy());
}

void ARText::setFont(const TagParameterString * ft)
{
	delete font;
	font = TagParameterString::cast(ft->getCopy());
}

void ARText::setFAttrib(const TagParameterString * fa)
{
	delete fattrib;
	fattrib = TagParameterString::cast(fa->getCopy());
}

void ARText::setFSize(const TagParameterFloat *fs)
{
	delete fsize;
	fsize = TagParameterFloat::cast(fs->getCopy());
}

int ARText::getFSize(float curLSPACE) const
{
	return fsize ? (int) fsize->getValue(curLSPACE) : 0;
}


