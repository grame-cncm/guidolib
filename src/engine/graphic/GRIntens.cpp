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

	// Now try to figure out, which intensity was selected:
//	const char * cp = getARIntens()->getText().c_str();
//	if (!strcmp(cp,"p"))		mSymbol = kIntensPSymbol;
//	else if (!strcmp(cp,"f") )	mSymbol = kIntensFSymbol;
//	else if (!strcmp(cp,"ff"))	mSymbol = kIntensFFSymbol;
//	else if (!strcmp(cp,"fff"))	mSymbol = kIntensFFFSymbol;
//	else if (!strcmp(cp,"ffff"))mSymbol = kIntensFFFFSymbol;
//	else if (!strcmp(cp,"mf"))	mSymbol = kIntensMFSymbol;
//	else if (!strcmp(cp,"mp"))	mSymbol = kIntensMPSymbol;
//	else if (!strcmp(cp,"sf"))	mSymbol = kIntensSFSymbol;
//	else if (!strcmp(cp,"pp"))	mSymbol = kIntensPPSymbol;
//	else if (!strcmp(cp,"ppp"))	mSymbol = kIntensPPPSymbol;
//	else if (!strcmp(cp,"pppp"))mSymbol = kIntensPPPPSymbol;

	string intens = getARIntens()->getText().c_str();
	if (intens == "p")			mSymbol = kIntensPSymbol;
	else if (intens == "f")		mSymbol = kIntensFSymbol;
	else if (intens == "ff")	mSymbol = kIntensFFSymbol;
	else if (intens == "fff")	mSymbol = kIntensFFFSymbol;
	else if (intens == "ffff")	mSymbol = kIntensFFFFSymbol;
	else if (intens == "mf")	mSymbol = kIntensMFSymbol;
	else if (intens == "mp")	mSymbol = kIntensMPSymbol;
	else if (intens == "sf")	mSymbol = kIntensSFSymbol;
	else if (intens == "pp")	mSymbol = kIntensPPSymbol;
	else if (intens == "ppp")	mSymbol = kIntensPPPSymbol;
	else if (intens == "pppp")	mSymbol = kIntensPPPPSymbol;
	else if (intens == "sfz")	mSymbol = kIntensSFZSymbol;
	else if (intens == "rfz")	mSymbol = kIntensRFZSymbol;
	else if (intens == "fz")	mSymbol = kIntensFZSymbol;
	else mSymbol = 0;


	if (mSymbol != 0) {
		float x = 0;
		float y = 0;
		float dx = 0;
		float dy = 0;

		if( gGlobalSettings.gDevice )
			FontManager::gFontScriab->GetExtent( mSymbol, &x, &y, gGlobalSettings.gDevice );

		const TagParameterFloat* p = ar->getDX();
		dx = p->getValue();
		p = ar->getDY();
		if (p) dy = -p->getValue();

		mBoundingBox.left  = dx;
		mBoundingBox.right = x + dx;
		mBoundingBox.top = LSPACE * -1.5;
		mBoundingBox.bottom = LSPACE/2;
	}
	else mBoundingBox.Set( 0, 0, 0, 0 );

	mLeftSpace = 0;
	mRightSpace = 0;

	// no referencePosition?
	// probably dependant on Type!
	const float curLSPACE = mGrStaff ? mGrStaff->getStaffLSPACE() : LSPACE;
	mPosition.y = (GCoord)(6 * curLSPACE);
}

// -----------------------------------------------------------------------------
void GRIntens::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

//## Other Operations (implementation)
void GRIntens::OnDraw(VGDevice & hdc) const
{
	if(!mDraw || !mShow) return;
	GRTagARNotationElement::OnDraw( hdc );

	const float space = (mGrStaff ? mGrStaff->getStaffLSPACE() : LSPACE) / 2;
	const ARIntens* ar = getARIntens();
	const string font = ar->getFont();
	const string attr = ar->getTextAttributes();
	const VGFont* hmyfont = FontManager::FindOrCreateFont( int(ar->getFSize()), &font, &attr );

	hdc.SetTextFont( hmyfont );
	const VGColor prevTextColor = hdc.GetFontColor();

	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));

	// - Print text
	const string& textafter = ar->getTextAfter();
	float y = mPosition.y - ar->getDY()->getValue();
	float x = mPosition.x + ar->getDX()->getValue();
    if (!textafter.empty()) {
		hdc.SetFontAlign( VGDevice::kAlignBase + VGDevice::kAlignLeft );
	    hdc.DrawString( mPosition.x + mBoundingBox.right + space, y, textafter.c_str(), int(textafter.size()));
	}

	const string& text = ar->getTextBefore();
    if (!text.empty()) {
		hdc.SetFontAlign( VGDevice::kAlignBase + VGDevice::kAlignRight );
		hdc.DrawString( x - space/2, y, text.c_str(), int(text.size()));
	}
	
	if( mColRef ) hdc.SetFontColor( prevTextColor );
}

const ARIntens* GRIntens::getARIntens() const
{
	return static_cast<const ARIntens*>(getAbstractRepresentation());
}





