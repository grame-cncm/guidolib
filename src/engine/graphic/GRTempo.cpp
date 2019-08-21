/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>
#include "secureio.h"

#include "ARTempo.h"
#include "FontManager.h"
#include "FontManager.h"
#include "GRFlag.h"
#include "GRStaff.h"
#include "GRTempo.h"
#include "GuidoDefs.h"
#include "GUIDOInternal.h"
#include "MusicalSymbols.h"
#include "TagParameterFloat.h"
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

// ----------------------------------------------------------------------------
GRTempo::GRTempo( GRStaff * staff, const ARTempo * inAR ) : GRTagARNotationElement( inAR, LSPACE )
{
	assert(inAR);

	// Sets the Type -> This tag is a SystemTag.
	setTagType(GRTag::SYSTEMTAG);
	mPosition.y -= 2 * LSPACE; // Tempo is usually placed two linespaces above the staff.

	VGDevice * hdc = gGlobalSettings.gDevice;
	mBoundingBox.Set (0, -2*LSPACE, 0, 0);

	fFormat = inAR->getTextFormat();
	float fsize = inAR->getFSize();
	fFont = FontManager::FindOrCreateFont( fsize, inAR->getFont(), inAR->getTextAttributes());
	fNoteScale = fsize / 90.f * 0.7f;  // 90 is the font nominal size and 0.7 is the note scaling
	fYAlign = getYAlign(fsize);

	float mfontsize = 200.f;
	fMusicFont = FontManager::FindOrCreateFont(  mfontsize * fNoteScale, kMusicFontStr, "");

	for (auto l : inAR->getTempoMark()) {
		if (l.second == FormatStringParser::kSpecial) {
			TYPE_DURATION duration = inAR->getDuration(l.first.c_str());
			mBoundingBox.right += GetSymbolExtent( kFullHeadSymbol );
			if (duration.getNumerator() == 3) mBoundingBox.right += LSPACE;
		}
		else if( hdc ) {
			float w, h;
			const char * str = l.first.c_str();
			fFont->GetExtent( str, int(l.first.size()), &w, &h, hdc );
			mBoundingBox.right += w;
		}
	}
	setGRStaff(staff);
	fDate = inAR->getRelativeTimePosition();
}

// ----------------------------------------------------------------------------
float GRTempo::getXPos() const
{
	float x = getOffset().x;
	if (fDate) {
		NEPointerList * elts = getGRStaff()->getElements();
		GuidoPos pos = elts->GetHeadPosition();
		while (pos) {
			GRNotationElement * e = elts->GetNext(pos);
			if ((e->getRelativeTimePosition() >= fDate) && e->isGREvent()) {
				x += e->getPosition().x - mPosition.x;
				if (e->isGREvent()) x -= e->getBoundingBox().Width()/2;
				break;
			}
		}
	}
	return x;
}

// ----------------------------------------------------------------------------
void GRTempo::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow) return;

    const ARTempo *ar = static_cast<const ARTempo *>(mAbstractRepresentation);
    if (!ar) return;

    VGColor prevFontColor = hdc.GetFontColor();
    if (mColRef) hdc.SetFontColor(VGColor(mColRef));

	float space = getGRStaff()->getStaffLSPACE() / 2 * fNoteScale;
	float currX = getXPos();
	float dy = ar->getDY() ? - ar->getDY()->getValue(LSPACE) : 0.f;
	for (auto l : ar->getTempoMark()) {
		if (l.second == FormatStringParser::kSpecial) {
			TYPE_DURATION duration = ar->getDuration(l.first.c_str());
			currX += DrawNote( hdc, duration, currX, dy ) + space;
		}
		else {
			currX += DrawText( hdc, l.first.c_str(), currX, dy ) + space;
		}
	}
    if (mColRef) hdc.SetFontColor(prevFontColor);
}

// ----------------------------------------------------------------------------
unsigned int GRTempo::getSymbol(const TYPE_DURATION & noteDur) const
{
	// - Choose notehead
	if (noteDur>=DURATION_1)
		return kWholeNoteHeadSymbol;
	if (noteDur == DURATION_2 || noteDur == DURATION_3_4 || noteDur == DURATION_7_8)
	 	return kHalfNoteHeadSymbol;
	return kFullHeadSymbol;
}

