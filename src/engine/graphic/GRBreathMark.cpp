/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

//(JB) experimental implementation of breath-marks

#include "ARBreathMark.h"
#include "GRBreathMark.h"
#include "GRDefine.h"

unsigned int GRBreathMark::sBreathMarkSymbol = kBreathMarkSymbol;
NVPoint GRBreathMark::refpos;

GRBreathMark::GRBreathMark( ARBreathMark * ar )
		: GRTagARNotationElement( ar, LSPACE )
{
	mNeedsSpring = 1;

	mSymbol = sBreathMarkSymbol;

	const float extent = (float)GetSymbolExtent( mSymbol );

	// obsolete ..
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;
	mBoundingBox.right = 0;
	mBoundingBox.bottom = 0; // todo

	// - A breath-mark must not change the spacing.
	mLeftSpace = 0;
	mRightSpace = 0;

	// no reference position?
	refpos = NVPoint( - extent - float(0.25) * LSPACE, 0 );

	mPosition.y = - float(1.5) * LSPACE;
}

GRBreathMark::~GRBreathMark()
{
}

void GRBreathMark::OnDraw(VGDevice & hdc) const
{
	GRTagARNotationElement::OnDraw(hdc);
}

void GRBreathMark::print(std::ostream& os) const
{
}

