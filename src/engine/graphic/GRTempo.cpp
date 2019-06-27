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

const VGFont* GRTempo::mFont = 0;

// ----------------------------------------------------------------------------
GRTempo::GRTempo( GRStaff * staff, const ARTempo * inAR )
					: GRTagARNotationElement( inAR, LSPACE )
{
	assert(inAR);

	// Sets the Type -> This tag is a SystemTag.
	setTagType(GRTag::SYSTEMTAG);
	mPosition.y -= 2 * LSPACE; // Tempo is usually placed two linespaces above the staff.

	VGDevice * hdc = gGlobalSettings.gDevice;
	mBoundingBox.Set (0, -2*LSPACE, 0, 0);

	if (!mFont) {
		const NVstring fontname("Times New Roman");
		NVstring attrs ("");
		mFont = FontManager::FindOrCreateFont( 90, &fontname, &attrs);
	}

	if (inAR) {
		FormatStringParserResult::const_iterator assoc;
		for (assoc = inAR->getTempoMark().begin(); assoc != inAR->getTempoMark().end(); assoc++) {
			if (assoc->second == FormatStringParser::kSpecial) {
				TYPE_DURATION duration = getDuration(assoc->first.c_str());
				mBoundingBox.right += GetSymbolExtent( kFullHeadSymbol );
				if (duration.getNumerator() == 3) {
					mBoundingBox.right += LSPACE;
				}
			}
			else if( hdc ) {
				float w, h;
				const char * str = assoc->first.c_str();
				mFont->GetExtent( str, int(assoc->first.size()), &w, &h, hdc );
				mBoundingBox.right += w;
			}
		}
	}
	setGRStaff(staff);
	fDate = inAR->getRelativeTimePosition();
}

// ----------------------------------------------------------------------------
GRTempo::~GRTempo()
{
}

// ----------------------------------------------------------------------------
TYPE_DURATION GRTempo::getDuration (const char * str) const
{
	int num, denom;
	TYPE_DURATION duration;
	if( sscanf( str,"%d/%d", &num, &denom ) == 2 ) {
		duration.set ( num, denom );
		duration.normalize();
	}
	return duration;
}

// ----------------------------------------------------------------------------
void GRTempo::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow) return;
	
    const ARTempo *ar = static_cast<const ARTempo *>(mAbstractRepresentation);
    if (!ar) return;

    VGColor prevFontColor = hdc.GetFontColor();
    if (mColRef) hdc.SetFontColor(VGColor(mColRef));

	const float noteOffsetY = 0; // LSPACE * 1.85f;
	float currX = getOffset().x;

	if (fDate) {
		NEPointerList * elts = getGRStaff()->getElements();
		GuidoPos pos = elts->GetHeadPosition();
		while (pos) {
			GRNotationElement * e = elts->GetNext(pos);
			if (e->getRelativeTimePosition() >= fDate) {
				currX += e->getPosition().x - mPosition.x;
				if (e->isGREvent()) currX -= e->getBoundingBox().Width()/2;
				break;
			}
		}
	}

	float dy = 0;
	if (ar->getDY())
		dy = - ar->getDY()->getValue(LSPACE);

	FormatStringParserResult::const_iterator assoc;
	for (assoc = ar->getTempoMark().begin(); assoc != ar->getTempoMark().end(); assoc++) {
		if (assoc->second == FormatStringParser::kSpecial) {
			TYPE_DURATION duration = getDuration(assoc->first.c_str());
			currX += DrawNote( hdc, duration, currX + LSPACE, noteOffsetY + dy ) + LSPACE;
		}
		else {
			float textwidth;
			DrawText( hdc, assoc->first.c_str(), currX, dy, &textwidth );
			currX += textwidth;
		}
	}

    if (mColRef) hdc.SetFontColor(prevFontColor);
}

