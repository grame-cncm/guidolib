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

#include <math.h>

#include "ARFinishBar.h"
#include "GRFinishBar.h"
#include "GuidoDefs.h"
#include "GRStaff.h"
#include "VGDevice.h"
#include "ARStaffFormat.h"

#include <iostream>
using namespace std;


NVPoint GRFinishBar::refpos;

GRFinishBar::GRFinishBar( ARFinishBar * p_ardbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & p_tp )
				: GRBar(p_ardbar,inStaff ,p_tp)
{
	mSymbol = kFinishBarSymbol;

	mLeftSpace = 55;	// hardcoded
	mRightSpace = 0;

	refpos.x = -40;		// hardcoded
//	refpos.y = inStaff ? inStaff->getDredgeSize() : (4 * LSPACE);
	refpos.y = 4 * LSPACE;

    fBaseThickness = LSPACE * 0.5f;
    fLineNumber = inStaff->getNumlines();
    fStaffThickness = inStaff->getLineThickness();
    fSize = inStaff->getSizeRatio();

	updateBoundingBox();
}

GRFinishBar::GRFinishBar(ARFinishBar * p_arbar, GRSystem * p_grsystem, GRStaff * inStaff,
														const TYPE_TIMEPOSITION & p_timeposition )
			: GRBar(p_arbar,p_grsystem,inStaff,p_timeposition)
{
	mSymbol = kFinishBarSymbol;

	mLeftSpace = 55;	// hardcoded
	mRightSpace = 0;

	refpos.x = -40;		// hardcoded
//	refpos.y = inStaff ? inStaff->getDredgeSize() : (4 * LSPACE);
	refpos.y = 4 * LSPACE;

    fBaseThickness = LSPACE * 0.5f;
    fLineNumber = inStaff->getNumlines();
    fStaffThickness = inStaff->getLineThickness();
    fSize = inStaff->getSizeRatio();

	updateBoundingBox();
}

GRFinishBar::~GRFinishBar()
{
}

// --------------------------------------------------------------------------
void GRFinishBar::updateBoundingBox()
{
    fBaseThickness = LSPACE * 0.5f * fSize;
	const float spacing = LSPACE * 0.4f * fSize;
	const float extend = spacing + fBaseThickness;
	mBoundingBox.left  = - extend;
	mBoundingBox.right = 0;
}

// --------------------------------------------------------------------------
void GRFinishBar::DrawWithLines( VGDevice & hdc ) const
{
	if ((getTagType() != GRTag::SYSTEMTAG) && isSystemSlice())
		return;			// don't draw staff bars on system slices

    // - Vertical adjustement according to staff's line number
    float offsety1 = (fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
    float offsety2 = 0;

    if (fLineNumber != 0 && fLineNumber != 1)
        offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

    const float spacing = LSPACE * 0.4f * fSize;
	const float x1 = mPosition.x - mBoundingBox.Width() + fSize * 1.8f + 1;
	const float x2 = x1 + spacing;
    const float y1 = mPosition.y + (offsety1 - 2) * fSize;
	const float y2 = y1 + mBoundingBox.bottom + (offsety2 + 4) * fSize;

    float leftLineThickness = 1.8f * kLineThick * fSize;

	hdc.Rectangle(x1, y1, x1 + leftLineThickness, y2);
	hdc.Rectangle(x2, y1, x2 + fBaseThickness, y2);
}

ARFinishBar * GRFinishBar::getARFinishBar()
{
	return static_cast/*dynamic cast*/<ARFinishBar *>(mAbstractRepresentation);
}