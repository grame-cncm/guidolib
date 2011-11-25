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

#include "GRPageText.h"
#include "GRStaff.h"
#include "GRPage.h"
#include "GRDefine.h"
#include "GRVoice.h"

#include "GUIDOInternal.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "FontManager.h"


GRPageText::GRPageText ( ARMusicalTag * o, GRPage * p_grpage,
				 const char* p_txt, const char* p_pageformat,
				 const char* p_textformat, const char* p_textfont,
				 int p_textsize, const char* p_textattrib)
				 
  : GRTagARNotationElement(o, LSPACE)
{
	setTagType(GRTag::PAGETAG);

	if (p_grpage) fGRPage = p_grpage;
	if (p_txt) fPageText = p_txt ;
	if (p_pageformat)fPageformat = p_pageformat;
	if (p_textformat) 	
		fTextformat =  p_textformat;
	else
		fTextformat = "lt";
	
	if (p_textfont)
		font = new NVstring(p_textfont);

	if (p_textattrib)
		fontAttrib = new NVstring(p_textattrib);

	mFontSize = p_textsize;

	// now, we have to get a font ...
	// this is plattform-dependant code ...
	// this was done in special?

	const VGFont* myfont = FontManager::gFontText;

	if (font && font->length() > 0)
	{
		// here, size and attributes must be regarded!!!!!
		myfont = FontManager::FindOrCreateFont(mFontSize, font, fontAttrib);
	}

	// depending on the textformat ...
	unsigned int xdir = VGDevice::kAlignLeft;
	unsigned int ydir = VGDevice::kAlignTop;
	if( fTextformat.size() == 2 )
	{
		switch (fTextformat[0]) {
			case 'l':	xdir = VGDevice::kAlignLeft; break;
			case 'c':	xdir = VGDevice::kAlignCenter; break;
			case 'r':	xdir = VGDevice::kAlignRight; break;
		}	  
		switch (fTextformat[1]) {
			case 't':	ydir = VGDevice::kAlignTop; break;
			case 'c':	ydir = VGDevice::kAlignBase; break;
			case 'b':	ydir = VGDevice::kAlignBottom; break;
		}
	}

    fTextalign = (xdir | ydir);

	const char * cp = fPageText.c_str();

	float width = 0;
	float height = 0;
	if( gGlobalSettings.gDevice )	
		myfont->GetExtent( cp, (int)fPageText.size(), &width, &height, gGlobalSettings.gDevice );
	
	// left and right Space is not
	// useful -> these are PageTexts

	if (xdir == VGDevice::kAlignLeft)
	{
		mBoundingBox.right = (GCoord)width;
		mBoundingBox.left = 0;
	}
	else if (xdir == VGDevice::kAlignRight)
	{
		mBoundingBox.left = (GCoord)(-width);
		mBoundingBox.right = 0;
	}
	else if (xdir == VGDevice::kAlignCenter)
	{
		mBoundingBox.left = (GCoord)(-width * 0.5f);
		mBoundingBox.right = (GCoord)(width * 0.5f);
	}

	if (ydir == VGDevice::kAlignTop)
	{
		mBoundingBox.top = 0;
		mBoundingBox.bottom = (GCoord)height;
	}
	else if (ydir == VGDevice::kAlignBase)
	{
		mBoundingBox.top = (GCoord)(-height);
		mBoundingBox.bottom = (GCoord)(2 * LSPACE);
	}
	else if (ydir == VGDevice::kAlignBottom)
	{
		mBoundingBox.top = (GCoord)(-height);
		mBoundingBox.bottom = 0;
	}
}

GRPageText::~GRPageText()
{
}

void GRPageText::OnDraw(VGDevice & hdc) const
{
	GRTagARNotationElement::OnDrawText(hdc, fPageText.c_str(), (int)fPageText.size());
}

void GRPageText::print() const
{
}

/** \brief Calculates the position of
	the text using the grpage and the format tag.
*/
void GRPageText::calcPosition()
{
	if (fGRPage == 0) return;

	// now we have the grpage and we have the
	// Format string.

	if (fPageformat.size() != 2) return;
	char first  = fPageformat[0];
	char second = fPageformat[1];

	float ml;
	float mt;
	float mr;
	float mb;

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


