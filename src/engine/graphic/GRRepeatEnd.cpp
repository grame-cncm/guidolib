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
GRRepeatEnd::GRRepeatEnd( const ARRepeatEnd * ar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float proportionnalRender )
					: GRBar(ar, inStaff, inTimePos, proportionnalRender)
{
	setGRStaff (inStaff);
	InitRepeatEnd();
	if (ar->hidden()) mShow = false;
}

const ARRepeatEnd* GRRepeatEnd::getARRepeatEnd() {
	return dynamic_cast<const ARRepeatEnd*>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
void GRRepeatEnd::InitRepeatEnd()
{
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;

	mSymbol = kRepeatEndSymbol; 
	mLeftSpace =  mRightSpace = 0;
	refpos.Set( -LSPACE * 0.8f, 4 * LSPACE );
	fLSpace = LSPACE;
	fSize = 1;
	fStaffThickness = 4;

    GRStaff *staff = getGRStaff();
    if (staff)
    {
        fStaffThickness = staff->getLineThickness();
        fSize = staff->getSizeRatio();
        fBaseThickness = LSPACE * 0.6f * fSize;
        fLSpace = staff->getStaffLSPACE();
    }
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
	if (sel == kGuidoBar || sel == kGuidoBarAndEvent)
		SendMap (f, getRelativeTimePosition(), getDuration(), kRepeatEnd, infos);
}

// --------------------------------------------------------------------------
void GRRepeatEnd::updateBoundingBox()
{
//	const float extent = GetSymbolExtent( mSymbol );
//	const float halfExtent = extent * 0.5f;
//
//	mBoundingBox.top = 0;
//	mBoundingBox.left = -halfExtent + refpos.x;
//	mBoundingBox.right = halfExtent + refpos.x;
//	mBoundingBox.bottom = 4 * LSPACE;
}

// --------------------------------------------------------------------------
float GRRepeatEnd::getXOffset() const { return mBoundingBox.Width() / fSize + (LSPACE * 0.75f * fSize); }

// --------------------------------------------------------------------------
void GRRepeatEnd::DrawDots( VGDevice & hdc ) const
{
	float hlspace = LSPACE * fSize * 0.5f;
	float symh = 2; //4;
	float y1 = hlspace * (fLinesCount - 5) - symh;
	if (! (fLinesCount % 2)) y1 += hlspace;
	float y2 = y1 - symh + LSPACE * fSize;
    float pointSize = 0.4f * fSize;

	if (fLinesCount == 2) {
		pointSize = 0.3f * fSize;
		y1 += 9 * fSize;
		y2 = y1 + 20 * fSize;
	}

    float x  = -hlspace * 0.75f - getXOffset();
    const VGFont* savedfont = hdc.GetMusicFont();
    hdc.SetFontAlign(VGDevice::kAlignBaseLeft);
    DrawSymbol(hdc, kDotSymbol, x, y1, pointSize);
    DrawSymbol(hdc, kDotSymbol, x, y2, pointSize);
    hdc.SetMusicFont(savedfont);
}

// --------------------------------------------------------------------------
void GRRepeatEnd::OnDraw( VGDevice & hdc ) const
{
    if (!mDraw || !mShow || fSize < kMinNoteSize)
        return;

    VGColor prevColor = hdc.GetFontColor();
    if (mColRef) {
        hdc.PushFillColor(VGColor(mColRef));
        hdc.SetFontColor(VGColor(mColRef));
    }

	bool systembar = getTagType() == GRTag::SYSTEMTAG;
	// for system tags, draw the bars only once
	if (systembar || !isSystemSlice()) {

		// - Horizontal adjustement according to staff's lines size and staff's size
//		const float offsetX = (fStaffThickness - 4) * 0.5f - 48 * (fSize - 1) + (fStaffThickness - 4) * (fSize - 1) * 0.5f + 49;

		float leftLineThickness = 1.8f * kLineThick * fSize;
		const float spacing = LSPACE * 0.4f * fSize;
		const float x1 = mPosition.x - getXOffset() + mDx;
		const float x2 = x1 + spacing;

		if (fRanges.empty()) {
			const float lthick = fStaffThickness/8;
			const float y1 = getY1 (mBoundingBox.top) - lthick;
			const float y2 = getY2 (y1, mBoundingBox.bottom) + lthick;
			hdc.Rectangle(x1, y1, x1 + leftLineThickness, y2);
			hdc.Rectangle(x2, y1, x2 + fBaseThickness, y2);
		}
		else
		for (size_t i=0; i< fRanges.size(); i++) {
			const float y1 = fRanges[i].first;
			const float y2 = fRanges[i].second;
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
