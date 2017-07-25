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
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "GRDefine.h"

using namespace std;

const float MINSIZEX = (float) (5 * kCmToVirtual);	// min size = 5 x 5 cm
const float MINSIZEY = (float) (5 * kCmToVirtual);	
const float MAXSIZEX = (float) (400 * kCmToVirtual); // (JB) arbitrary max value. was (32767) (Windows 95-98 GDI limitation ?)
const float MAXSIZEY = (float) (300 * kCmToVirtual); // (JB) arbitrary max value. was (32767);

static const TagParameterMap sARPageFormatMap (kARPageFormatParams);

/** \brief Creates a page format, in Guido internal units.

	Size: width height. Margins: left top right bottom
*/
//ARPageFormat::ARPageFormat(float px, float py,
//						   float ml, float mt,
//						   float mr, float mb)
//{
//	// assert, that the given units are
//	// in INTERNAL Coordinates!
//	// parcount = 6;
//	mFormat = "";
//	mSizeX = px; // * kCmToVirtual;
//	mSizeY = py; //  * kCmToVirtual;
//	mLeft = ml; //  * kCmToVirtual;
//	mTop = mt; //  * kCmToVirtual;
//	mRight = mr; //  * kCmToVirtual;
//	mBottom = mb; //  * kCmToVirtual;
//	
//	ClipSize();
//	AdjustMargins();
//}

/** \brief Creates a page format with parameters as c-strings, in Guido internal units.

	Size: width, height. Margins: left top right bottom
*/
//ARPageFormat::ARPageFormat(char * px, char * py,
//						   char * ml, char * mt,
//						   char * mr, char * mb)
//{
//	// (JB) Shouldn't input values be in cm, then converted into virtual units ?
//	mFormat = ""; // added by jk
//
//	// this uses features of TagPArameterFloat to set the parameter ...
//	TagParameterFloat tpf(0);
//	tpf.setValue(px);
//	mSizeX = tpf.getValue();
//
//	tpf.setValue(py);
//	mSizeY = tpf.getValue();
//
//	tpf.setValue(ml);
//	mLeft = tpf.getValue();
//
//	tpf.setValue(mt);
//	mTop = tpf.getValue();
//
//	tpf.setValue(mr);
//	mRight = tpf.getValue();
//
//	tpf.setValue(mb);
//	mBottom = tpf.getValue();
//
//	ClipSize();
//	AdjustMargins();
//}

//ARPageFormat::ARPageFormat(const ARPageFormat & arp)
//{
//	(*this) = arp;
//	ClipSize();
//	AdjustMargins();
//}

void ARPageFormat::operator=(const ARPageFormat& arp)
{
	mFormat = arp.mFormat;
	mSizeX = arp.mSizeX;
	mSizeY = arp.mSizeY;
	mLeft = arp.mLeft;
	mTop = arp.mTop;
	mRight = arp.mRight;
	mBottom = arp.mBottom;
}

/** \brief Creates a page format with default parameters.

	Default size is 21 x 29.7 cm
	Default margins are 2 cm
*/
ARPageFormat::ARPageFormat()
{
	setupTagParameters (sARPageFormatMap);

	mFormat = "";
	mSizeX = DF_SX * kCmToVirtual;
	mSizeY = DF_SY * kCmToVirtual;
	mLeft = DF_ML * kCmToVirtual;
	mTop = DF_MT * kCmToVirtual;
	mRight = DF_MR * kCmToVirtual;
	mBottom = DF_MB * kCmToVirtual;
	// parcount = 6;
}

//--------------------------------------------------------------------------
void ARPageFormat::getMargins (const TagParameterMap& params)
{
	mLeft = getParameter<TagParameterFloat>(kLmStr, true)->getValue();
	mTop = getParameter<TagParameterFloat>(kTmStr, true)->getValue();
	mRight = getParameter<TagParameterFloat>(kRmStr, true)->getValue();
	mBottom = getParameter<TagParameterFloat>(kBmStr, true)->getValue();
}

//--------------------------------------------------------------------------
TagParameterMap ARPageFormat::checkTagParameters (TagParametersList& params, const std::string pTemplate)
{
	string margins = "U,lm,2cm,o;U,tm,5cm,o;U,rm,2cm,o;U,bm,3cm,o";
	string str = "U,w,,r;U,h,,r;";
	if (params.size() && params[0]->isString()) {
		str = "S,type,,r;";
		clearTagDefaultParameter(kWStr);		// this is to avoid a warning regarding required parameter
		clearTagDefaultParameter(kHStr);		// this is to avoid a warning regarding required parameter
	}
	else clearTagDefaultParameter(kTypeStr);		// this is to avoid a warning regarding required parameter

	str += margins;
	return ARMTParameter::checkTagParameters (params, str);
}

//--------------------------------------------------------------------------
void ARPageFormat::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* vtype = getParameter<TagParameterString>(kTypeStr);
	if (vtype) {		// page size is specified by type
	clearTagDefaultParameter(kTextStr);		// this is to avoid a warning regarding inherited required parameter for ARText

		mFormat = vtype->getValue();
		if (mFormat == "A4") {
			mSizeX = (float) (21.0f * kCmToVirtual);
			mSizeY = (float) (29.7f * kCmToVirtual);
		}
		else if (mFormat == "A3") {
			mSizeX = (float) (29.7f * kCmToVirtual);
			mSizeY = (float) (42.0f * kCmToVirtual);
		}
		else if (mFormat == "letter") {
			mSizeX = (float) ( 18 * kCmToVirtual );
			mSizeY = (float) ( 31 * kCmToVirtual ) ;
		}
		getMargins(params);
	}
	else {		// page size is specified by width and height
		const TagParameterFloat* vw = getParameter<TagParameterFloat>(kWStr);
		if (vw) mSizeX = vw->getValue();
		const TagParameterFloat* vh = getParameter<TagParameterFloat>(kHStr);
		if (vh) mSizeY = vh->getValue();
		getMargins(params);
	}
	ClipSize();
    AdjustMargins();
}

/** \brief Returns the size and the margins of the page, in virtual units.

	was: getMargins()
	was: getSizeAndMargins()	(was in centimers)

	\note: returns Guido internal units.
*/
void ARPageFormat::getPageFormat( float * sx, float * sy, float * ml, float * mt, float * mr, float * mb ) const
{
	*sx = (float)(mSizeX); //  * kVirtualToCm);
	*sy = (float)(mSizeY);
	*ml = (float)(mLeft);
	*mt = (float)(mTop);
	*mr = (float)(mRight);
	*mb = (float)(mBottom);
}

void ARPageFormat::setPageFormat(float sx, float sy, float ml, float mt, float mr, float mb )
{
	mSizeX = sx;
	mSizeY = sy;
	mLeft = ml;
	mTop = mt;
	mRight = mr;
	mBottom = mb;
}

void ARPageFormat::ClipSize()
{
	if (mSizeX < MINSIZEX)		mSizeX = MINSIZEX;
	else if (mSizeX > MAXSIZEX)	mSizeX = MAXSIZEX;

	if (mSizeY < MINSIZEY)		mSizeY = MINSIZEY;
	else if (mSizeY > MAXSIZEY)	mSizeY = MAXSIZEY;
}

void ARPageFormat::AdjustMargins()
{
	if ( mSizeX - mLeft - mRight <= 0.1 ) // If horizontal margins sum if higher or equal than width,
		mLeft = mRight = 0;             // we set both left and right margin to 0.

	if (mSizeY - mTop - mBottom <= 0.1 )  // Idem for vertical margins.
		mTop = mBottom = 0;
}

