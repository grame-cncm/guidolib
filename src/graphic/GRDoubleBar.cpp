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

#include "GRDoubleBar.h"
#include "ARDoubleBar.h"
#include "GRStaff.h"
#include "GuidoDefs.h"
#include "VGDevice.h"

#include <iostream>
using namespace std;

GRDoubleBar::GRDoubleBar( ARDoubleBar * ardbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos)
				: GRBar(ardbar, inStaff ,inTimePos)
{
	mSymbol = kDoubleBarSymbol;
	const float spacing = inStaff->getStaffLSPACE() * 0.2f;
	mBoundingBox.left  -= spacing;
	mBoundingBox.right += spacing;
	mRightSpace = mBoundingBox.right * mTagSize;

    fLineNumber = inStaff->getNumlines();
    fSize = inStaff->getSizeRatio();
}

GRDoubleBar::GRDoubleBar(ARDoubleBar * ardbar, GRSystem * p_grsystem, GRStaff * inStaff,
														const TYPE_TIMEPOSITION & inTimePos )
			: GRBar(ardbar,p_grsystem,inStaff,inTimePos)
{
	mSymbol = kDoubleBarSymbol;
	const float spacing = inStaff->getStaffLSPACE() * 0.2f;
	mBoundingBox.left  -= spacing;
	mBoundingBox.right += spacing;
	mRightSpace = mBoundingBox.right * mTagSize;

    fLineNumber = inStaff->getNumlines();
    fSize = inStaff->getSizeRatio();
}

GRDoubleBar::~GRDoubleBar()
{
}

// --------------------------------------------------------------------------
void GRDoubleBar::DrawWithLines( VGDevice & hdc ) const
{
	if ((getTagType() != GRTag::SYSTEMTAG) && isSystemSlice())
		return;			// don't draw staff bars on system slices

    // - Vertical adjustement according to staff's line number
    float offsety1 = (fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
    float offsety2 = 0;

    if (fLineNumber != 0 && fLineNumber != 1)
    offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

	const float x1 = mPosition.x + mBoundingBox.left;
	const float x2 = mPosition.x + mBoundingBox.right;
	const float y1 = mPosition.y + offsety1 * fSize;
	const float y2 = y1 + mBoundingBox.bottom + offsety2 * fSize;

    float lineThickness = kLineThick * 1.2f * fSize;

	hdc.Rectangle(x1, y1, x1 + lineThickness, y2);
	hdc.Rectangle(x2, y1, x2 + lineThickness, y2);
}

ARDoubleBar * GRDoubleBar::getARDoubleBar()	{ return static_cast<ARDoubleBar *>(mAbstractRepresentation); }
