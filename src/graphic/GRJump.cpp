/*
	GUIDO Library
	Copyright (C) 2004	Grame

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
	return dynamic_cast<ARJump *>(getAbstractRepresentation());
}

//----------------------------------------------------------------------------------
float GRJump::getSymbolSize () const	{ return 1.0f; }

//----------------------------------------------------------------------------------
void GRJump::OnDraw( VGDevice & hdc ) const
{
//	DrawBoundingBox (hdc, VGColor(255,0,0));

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
			const VGFont* font = SelectTextFont (hdc);
			const char * text = assoc->first.c_str();
			int size = int(assoc->first.size());
			float w, h, texty = pos.y + r.bottom + getOffset().y;
			font->GetExtent ( 'a', &w, &h, &hdc );
			texty += (r.bottom - r.top - h) / 2;
			hdc.DrawString( pos.x + getOffset().x, texty, text, size);
			font->GetExtent ( text, size, &w, &h, &hdc );
			pos.x += w;
		}
	}
}
