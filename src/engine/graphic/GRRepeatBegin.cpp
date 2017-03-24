/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <math.h>

#include "ARRepeatBegin.h"

#include "GRRepeatBegin.h"
#include "GRSystemSlice.h"
#include "GRStaff.h"
#include "VGDevice.h"

using namespace std;

NVPoint GRRepeatBegin::refpos;

// --------------------------------------------------------------------------
GRRepeatBegin::GRRepeatBegin(ARRepeatBegin *arrb, bool p_ownsar)
					: GRTagARNotationElement(arrb, LSPACE, p_ownsar) 
//GRRepeatBegin::GRRepeatBegin( ARRepeatBegin * arrb, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos )
//					: GRBar(arrb, inStaff, inTimePos) 
{
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;
	mSymbol = kRepeatBeginSymbol;
	mLeftSpace = mRightSpace = 0;
	refpos = NVPoint (0, 4 * LSPACE);

    fLineNumber = 5;
    fStaffThickness = 4;
    fSize = 1;
    fBaseThickness = LSPACE * 0.6f;
	updateBoundingBox();
}

// --------------------------------------------------------------------------
GRRepeatBegin::~GRRepeatBegin()
{
}

// --------------------------------------------------------------------------
const ARBar::TRanges GRRepeatBegin::getRanges() const	{ return getARRepeatBegin()->getRanges(); }

// --------------------------------------------------------------------------
void GRRepeatBegin::setPosFrom(GCoord posy)		{ mBoundingBox.top = posy; }
void GRRepeatBegin::setPosTo(GCoord posy)		{ mBoundingBox.bottom = posy; }

ARRepeatBegin* GRRepeatBegin::getARRepeatBegin() const {
	return dynamic_cast<ARRepeatBegin*>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
void GRRepeatBegin::updateBoundingBox()
{
	const float halfExtent = GetSymbolExtent(mSymbol) * 0.5f;

	mBoundingBox.top    = 0;
	mBoundingBox.left   = -halfExtent;
	mBoundingBox.right  = halfExtent;
	mBoundingBox.bottom = 4 * LSPACE;

    GRStaff *staff = getGRStaff();

    if (staff)
    {
        fLineNumber = staff->getNumlines();

        int linesOffset = fLineNumber - 5;

        if (linesOffset)
            mPosition.y += staff->getStaffLSPACE() * linesOffset / 2;

        fStaffThickness = staff->getLineThickness();
        fSize = staff->getSizeRatio();
        fBaseThickness = LSPACE * 0.6f * fSize;

        mTagSize *= fSize;
    }
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRRepeatBegin::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoBar || sel == kGuidoBarAndEvent)
		SendMap (f, getRelativeTimePosition(), getDuration(), kRepeatBegin, infos);
}

// --------------------------------------------------------------------------
void GRRepeatBegin::setHPosition( float nx )
{
	GRTagARNotationElement::setHPosition(nx);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
void GRRepeatBegin::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	setPosition (newPosition);
	GRTagARNotationElement::tellPosition(caller, newPosition);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
bool GRRepeatBegin::isSystemSlice() const
{
	GRSystemSlice * systemslice = mGrStaff ? mGrStaff->getGRSystemSlice() : 0;
	return systemslice ? systemslice->hasSystemBars() : true;
}

// --------------------------------------------------------------------------
void GRRepeatBegin::DrawDots( VGDevice & hdc ) const
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

    int   pointSymbol = 220;
    float pointOffsety1 = - 5 * fSize + offsety1AccordingToLineNumber;
    float pointOffsety2 = pointOffsety1 + LSPACE * fSize + offsety2AccordingToLineNumber;
    float pointOffsetx = 28 * (fSize - 1) + 0.5f * (fStaffThickness - 4) + (fSize - 1) * (fStaffThickness - 4) * 0.5f + 8;
    float pointSize = 0.4f * fSize;

    DrawSymbol(hdc, pointSymbol, pointOffsetx, pointOffsety1, pointSize);
    DrawSymbol(hdc, pointSymbol, pointOffsetx, pointOffsety2, pointSize);
}

// --------------------------------------------------------------------------
void GRRepeatBegin::OnDraw(VGDevice & hdc ) const
{
    if (!mDraw || fSize < kMinNoteSize)
		return;

    VGColor prevColor = hdc.GetFontColor();
    if (mColRef) {
        hdc.PushFillColor(VGColor(mColRef));
        hdc.SetFontColor(VGColor(mColRef));
    }

	bool systembar = getTagType() == GRTag::SYSTEMTAG;
	if (systembar || !isSystemSlice()) {
		// - Vertical adjustement according to staff's line number
		float offsety1 = (float)(fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
		float offsety2 = 0;

		if (fLineNumber != 0 && fLineNumber != 1)
			offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

		float rightLineThickness = 1.8f * kLineThick * fSize;

		// - Horizontal adjustement according to staff's lines size and staff's size
		const float offsetX = 0.5f * (fStaffThickness - 4) - 30 * (fSize - 1) + (fSize - 1) * (fStaffThickness - 4) * 0.5f + 40;

		const float spacing = fBaseThickness + LSPACE * 0.4f * fSize - rightLineThickness;
		const float x1 = mPosition.x - mBoundingBox.Width() + offsetX;
		const float x2 = x1 + spacing;

		if (fRanges.empty()) {
			const float y1 = mPosition.y + offsety1 * fSize;
			const float y2 = y1 + (mBoundingBox.bottom + offsety2) * fSize;
			hdc.Rectangle(x1, y1, x1 + fBaseThickness, y2);
			hdc.Rectangle(x2, y1, x2 + rightLineThickness, y2);
		}
		else
		for (size_t i=0; i< fRanges.size(); i++) {
			float y1 = fRanges[i].first + offsety1 * fSize;
			float y2 = fRanges[i].second + offsety2 * fSize;
			hdc.Rectangle(x1, y1, x1 + fBaseThickness, y2);
			hdc.Rectangle(x2, y1, x2 + rightLineThickness, y2);
		}
	}

	if (!systembar) DrawDots( hdc);
    if (mColRef) {
        hdc.SetFontColor(prevColor);
        hdc.PopFillColor();
    }
}

// --------------------------------------------------------------------------
unsigned int GRRepeatBegin::getTextAlign() const
{
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase);
}
