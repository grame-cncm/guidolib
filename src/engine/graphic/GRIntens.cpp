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

#include <iostream>
#include <string>
#include <string.h>

#include "ARIntens.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "MusicalSymbols.h"

#include "GRIntens.h"
#include "GRStaff.h"
#include "GRDefine.h"
#include "GRSingleNote.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"

#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

GRIntens::GRIntens( GRStaff * inStaff, const ARIntens* ar)
  : GRTagARNotationElement(ar, LSPACE)
{
	assert(ar);

	// this is not quite clear!
	mNeedsSpring = 1;
	sconst = SCONST_INTENS;
	mGrStaff = inStaff;

	string intens = getARIntens()->getText().c_str();
	if (intens == "p")			mSymbol = kIntensPSymbol;
	else if (intens == "f")		mSymbol = kIntensFSymbol;
	else if (intens == "ff")	mSymbol = kIntensFFSymbol;
	else if (intens == "fff")	mSymbol = kIntensFFFSymbol;
	else if (intens == "ffff")	mSymbol = kIntensFFFFSymbol;
	else if (intens == "mf")	mSymbol = kIntensMFSymbol;
	else if (intens == "mp")	mSymbol = kIntensMPSymbol;
	else if (intens == "fp")	mSymbol = kIntensFPSymbol;
	else if (intens == "sf")	mSymbol = kIntensSFSymbol;
	else if (intens == "pp")	mSymbol = kIntensPPSymbol;
	else if (intens == "ppp")	mSymbol = kIntensPPPSymbol;
	else if (intens == "pppp")	mSymbol = kIntensPPPPSymbol;
	else if (intens == "sfz")	mSymbol = kIntensSFZSymbol;
	else if (intens == "rfz")	mSymbol = kIntensRFZSymbol;
	else if (intens == "fz")	mSymbol = kIntensFZSymbol;
	else mSymbol = 0;

	const float curLSPACE = mGrStaff ? mGrStaff->getStaffLSPACE() : LSPACE;
	fTextAlign = VGDevice::kAlignBase;
	fFont = FontManager::GetTextFont(ar, curLSPACE, fTextAlign);

	if (mSymbol != 0) {
		float x = GetSymbolExtent(mSymbol);
		fDx = ar->getDX()->getValue();
		fDy = ar->getDY()->getValue();

		const TagParameterFloat* p = ar->getSize();
		if (p) fSize = p->getValue();

		mBoundingBox.left  = fDx - x/3;
		mBoundingBox.right = mBoundingBox.left + x;
		mBoundingBox.top = LSPACE * -1.5;
		mBoundingBox.bottom = LSPACE/2;
	}
	else mBoundingBox.Set( 0, 0, 0, 0 );
	mLeftSpace = 0;
	mRightSpace = 0;
	mPosition.y =  6 * curLSPACE;
}

// -----------------------------------------------------------------------------
void GRIntens::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
void GRIntens::OnDraw(VGDevice & hdc) const
{
	if(!mDraw || !mShow) return;

	const float space = (mGrStaff ? mGrStaff->getStaffLSPACE() : LSPACE) / 2;
	const ARIntens* ar = getARIntens();

	hdc.SetTextFont( fFont );
	const VGColor prevTextColor = hdc.GetFontColor();
	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));

	float sy = 0;
	float w = 0;
	if (ar->autoPos() && fNote) {
		NVRect r = fNote->getEnclosingBox(false, false, false);
		sy = (r.bottom - mPosition.y) + space * 3.5f * fSize;
		if (sy < 0) sy = 0;
		w = GetSymbolExtent(mSymbol) / 3 * fSize;
	}
	float y = mPosition.y - fDy + sy;
	float x = mPosition.x + fDx - w;

	if (mSymbol) OnDrawSymbol (hdc, mSymbol, -w, sy, 0);

	// - Print text
	const string& textafter  = ar->getTextAfter();
	const string& textbefore = ar->getTextBefore();
	unsigned int valign = fTextAlign & (VGDevice::kAlignBase + VGDevice::kAlignTop + VGDevice::kAlignBottom);
    if (!textafter.empty()) {
		hdc.SetFontAlign( valign + VGDevice::kAlignLeft );
	    hdc.DrawString( x + mBoundingBox.Width() * fSize + space, y, textafter.c_str(), int(textafter.size()));
	}

    if (!textbefore.empty()) {
		hdc.SetFontAlign( valign + VGDevice::kAlignRight );
		hdc.DrawString( x - space/2, y, textbefore.c_str(), int(textbefore.size()));
	}

	if( mColRef ) hdc.SetFontColor( prevTextColor );
}

// -----------------------------------------------------------------------------
const ARIntens* GRIntens::getARIntens() const
{
	return static_cast<const ARIntens*>(getAbstractRepresentation());
}
