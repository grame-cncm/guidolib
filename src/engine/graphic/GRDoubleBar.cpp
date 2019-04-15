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

#include <math.h>

#include "GUIDOInternal.h"

#include "GRStaffManager.h"
#include "GRDoubleBar.h"
#include "ARDoubleBar.h"
#include "GRStaff.h"
#include "GuidoDefs.h"
#include "VGDevice.h"

#include <iostream>
using namespace std;

GRDoubleBar::GRDoubleBar(const ARDoubleBar * ardbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float propRender)
				: GRBar(ardbar, inStaff ,inTimePos, propRender)
{
	mSymbol = kDoubleBarSymbol;
	const float spacing = inStaff->getStaffLSPACE() * 0.2f;
	mBoundingBox.left  -= spacing;
	mBoundingBox.right += spacing;
    mRightSpace = mBoundingBox.right * mTagSize * inStaff->getSizeRatio();
    fStaffThickness = inStaff->getLineThickness();
}

GRDoubleBar::GRDoubleBar(const ARDoubleBar * ardbar, GRSystem * p_grsystem, GRStaff * inStaff,
														const TYPE_TIMEPOSITION & inTimePos, float propRender)
			: GRBar(ardbar,p_grsystem,inStaff,inTimePos, propRender)
{
	mSymbol = kDoubleBarSymbol;
	const float spacing = inStaff->getStaffLSPACE() * 0.2f;
	mBoundingBox.left  -= spacing;
	mBoundingBox.right += spacing;
	mRightSpace = mBoundingBox.right * mTagSize * inStaff->getSizeRatio();
    fStaffThickness = inStaff->getLineThickness();
}

// --------------------------------------------------------------------------
void GRDoubleBar::DrawWithLines( VGDevice & hdc ) const
{
	if ((getTagType() != GRTag::SYSTEMTAG) && isSystemSlice())
		return;			// don't draw staff bars on system slices
    
    const float staffSize = mGrStaff->getSizeRatio();
    if (staffSize < kMinNoteSize) // Too small, don't draw
        return;

    if (mColRef)
        hdc.PushPenColor(VGColor(mColRef));

	DisplayMeasureNum (hdc);

    float lineThickness = kLineThick * 1.5f * staffSize;
    hdc.PushPenWidth(lineThickness);

    const float spacing = LSPACE * 0.75f * staffSize;
	const float x2 = getXPos (staffSize) + 14 - lineThickness;
	const float x1 = x2 - spacing;

	if (fRanges.empty()) {
		const float y1 = getY1 (mBoundingBox.top);
		const float y2 = getY2 (y1, mBoundingBox.bottom);
		hdc.Line(x1, y1, x1, y2);
		hdc.Line(x2, y1, x2, y2);
	}
	else
	for (size_t i=0; i< fRanges.size(); i++) {
		float y1 = getY1 (fRanges[i].first) + lineThickness / 2;
		float y2 = getY2 (-mDy, fRanges[i].second) - lineThickness / 2;
		hdc.Line(x1, y1, x1, y2);
		hdc.Line(x2, y1, x2, y2);
	}

    hdc.PopPenWidth();
    if (mColRef)
        hdc.PopPenColor();
}

const ARDoubleBar * GRDoubleBar::getARDoubleBar() const	{ return static_cast<const ARDoubleBar *>(mAbstractRepresentation); }
