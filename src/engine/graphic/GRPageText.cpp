/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARText.h"
#include "GRPageText.h"
#include "GRStaff.h"
#include "GRPage.h"
#include "GRDefine.h"
#include "GRVoice.h"

#include "GUIDOInternal.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "FontManager.h"


GRPageText::GRPageText ( const ARText * ar, GRPage * page, const char* txt, const char* location)
  : GRTagARNotationElement(ar, LSPACE)
{
	setTagType(GRTag::PAGETAG);

	fGRPage = page;
	if (txt)		fPageText = txt ;
	if (format)		fLocation = location;

	fFontName   = ar->getFont();
	fFontAttrib = ar->getTextAttributes();
	fFontSize   = ar->getFSize();

	fTextalign = VGDevice::kAlignLeft + VGDevice::kAlignTop;
	const VGFont* myfont = FontManager::GetTextFont(ar, LSPACE, fTextalign);

	const char * cp = fPageText.c_str();
	float width = 0;
	float height = 0;
	if( gGlobalSettings.gDevice && fPageText.size())	
		myfont->GetExtent( cp, (int)fPageText.size(), &width, &height, gGlobalSettings.gDevice );
	
	if (fTextalign & VGDevice::kAlignLeft) {
		mBoundingBox.right = width;
		mBoundingBox.left = 0;
	}
	else if (fTextalign & VGDevice::kAlignRight) {
		mBoundingBox.left  = -width;
		mBoundingBox.right = 0;
	}
	else if (fTextalign & VGDevice::kAlignCenter) {
		mBoundingBox.left  = -width * 0.5f;
		mBoundingBox.right = width * 0.5f;
	}

	if (fTextalign & VGDevice::kAlignTop) {
		mBoundingBox.top    = 0;
		mBoundingBox.bottom = height;
	}
	else if (fTextalign & VGDevice::kAlignBase) {
		mBoundingBox.top    = -height;
		mBoundingBox.bottom = 2 * LSPACE;
	}
	else if (fTextalign & VGDevice::kAlignBottom) {
		mBoundingBox.top    = -height;
		mBoundingBox.bottom = 0;
	}
}

//-------------------------------------------------------------------------------
void GRPageText::OnDraw(VGDevice & hdc) const
{
	if (fPageText.size()) GRTagARNotationElement::OnDrawText(hdc, fPageText.c_str(), (int)fPageText.size());
}

//-------------------------------------------------------------------------------
/** \brief Calculates the position of
	the text using the grpage and the format tag.
*/
void GRPageText::calcPosition()
{
	if (fGRPage == 0) return;

	// now we have the grpage and we have the Format string.

	if (fLocation.size() != 2) return;
	char first  = fLocation[0];
	char second = fLocation[1];

	float ml, mt, mr, mb;
	fGRPage->getMarginsCm(&ml, &mt, &mr, &mb);

	const float pageWidth = fGRPage->getPageWidth();
	const float pageHeight = fGRPage->getPageHeight();

	if (first == 'l' || first == '1')		mPosition.x = 0;
	else if (first == '2')					mPosition.x = ((ml * kCmToVirtual) * 0.5f);
	else if (first == '3')					mPosition.x = (ml * kCmToVirtual);
	else if (first == 'c' || first == '4')	mPosition.x = (pageWidth * 0.5f);
	else if (first == '5')					mPosition.x = (pageWidth - (mr * kCmToVirtual));
	else if (first == '6')					mPosition.x = (pageWidth - ((mr * kCmToVirtual) * 0.5f));
	else if (first == 'r' || first == '7')	mPosition.x = (pageWidth);

	if (second == 't' || second == '1')		mPosition.y = 0;
	else if (second == '2')					mPosition.y = ((mt * kCmToVirtual) * 0.5f);
	else if (second == '3')					mPosition.y = (mt * kCmToVirtual);
	else if (second == 'c' || second == '4')mPosition.y = (pageHeight * 0.5f);
	else if (second == '5')					mPosition.y = (pageHeight -(mb * kCmToVirtual));
	else if (second == '6')					mPosition.y = (pageHeight - ((mb * kCmToVirtual) * 0.5f));
	else if (second == 'b' || second == '7')mPosition.y = (pageHeight);
}


