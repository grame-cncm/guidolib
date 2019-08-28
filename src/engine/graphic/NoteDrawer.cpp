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

#include "defines.h"
#include "FontManager.h"
#include "GRFlag.h"
#include "MusicalSymbols.h"
#include "NoteDrawer.h"
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

// ----------------------------------------------------------------------------
NoteDrawer::NoteDrawer(const VGFont * font, const NVPoint& pos, float align )
	: fFont (font), fPosition (pos), fYAlign(align) {}

// ----------------------------------------------------------------------------
float NoteDrawer::GetScaling (float textfontsize )
{
	return textfontsize / 90.f * 0.7f;  // 90 is the font nominal size and 0.7 is the note scaling
}

// ----------------------------------------------------------------------------
const VGFont* NoteDrawer::GetMusicFont( float scale )
{
	float musicfontsize = 200.f;
	return FontManager::FindOrCreateFont(  int(musicfontsize * scale), kMusicFontStr, "");
}

// ----------------------------------------------------------------------------
unsigned int NoteDrawer::getSymbol(const TYPE_DURATION & noteDur) const
{
	// - Choose notehead
	if (noteDur>=DURATION_1)
		return kWholeNoteHeadSymbol;
	if (noteDur == DURATION_2 || noteDur == DURATION_3_4 || noteDur == DURATION_7_8)
	 	return kHalfNoteHeadSymbol;
	return kFullHeadSymbol;
}

// ----------------------------------------------------------------------------
unsigned int NoteDrawer::getFlags(const TYPE_DURATION & noteDur) const
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
float NoteDrawer::DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const
{
	// - Choose notehead
	unsigned int symbol = getSymbol (noteDur);
	unsigned int flagSymbol = getFlags (noteDur);
	unsigned int dotSymbol = (noteDur.getNumerator() == 3) ? kNoteDotSymbol : kNoneSymbol;

	unsigned int align = hdc.GetFontAlign();
	hdc.SetFontAlign(VGDevice::kAlignLeft + VGDevice::kAlignBase);
	hdc.SetMusicFont (fFont);
	
	float w, hh;
	fFont->GetExtent( symbol, &w, &hh, &hdc );
	float xPos = xOffset + fPosition.x;
	float yPos = fYAlign + yOffset + fPosition.y - w / 2;

	// - Draw Head
	hdc.DrawMusicSymbol(xPos, yPos, symbol);
	float width = w;

	// - Draw Stem
	float		stemLen = w*2.8f;
	if (symbol != kWholeNoteHeadSymbol) {
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
	if (flagSymbol != kNoneSymbol) hdc.DrawMusicSymbol( xPos, yPos - stemLen, flagSymbol );

	// - Draw Dot
	if (dotSymbol != kNoneSymbol) {
		float space = w/3;
		hdc.DrawMusicSymbol( xPos + width + space, yPos, dotSymbol);
		width += space * 1.3f;
	}

	// - Cleanup
	hdc.SetFontAlign(align);
	return width;
}
