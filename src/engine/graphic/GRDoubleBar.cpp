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

#include "GUIDOInternal.h"

#include "GRStaffManager.h"
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
    mRightSpace = mBoundingBox.right * mTagSize * inStaff->getSizeRatio();

    fLineNumber = inStaff->getNumlines();
    fStaffThickness = inStaff->getLineThickness();
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
	mRightSpace = mBoundingBox.right * mTagSize * inStaff->getSizeRatio();

    fLineNumber = inStaff->getNumlines();
    fStaffThickness = inStaff->getLineThickness();
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
    
    if (fSize < kMinNoteSize) // Too small, don't draw
        return;

    if (mColRef)
        hdc.PushPenColor(VGColor(mColRef));

    // - Vertical adjustement according to staff's line number
    float offsety1 = (fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
    float offsety2 = 0;

    if (fLineNumber != 0 && fLineNumber != 1)
        offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

    float proportionalRenderingXOffset = 0;
    if (GRStaffManager::sPropRender != 0)
        proportionalRenderingXOffset = - 90; // REM: 30 hardcoded (1.5 * note extent)

    // - Horizontal adjustement according to staff's lines size and staff's size
    const float offsetX = (fStaffThickness - 4) * 0.5f - 24 + proportionalRenderingXOffset;

    const float spacing = LSPACE * 0.7f * fSize;
	const float x1 = mPosition.x + offsetX;
	const float x2 = x1 + spacing;
	const float y1 = mPosition.y + mBoundingBox.top + offsety1 * fSize;
	const float y2 = y1 + mBoundingBox.bottom + offsety2 * fSize;

    float lineThickness = kLineThick * 1.5f * fSize;

    hdc.PushPenWidth(lineThickness);
    hdc.Line(x1, y1 + lineThickness / 2, x1, y2 - lineThickness / 2);
    hdc.Line(x2, y1 + lineThickness / 2, x2, y2 - lineThickness / 2);
    hdc.PopPenWidth();

    if (mColRef)
        hdc.PopPenColor();
}

ARDoubleBar * GRDoubleBar::getARDoubleBar()	{ return static_cast<ARDoubleBar *>(mAbstractRepresentation); }
