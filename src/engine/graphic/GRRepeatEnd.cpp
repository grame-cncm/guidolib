/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <math.h>

#include "NEPointerList.h"	// TEMP

#include "GRRepeatEnd.h"

#include "GRStaff.h"
#include "ARRepeatEnd.h"
#include "ARRepeatEndRangeEnd.h"
#include "GUIDOInternal.h"
#include "VGDevice.h"

using namespace std;

NVPoint GRRepeatEnd::refpos;

// --------------------------------------------------------------------------
/*
GRRepeatEnd::GRRepeatEnd( ARRepeatEnd * ar, bool ownsar )
				: GRTagARNotationElement( ar, LSPACE, ownsar )
//				: GRPTagARNotationElement( ar, ownsar )
*/
GRRepeatEnd::GRRepeatEnd( ARRepeatEnd * arre, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float proportionnalRender )
					: GRBar(arre, inStaff, inTimePos, proportionnalRender)
{
	InitRepeatEnd();
}

// --------------------------------------------------------------------------
/*
GRRepeatEnd::GRRepeatEnd( ARRepeatEndRangeEnd * ar, bool ownsar )
						: GRTagARNotationElement( ar, LSPACE, ownsar )
//				: GRPTagARNotationElement( ar, ownsar )
{
	InitRepeatEnd();
} 
*/

// --------------------------------------------------------------------------
GRRepeatEnd::~GRRepeatEnd()
{
}

ARRepeatEnd* GRRepeatEnd::getARRepeatEnd() {
	return dynamic_cast<ARRepeatEnd*>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
void GRRepeatEnd::InitRepeatEnd()
{
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;
//	sconst = 5; //SCONST_BAR;

	mSymbol = kRepeatEndSymbol; 
	mLeftSpace =  mRightSpace = 0;
	refpos.Set( -LSPACE * 0.8f, 4 * LSPACE );

    GRStaff *staff = getGRStaff();

    if (staff)
    {
        fLineNumber = staff->getNumlines();

        fStaffThickness = staff->getLineThickness();
        fSize = staff->getSizeRatio();
        fBaseThickness = LSPACE * 0.6f * fSize;
    }
	updateBoundingBox();
}

// --------------------------------------------------------------------------
void GRRepeatEnd::setPosFrom(GCoord posy)	{ mBoundingBox.top = posy; }
void GRRepeatEnd::setPosTo(GCoord posy)		{ mBoundingBox.bottom = posy; }

// --------------------------------------------------------------------------
void GRRepeatEnd::setHPosition(float nx)
{
	GRBar::setHPosition(nx);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset() - NVPoint(15,0);
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRRepeatEnd::GetMap(GuidoElementSelector sel, MapCollector& f, MapInfos& infos) const
{
	if (sel == kGuidoBar)
		SendMap (f, getRelativeTimePosition(), getDuration(), kRepeatEnd, infos);
}

// --------------------------------------------------------------------------
void GRRepeatEnd::updateBoundingBox()
{
	const float extent = GetSymbolExtent( mSymbol );
	const float halfExtent = extent * 0.5f;

	mBoundingBox.top = 0;
	mBoundingBox.left = -halfExtent + refpos.x;
	mBoundingBox.right = halfExtent + refpos.x;
	mBoundingBox.bottom = 4 * LSPACE;
}

// --------------------------------------------------------------------------
void GRRepeatEnd::DrawDots( VGDevice & hdc ) const
{
   float offsety1AccordingToLineNumber = 0;
    float offsety2AccordingToLineNumber = 0;

    if (fLineNumber == 0)
        offsety1AccordingToLineNumber = - LSPACE / 2 * fSize;
    else if (fLineNumber == 1)
        offsety1AccordingToLineNumber = - LSPACE * fSize;
    else if (fLineNumber == 2)
    {
        offsety1AccordingToLineNumber = 14 * fSize;
        offsety2AccordingToLineNumber = - 2 * offsety1AccordingToLineNumber;
    }

    float pointOffsety1 = - 5 * fSize + offsety1AccordingToLineNumber;
    float pointOffsety2 = pointOffsety1 + LSPACE * fSize + offsety2AccordingToLineNumber;
    float pointOffsetx  = (fStaffThickness - 4) * 0.5f - 62 * (fSize - 1) + (fStaffThickness - 4) * (fSize - 1) * 0.5f - 57;
    float pointSize = 0.4f * fSize;
    DrawSymbol(hdc, kDotSymbol, pointOffsetx, pointOffsety1, pointSize);
    DrawSymbol(hdc, kDotSymbol, pointOffsetx, pointOffsety2, pointSize);
}

// --------------------------------------------------------------------------
void GRRepeatEnd::OnDraw( VGDevice & hdc ) const
{
    if (!mDraw || fSize < kMinNoteSize)
        return;

    VGColor prevColor = hdc.GetFontColor();
    if (mColRef) {
        hdc.PushFillColor(VGColor(mColRef));
        hdc.SetFontColor(VGColor(mColRef));
    }

	bool systembar = getTagType() == GRTag::SYSTEMTAG;
	// for system tags, draw the bars only once
	if (systembar || !isSystemSlice()) {
		// - Vertical adjustement according to staff's line number
		float offsety1 = (float)(fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
		float offsety2 = 0;

		if (fLineNumber != 0 && fLineNumber != 1)
			offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

		// - Horizontal adjustement according to staff's lines size and staff's size
		const float offsetX = (fStaffThickness - 4) * 0.5f - 48 * (fSize - 1) + (fStaffThickness - 4) * (fSize - 1) * 0.5f + 49;

		float leftLineThickness = 1.8f * kLineThick * fSize;
		const float spacing = LSPACE * 0.4f * fSize;
		const float x1 = mPosition.x - mBoundingBox.Width() + offsetX;
		const float x2 = x1 + spacing;

		if (fRanges.empty()) {
			const float y1 = mPosition.y + offsety1 * fSize;
			const float y2 = y1 + (mBoundingBox.bottom + offsety2) * fSize;
			hdc.Rectangle(x1, y1, x1 + leftLineThickness, y2);
			hdc.Rectangle(x2, y1, x2 + fBaseThickness, y2);
		}
		else
		for (size_t i=0; i< fRanges.size(); i++) {
			const float y1 = fRanges[i].first + offsety1 * fSize;
			const float y2 = fRanges[i].second + offsety2 * fSize;
			hdc.Rectangle(x1, y1, x1 + leftLineThickness, y2);
			hdc.Rectangle(x2, y1, x2 + fBaseThickness, y2);
		}
	}
	if (!systembar) DrawDots( hdc);
    if (mColRef) {
        hdc.SetFontColor(prevColor);
        hdc.PopFillColor();
    }
}

// --------------------------------------------------------------------------
unsigned int GRRepeatEnd::getTextAlign() const
{
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase);
}
