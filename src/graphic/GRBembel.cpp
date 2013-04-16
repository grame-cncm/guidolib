/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARBembel.h"

#include "GRBembel.h"
#include "GRDefine.h"

unsigned int GRBembel::sBembelSymbol = kBembelSymbol; // SCR_BEMBEL	was 164
NVPoint GRBembel::refpos;

GRBembel::GRBembel(ARBembel * par)
		: GRTagARNotationElement(/*dynamic cast<ARMusicalObject *>*/(par), LSPACE)
{
	// No!
	mNeedsSpring = 1;

	// obsolete
	// spacing = 0;
	mSymbol = sBembelSymbol;

	const float extent = (float)GetSymbolExtent( mSymbol );

	// obsolete ..
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;
	mBoundingBox.right = (GCoord)extent;
	mBoundingBox.bottom = (GCoord)(3 * LSPACE);

	mLeftSpace = (GCoord)(extent * 0.5f);
	mRightSpace = (GCoord)(extent * 0.5f); // extent;


	// no referencePosition?
	refpos = NVPoint( (GCoord)(-extent * 0.5f) ,0);
}

GRBembel::~GRBembel()
{
}

void GRBembel::print() const
{
}

void GRBembel::OnDraw(VGDevice & hdc) const
{
	GRTagARNotationElement::OnDraw(hdc);
}

