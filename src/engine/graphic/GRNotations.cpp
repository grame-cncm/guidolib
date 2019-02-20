/*
  GUIDO Library
  Copyright (C) 2019Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "ARNotations.h"
#include "FontManager.h"
#include "GRStaff.h"
#include "GuidoDefs.h"
#include "GUIDOInternal.h"
#include "MusicalSymbols.h"
#include "TagParameterFloat.h"
#include "VGFont.h"

#include "GRNotations.h"

using namespace std;

GRNotations::GRNotations( GRStaff * inStaff, const ARNotations * ar)
  : GRTagARNotationElement(ar, LSPACE)
{
	assert(ar);

	// this is not quite clear!
	mNeedsSpring = 1;
	sconst = SCONST_INTENS;
	mGrStaff = inStaff;

	switch (ar->getType()) {
		case ARNotations::kPedalBegin:
			mSymbol = kPedalSymbol;
			break;
		case ARNotations::kPedalEnd:
			mSymbol = kPedalUpSymbol;
			break;
		default:
			mSymbol = 0;
	}
	
	if (mSymbol != 0) {
		float x = 0;
		float y = 0;
		if( gGlobalSettings.gDevice )
			FontManager::gFontScriab->GetExtent( mSymbol, &x, &y, gGlobalSettings.gDevice );

		float dx = ar->getDX()->getValue();
		float dy = ar->getDY()->getValue();
		float size = ar->getSize() ? ar->getSize()->getValue() : 1;

		mBoundingBox.left  = dx;
		mBoundingBox.right = x * size + dx;
		mBoundingBox.top = LSPACE * -2 * size - dy;
		mBoundingBox.bottom = LSPACE / 2 - dy;
	}
	else mBoundingBox.Set( 0, 0, 0, 0 );

	mLeftSpace = 0;
	mRightSpace = 0;

	const float curLSPACE = mGrStaff ? mGrStaff->getStaffLSPACE() : LSPACE;
	mPosition.y = 7 * curLSPACE;
}

// -----------------------------------------------------------------------------
void GRNotations::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
void GRNotations::OnDraw(VGDevice & hdc) const
{
	if(!mDraw || !mShow) return;

//	NVRect r = getBoundingBox() + getPosition();
//	hdc.Frame (r.left, r.top, r.right, r.bottom);
//	r = mGrStaff->getBoundingBox();
//	hdc.Frame (r.left, r.top, r.right, r.bottom);
	GRTagARNotationElement::OnDraw( hdc );
}




