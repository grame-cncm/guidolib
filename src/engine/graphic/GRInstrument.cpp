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

#include <string.h>

#include "GRInstrument.h"
#include "GRDefine.h"
#include "ARMusicalObject.h"
#include "ARInstrument.h"

GRInstrument::GRInstrument(ARInstrument * par)
		: GRTagARNotationElement(par, LSPACE)
{
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;

	// No!
	mNeedsSpring = 1;


	// obsolete
	// spacing = 0;

	// to do: read real text ...
	const GCoord extent = 100;

	mBoundingBox.right = extent;
	mBoundingBox.bottom = (GCoord)(3*LSPACE);

	// no Space needed !
	mLeftSpace = 0;
	mRightSpace = 0;

	// no reference position ...
}

GRInstrument::~GRInstrument()
{
}

void GRInstrument::OnDraw(VGDevice & hdc) const
{
	// Now draw something
	//HFONT hfontold = (HFONT) SelectObject(hdc,hfontscriab);
	//PS_SELECTSCRIABIN ;

	const char* nv = static_cast/*dynamic cast*/<ARInstrument *>(mAbstractRepresentation)->getName();
	if (nv == 0) return;
	if(!mDraw)
		return;
	GRTagARNotationElement::OnDrawText(hdc, nv, strlen(nv));
}