// ----------------------------------------------------------------------------
unsigned int GRTempo::getFlags(const TYPE_DURATION & noteDur) const
{
	// - Choose flag
	if (noteDur == DURATION_8 ||  noteDur == DURATION_3_16 || noteDur == DURATION_7_32)
		return GRFlag::H8U;
	if (noteDur == DURATION_16 || noteDur == DURATION_3_32 || noteDur == DURATION_7_64)
		return GRFlag::H16U;
	if (noteDur == DURATION_32)
		return  GRFlag::H32U;
	if (noteDur == DURATION_64)
		return GRFlag::H64U;
	return kNoneSymbol;
}

// ----------------------------------------------------------------------------
/** Draws the note corresponding to a given symbolic musical duration.
*/
float GRTempo::DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const
{
	// - Choose notehead
	unsigned int theSymbol = getSymbol (noteDur);
	unsigned int theFlagSymbol = getFlags (noteDur);
	unsigned int theDotSymbol = (noteDur.getNumerator() == 3) ? kNoteDotSymbol : kNoneSymbol;

	unsigned int align = hdc.GetFontAlign();
	hdc.SetFontAlign(VGDevice::kAlignLeft + VGDevice::kAlignBase);
	hdc.SetMusicFont (fMusicFont);
	float w = GetSymbolExtent(theSymbol);
	float xPos = xOffset + mPosition.x;
	float yPos = fYAlign + yOffset + mPosition.y - w/3.f;

	// - Draw Head
	hdc.DrawMusicSymbol(xPos, yPos, theSymbol);
	float width = w * fNoteScale;

	// - Draw Stem
	if (theSymbol != kWholeNoteHeadSymbol) {
		float		stemLen = 3 * LSPACE * fNoteScale;
		float		stemTagSize = 1;

		const float stemCharSize = LSPACE * stemTagSize;
		const float halfStemCharSize = 0.5f * stemCharSize;
		hdc.DrawMusicSymbol( xPos, yPos, kStemUp1Symbol );

		// - Draws until the length has been completed ...
		float offsy = -halfStemCharSize;
		while( -offsy < stemLen ) {
			if(( stemCharSize - offsy ) > stemLen ) {
				offsy =  (-(stemLen) + stemCharSize );
				hdc.DrawMusicSymbol( xPos, yPos + offsy, kStemUp2Symbol );
				break;
			}
			hdc.DrawMusicSymbol( xPos, yPos + offsy, kStemUp2Symbol );
			offsy -= halfStemCharSize;
		}
	}

	// - Draw flag
	if (theFlagSymbol != kNoneSymbol) hdc.DrawMusicSymbol( xPos, yPos - 3.5 * LSPACE * fNoteScale, theFlagSymbol );

	// - Draw Dot
	if (theDotSymbol != kNoneSymbol) {
		float space = LSPACE * fNoteScale * 0.5;
		hdc.DrawMusicSymbol( xPos + width + space, yPos, theDotSymbol);
		width += space * 1.3;
	}

	// - Cleanup
	hdc.SetFontAlign(align);
	return width;
}

// ----------------------------------------------------------------------------
float GRTempo::DrawText( VGDevice & hdc, const char * cp, float xOffset, float yOffset) const
{
	hdc.SetTextFont( fFont );
    hdc.SetFontAlign (getTextAlign());
	hdc.DrawString ( xOffset + mPosition.x, yOffset + mPosition.y, cp, (int)strlen(cp) );

	float h, width;
	fFont->GetExtent(cp, (int)strlen(cp), &width, &h, &hdc);
	return width;
}

// ----------------------------------------------------------------------------
unsigned int GRTempo::getTextAlign() const
{
	unsigned int xdir = VGDevice::kAlignLeft;
	unsigned int ydir = VGDevice::kAlignBase;
	if (fFormat.size() == 2) {
//		switch (fFormat[0]) {		// horizontal alignment ignored - to do
//			case 'l':	xdir = VGDevice::kAlignLeft; break;
//			case 'c':	xdir = VGDevice::kAlignCenter; break;
//			case 'r':	xdir = VGDevice::kAlignRight; break;
//		}

		switch (fFormat[1]) {
			case 't':	ydir = VGDevice::kAlignTop; break;
			case 'c':	ydir = VGDevice::kAlignBase; break;
			case 'b':	ydir = VGDevice::kAlignBottom; break;
		}
	}
	return xdir | ydir;
}

// ----------------------------------------------------------------------------
float GRTempo::getYAlign(float fsize) const
{
	if (fFormat.size() == 2) {
		switch (fFormat[1]) {
			case 't':	return fsize * 0.8f;
			case 'c':	return 0.f;
			case 'b':	return -fsize/3.f;
		}
	}
	return 0.f;
}