// ----------------------------------------------------------------------------
/** Draws the note corresponding to a given symbolic musical duration.
*/
float GRTempo::DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const
{
	float offsetX = 0;

	// - Choose notehead
	unsigned int theSymbol = kNoneSymbol;
	if (noteDur>=DURATION_1) {
		theSymbol = kWholeNoteHeadSymbol;
	}
	else if (noteDur == DURATION_2 || noteDur == DURATION_3_4 || noteDur == DURATION_7_8) {
   		theSymbol = kHalfNoteHeadSymbol;
	}
	else {
		theSymbol = kFullHeadSymbol;
	}

	// - Choose flag
	unsigned int theFlagSymbol = kNoneSymbol;
	if (noteDur==DURATION_8 ||  noteDur == DURATION_3_16 || noteDur == DURATION_7_32) {
		theFlagSymbol = GRFlag::H8U;
	}
	else if(noteDur==DURATION_16 || noteDur == DURATION_3_32 || noteDur == DURATION_7_64) {
		theFlagSymbol = GRFlag::H16U;
	}
	else if(noteDur == DURATION_32) {
		theFlagSymbol =  GRFlag::H32U;
	}
	else if (noteDur == DURATION_64) {
		theFlagSymbol = GRFlag::H64U;
	}

	// - Choose dot
	unsigned int theDotSymbol = kNoneSymbol;
//	if (noteDur == DURATION_3_4 || noteDur == DURATION_3_8 || noteDur == DURATION_3_16 || noteDur == DURATION_3_32) {
	if (noteDur.getNumerator() == 3) {
		theDotSymbol = kNoteDotSymbol;
	}

	// - Setup zoom
    hdc.selectfont(1); // Not very beautiful but avoid a bug during SVG export
	const float cueScale = 0.70f;
	hdc.SetScale(cueScale, cueScale);

	// - Calculate the position of the head
	if (!hdc.GetMusicFont()) hdc.SetMusicFont(FontManager::gFontScriab);
	float w = GetSymbolExtent(theSymbol);
	float xPos = (xOffset + mPosition.x) / cueScale;
	float yPos = (yOffset + mPosition.y - w / 2.5f) / cueScale;

	// - Draw Head
	hdc.DrawMusicSymbol(xPos, yPos, theSymbol);
	offsetX = w * cueScale;

	// - Draw Stem
	if (theSymbol != kWholeNoteHeadSymbol)
    {
		float		stemLen = 3 * LSPACE;
		float		stemTagSize = 1;

		const float stemCharSize = LSPACE * stemTagSize;
		const float halfStemCharSize = 0.5f * stemCharSize;

		hdc.DrawMusicSymbol( xPos, yPos, kStemUp1Symbol );

		// - Draws until the length has been completed ...
		float offsy = -halfStemCharSize;

		while( -offsy < stemLen ) // * mSize)
		{
			if(( stemCharSize - offsy ) > stemLen ) // * mSize)
			{
				offsy =  (-(stemLen) // * mSize)
					+ stemCharSize );
				hdc.DrawMusicSymbol( xPos, yPos + offsy, kStemUp2Symbol );
				break;
			}
			hdc.DrawMusicSymbol( xPos, yPos + offsy, kStemUp2Symbol );
			offsy -= halfStemCharSize;
		}
	}

	// - Draw flag
	if (theFlagSymbol != kNoneSymbol) hdc.DrawMusicSymbol( xPos, yPos - 4 * LSPACE, theFlagSymbol );

	// - Draw Dot
	if (theDotSymbol != kNoneSymbol)
    {
//		float w = GetSymbolExtent (theDotSymbol);
		hdc.DrawMusicSymbol( xPos + 2 * LSPACE, yPos, theDotSymbol);
		offsetX += LSPACE;
	}

	// - Cleanup
	hdc.SetScale(1 / cueScale, 1 / cueScale);
	return offsetX;
}

// ----------------------------------------------------------------------------
void GRTempo::DrawText( VGDevice & hdc, const char * cp, float xOffset, float yOffset, float * outWidth ) const
{
//	hdc.SelectFont( mFont );
	hdc.SetTextFont( mFont );
    hdc.SetFontAlign (getTextAlign());
	hdc.DrawString ( xOffset + mPosition.x, yOffset + mPosition.y, cp, (int)strlen(cp) );
	if( outWidth ) {
		float fooHeight;
//		hdc.GetTextExtent(cp, (int)strlen(cp), outWidth, &fooHeight);
                const VGFont *font = hdc.GetTextFont();
                if (!font) {
                        font = FontManager::gFontText;
                }
                if (!font) {
                        std::cerr << "Cannot find text font" << std::endl;
                        *outWidth = 0;
                } else {
		        font->GetExtent(cp, (int)strlen(cp), outWidth, &fooHeight, &hdc);
                }
	}
}

// ----------------------------------------------------------------------------
unsigned int GRTempo::getTextAlign() const
{
	return VGDevice::kAlignBaseLeft;
}
