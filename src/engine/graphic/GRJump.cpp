/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <iostream>
#include <ctype.h>
using namespace std;

#include "ARJump.h"
#include "GRJump.h"
#include "GRDefine.h"
#include "FontManager.h"
#include "GUIDOInternal.h"
#include "VGDevice.h"
#include "VGFont.h"

NVPoint		GRJump::refpos;
SymbolMap   GRJump::mSymbols;
const VGFont*	GRJump::mFont;

//----------------------------------------------------------------------------------
GRJump::GRJump( ARJump * ar, bool rightJustified )
		: GRTagARNotationElement( ar, LSPACE ), mRightJustified(rightJustified)
{
	if (mSymbols.empty()) {				// first time initialization
		mSymbols["coda"] = kCodaSymbol;
		mSymbols["segno"] = kSegnoSymbol;

		const NVstring fontname("Times New Roman Bold Italic");
		NVstring attrs ("bi");
		mFont = FontManager::FindOrCreateFont( 110, &fontname, &attrs);
	}

	mNeedsSpring = 1;
	sconst = SCONST_INTENS;
	refpos = NVPoint( 0, 0 );
	mPosition.y = - 2 * LSPACE;	// above the staff

	mBoundingBox.top = -LSPACE;
	mBoundingBox.bottom = LSPACE;
	mBoundingBox.left = 0;
	mBoundingBox.right = 0;

	VGDevice * hdc = gGlobalSettings.gDevice;
	if (ar) {
		FormatStringParserResult::const_iterator assoc;
		for (assoc = ar->getMark().begin(); assoc != ar->getMark().end(); assoc++) {
			if (assoc->second == FormatStringParser::kSpecial) {
				unsigned int symbol = mSymbols[assoc->first];
				if (symbol)
					mBoundingBox.right += GetSymbolExtent( symbol );
			}
			else if (hdc) {
				float w, h;
				int size = int(assoc->first.size());
				mFont->GetExtent( assoc->first.c_str(), size, &w, &h, hdc );
				mBoundingBox.right += w;
			}
		}
	}

	if (rightJustified) {
		float offset = mBoundingBox.right - mBoundingBox.left;
		mBoundingBox.right -= offset;
		mBoundingBox.left -= offset;
	}
	else {
		mBoundingBox.right -= LSPACE;
		mBoundingBox.left -= LSPACE;
	}
	mLeftSpace = 0;
	mRightSpace = mBoundingBox.left - mBoundingBox.right;
}

GRJump::~GRJump()   {}


//----------------------------------------------------------------------------------
const VGFont* GRJump::SelectTextFont( VGDevice & hdc ) const
{
//    hdc.SelectFont( mFont );
    hdc.SetTextFont( mFont );
    hdc.SetFontAlign (VGDevice::kAlignBaseLeft);
	return mFont;
}

//----------------------------------------------------------------------------------
const ARJump * GRJump::getARJump() const
{
	return static_cast<ARJump *>(getAbstractRepresentation()->isARJump());
}

//----------------------------------------------------------------------------------
float GRJump::getSymbolSize () const	{ return 1.0f; }

//----------------------------------------------------------------------------------
void GRJump::OnDraw( VGDevice & hdc ) const
{
//	DrawBoundingBox (hdc, VGColor(255,0,0));
	if(!mDraw || !mShow)
		return;

	const ARJump * ar = getARJump();
	if (!ar) return;

    NVRect r = getBoundingBox();
	NVPoint pos = getPosition();
	pos.x += r.left;

	FormatStringParserResult::const_iterator assoc;
	for (assoc = ar->getMark().begin(); assoc != ar->getMark().end(); assoc++) {
		if (assoc->second == FormatStringParser::kSpecial) {
			unsigned int symbol = mSymbols[assoc->first];
			if (symbol) {
				OnDrawSymbol( hdc, symbol, pos.x - getPosition().x, 0, getSymbolSize() );
				pos.x += GetSymbolExtent( symbol ) * getSymbolSize();
			}
		}
        else {
            const VGColor prevTextColor = hdc.GetFontColor();
            if (mColRef)
                hdc.SetFontColor(VGColor(mColRef));

            const VGFont* font = SelectTextFont (hdc);
            const char * text = assoc->first.c_str();
            int size = int(assoc->first.size());
            float w, h, texty = pos.y + r.bottom + getOffset().y;
            font->GetExtent ( 'a', &w, &h, &hdc );
            texty += (r.bottom - r.top - h) / 2;
            hdc.DrawString( pos.x + getOffset().x, texty, text, size);
            font->GetExtent ( text, size, &w, &h, &hdc );
            pos.x += w;

            if (mColRef)
                hdc.SetFontColor(prevTextColor);
		}
	}
}
