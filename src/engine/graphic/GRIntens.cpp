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

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <iostream>
#include <string.h>

#include "ARIntens.h"
#include "TagParameterFloat.h"
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
	const char * cp = getARIntens()->getText().c_str();
	if (!strcmp(cp,"p"))		mSymbol = INTENS_P;
	else if (!strcmp(cp,"f") )	mSymbol = INTENS_F;
	else if (!strcmp(cp,"ff"))	mSymbol = INTENS_FF;
	else if (!strcmp(cp,"fff"))	mSymbol = INTENS_FFF;
	else if (!strcmp(cp,"ffff"))mSymbol = INTENS_FFFF;
	else if (!strcmp(cp,"mf"))	mSymbol = INTENS_MF;
	else if (!strcmp(cp,"mp"))	mSymbol = INTENS_MP;
	else if (!strcmp(cp,"sf"))	mSymbol = INTENS_SF;
	else if (!strcmp(cp,"pp"))	mSymbol = INTENS_PP;
	else if (!strcmp(cp,"ppp"))	mSymbol = INTENS_PPP;
	else if (!strcmp(cp,"pppp"))mSymbol = INTENS_PPPP;

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
	if(!mDraw || !mShow)
		return;
	GRTagARNotationElement::OnDraw( hdc );
}


const ARIntens* GRIntens::getARIntens() const
{
	return static_cast<const ARIntens*>(getAbstractRepresentation());
}





