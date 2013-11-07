/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>

#include "ARPageFormat.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "GRDefine.h"
#include "ListOfStrings.h"

const float MINSIZEX = (float) (5 * kCmToVirtual);	// min size = 5 x 5 cm
const float MINSIZEY = (float) (5 * kCmToVirtual);	
const float MAXSIZEX = (float) (400 * kCmToVirtual); // (JB) arbitrary max value. was (32767) (Windows 95-98 GDI limitation ?)
const float MAXSIZEY = (float) (300 * kCmToVirtual); // (JB) arbitrary max value. was (32767);

ListOfTPLs ARPageFormat::ltpls(1);

/** \brief Creates a page format, in Guido internal units.

	Size: width height. Margins: left top right bottom
*/
ARPageFormat::ARPageFormat(float px, float py,
						   float ml, float mt,
						   float mr, float mb)
{
	// assert, that the given units are
	// in INTERNAL Coordinates!
	// parcount = 6;
	mFormat = "";
	mSizeX = px; // * kCmToVirtual;
	mSizeY = py; //  * kCmToVirtual;
	mLeft = ml; //  * kCmToVirtual;
	mTop = mt; //  * kCmToVirtual;
	mRight = mr; //  * kCmToVirtual;
	mBottom = mb; //  * kCmToVirtual;
	
	ClipSize();
	AdjustMargins();
}

/** \brief Creates a page format with parameters as c-strings, in Guido internal units.

	Size: width, height. Margins: left top right bottom
*/
ARPageFormat::ARPageFormat(char * px, char * py,
						   char * ml, char * mt,
						   char * mr, char * mb)
{
	// (JB) Shouldn't input values be in cm, then converted into virtual units ?

	mFormat = ""; // added by jk

	// this uses features of TagPArameterFloat to set the parameter ...

	TagParameterFloat tpf(0);
	tpf.setValue(px);
	mSizeX = tpf.getValue();

	tpf.setValue(py);
	mSizeY = tpf.getValue();

	tpf.setValue(ml);
	mLeft = tpf.getValue();

	tpf.setValue(mt);
	mTop = tpf.getValue();

	tpf.setValue(mr);
	mRight = tpf.getValue();

	tpf.setValue(mb);
	mBottom = tpf.getValue();

	ClipSize();
	AdjustMargins();
}

ARPageFormat::ARPageFormat(const ARPageFormat & arp)
{
	// parcount = arp.parcount;
	mFormat = arp.mFormat;
	mSizeX = arp.mSizeX;
	mSizeY = arp.mSizeY;
	mLeft = arp.mLeft;
	mTop = arp.mTop;
	mRight = arp.mRight;
	mBottom = arp.mBottom;

	ClipSize();
	AdjustMargins();
}

/** \brief Creates a page format with default parameters.

	Default size is 21 x 29.7 cm
	Default margins are 2 cm
*/
ARPageFormat::ARPageFormat()
{
	mFormat = "";
	mSizeX = DF_SX * kCmToVirtual;
	mSizeY = DF_SY * kCmToVirtual;
	mLeft = DF_ML * kCmToVirtual;
	mTop = DF_MT * kCmToVirtual;
	mRight = DF_MR * kCmToVirtual;
	mBottom = DF_MB * kCmToVirtual;
	// parcount = 6;
}

ARPageFormat::~ARPageFormat()
{

}

/** \brief Setup the page size and margins according to input tags.
*/
void ARPageFormat::setTagParameterList(	TagParameterList & tpl )
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("U,w,,r;U,h,,r;U,lm,2cm,o;"
			"U,tm,5cm,o;U,rm,2cm,o;U,bm,3cm,o");
		
		lstrs.AddTail("S,type,,r;U,lm,2cm,o;"
			"U,tm,5cm,o;U,rm,2cm,o;U,bm,3cm,o");
	
		CreateListOfTPLs(ltpls, lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);

	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// Then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);			
			mSizeX = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);
			mSizeY = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);			
			mLeft = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);			
			mTop = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);			
			mRight = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);			
			mBottom = tpf->getValue();

			mFormat = "";
		}
		else if (ret==1)
		{
			// it matches the second parameter list
			// pagesize, ml, mt, mr, mb

			TagParameterString * tps = TagParameterString::cast(rtpl->RemoveHead());
			assert(tps);
			mFormat = tps->getValue();

			if (!strcmp(mFormat.c_str(),"A4")) 
			{
				mSizeX = (float) (21.0f * kCmToVirtual);
				mSizeY = (float) (29.7f * kCmToVirtual);
			}
			else if (!strcmp(mFormat.c_str(),"A3"))
			{
				mSizeX = (float) (29.7f * kCmToVirtual);
				mSizeY = (float) (42.0f * kCmToVirtual);
			}
			else if (!strcmp(mFormat.c_str(),"letter"))
			{
				// this is WRONG
				mSizeX = (float) ( 18 * kCmToVirtual );
				mSizeY = (float) ( 31 * kCmToVirtual ) ;
			}

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);
			mLeft = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);
			mTop = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);
			mRight = tpf->getValue();

			tpf = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(tpf);
			mBottom = tpf->getValue();
		}

		// this removes all the elements ...
		delete rtpl;
	}
	else
	{
		// failure ..
	}

	ClipSize();

	// - (JB) Should be replaced by AdjustMargins() ? -> 
	if ( mSizeX - mLeft - mRight < MINSIZEX )
	{
		mLeft = 0;
		mRight = 0;
	}

	if (mSizeY - mTop - mBottom < MINSIZEY )
	{
		mTop = 0;
		mBottom = 0;
	}
	// <-

	tpl.RemoveAll();

	return;
}

/** \brief Returns the size and the margins of the page, in virtual units.

	was: getMargins()
	was: getSizeAndMargins()	(was in centimers)

	\note: returns Guido internal units.
*/
void ARPageFormat::getPageFormat( float * sx, float * sy, float * ml, float * mt, 
															float * mr, float * mb )
{
	*sx = (float)(mSizeX); //  * kVirtualToCm);
	*sy = (float)(mSizeY);
	*ml = (float)(mLeft);
	*mt = (float)(mTop);
	*mr = (float)(mRight);
	*mb = (float)(mBottom);
}

void	
ARPageFormat::ClipSize()
{
	if (mSizeX < MINSIZEX)		mSizeX = MINSIZEX;
	else if (mSizeX > MAXSIZEX)	mSizeX = MAXSIZEX;

	if (mSizeY < MINSIZEY)		mSizeY = MINSIZEY;
	else if (mSizeY > MAXSIZEY)	mSizeY = MAXSIZEY;
}

void	
ARPageFormat::AdjustMargins()
{
	if ( mSizeX - mLeft - mRight < MINSIZEX )
	{
		// changed by jk
		// mLeft = mRight = 0;
		mLeft = mRight = (mSizeX - MINSIZEX) * 0.5f;
	}
	if (mSizeY - mTop - mBottom < MINSIZEY )
	{
		// changed by jk
		// mTop = mBottom = 0;
		mTop = mBottom = (mSizeY - MINSIZEY) * 0.5f;
	}
}
