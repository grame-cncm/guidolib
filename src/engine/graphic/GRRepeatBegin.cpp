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

#include <kf_ivect.h>
#include "ARRepeatBegin.h"

#include "GRRepeatBegin.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRStaff.h"
#include "TagParameterFloat.h"
#include "VGDevice.h"

using namespace std;

NVPoint GRRepeatBegin::refpos;

// --------------------------------------------------------------------------
GRRepeatBegin::GRRepeatBegin(const ARRepeatBegin *ar, GRStaff * inStaff, bool p_ownsar)
					: GRTagARNotationElement(ar, LSPACE, p_ownsar)
{
	setGRStaff(inStaff);
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;
	mSymbol = kRepeatBeginSymbol;
	mLeftSpace = mRightSpace = 0;
	refpos = NVPoint (0, 4 * LSPACE);

	fLinesCount = 5;
    fStaffThickness = 4;
    fSize = 1;
    fBaseThickness = LSPACE * 0.6f;
    fDx = ar->getDX()->getValue();
    fDy = ar->getDY()->getValue();
	updateBoundingBox();
	if (ar->hidden()) mShow = false;
}

// --------------------------------------------------------------------------
const ARBar::TRanges GRRepeatBegin::getRanges() const	{ return getARRepeatBegin()->getRanges(); }

// --------------------------------------------------------------------------
void GRRepeatBegin::setPosFrom(GCoord posy)		{ mBoundingBox.top = posy; }
void GRRepeatBegin::setPosTo(GCoord posy)		{ mBoundingBox.bottom = posy; }

const ARRepeatBegin* GRRepeatBegin::getARRepeatBegin() const {
	return dynamic_cast<const ARRepeatBegin*>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
void GRRepeatBegin::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// --------------------------------------------------------------------------
void GRRepeatBegin::updateBoundingBox()
{
	const float halfExtent = GetSymbolExtent( mSymbol ) * 0.5f;
	float curLSPACE = LSPACE;
	GRStaff *staff = getGRStaff();
	if (staff) {
		curLSPACE   = staff->getStaffLSPACE();
		fLinesCount = staff->getNumlines();
		GRSystemSlice * slice = staff->getGRSystemSlice();
		if (slice && slice->hasSystemBars())
		fStaffThickness = staff->getLineThickness();
		fSize = staff->getSizeRatio();
		fBaseThickness = LSPACE * 0.6f * fSize;
		mTagSize *= fSize;
}

	float topoffset = 0;
	float botoffset = 4 * curLSPACE;
	if ((fLinesCount > 1) && staff) botoffset = staff->getDredgeSize();
	else {
		topoffset = botoffset = 2 * curLSPACE;
	}

	mBoundingBox.top = -topoffset;
	mBoundingBox.left = -halfExtent;
	mBoundingBox.right = halfExtent;
	mBoundingBox.bottom = botoffset; //curLSPACE * ((fLinesCount > 5) ? (fLinesCount-1) : 4);
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
float GRRepeatBegin::getXOffset() const
{
	// obscure computation, should be revised !!!
	float v = (fStaffThickness - 4) * 0.5f;
	return 40 - 30 * (fSize - 1) + v * fSize  - mBoundingBox.Width();  // ????????
}

// --------------------------------------------------------------------------
void GRRepeatBegin::DrawDots( VGDevice & hdc ) const
{
	float hlspace = LSPACE * fSize * 0.5f;
	float symh = 2;
	float y1 = hlspace * (fLinesCount - 5) - symh;
	if (! (fLinesCount % 2)) y1 += hlspace;
	float y2 = y1 - symh + LSPACE * fSize;
    float pointSize = 0.4f * fSize;
	
	if (fLinesCount == 2) {
		pointSize = 0.3f * fSize;
		y1 += 9 * fSize;
		y2 = y1 + 20 * fSize;
	}

    float x  = getXOffset() + hlspace * 2.4f;
    const VGFont* savedfont = hdc.GetMusicFont();
    DrawSymbol(hdc, kDotSymbol, x, y1, pointSize);
    DrawSymbol(hdc, kDotSymbol, x, y2, pointSize);
    hdc.SetMusicFont(savedfont);
}

// --------------------------------------------------------------------------
void GRRepeatBegin::OnDraw(VGDevice & hdc ) const
{
    if (!mDraw || !mShow || fSize < kMinNoteSize)
		return;

    VGColor prevColor = hdc.GetFontColor();
    if (mColRef) {
        hdc.PushFillColor(VGColor(mColRef));
        hdc.SetFontColor(VGColor(mColRef));
    }

	bool systembar = getTagType() == GRTag::SYSTEMTAG;
	if (systembar || !isSystemSlice()) {
		// - Vertical adjustement according to staff's line number
		float offsety1 = (float)(fmod(- 0.5f * fLinesCount - 2, 3) + 1.5f) * LSPACE;
		float offsety2 = 0;

		if (fLinesCount != 0 && fLinesCount != 1)
			offsety2 = ((fLinesCount - 5) % 6) * LSPACE;

		float rightLineThickness = 1.8f * kLineThick * fSize;

		// - Horizontal adjustement according to staff's lines size and staff's size
//		const float offsetX = 0.5f * (fStaffThickness - 4) - 30 * (fSize - 1) + (fSize - 1) * (fStaffThickness - 4) * 0.5f + 40;

		const float spacing = fBaseThickness + LSPACE * 0.4f * fSize - rightLineThickness;
		const float x1 = mPosition.x  + getXOffset() + fDx;
		const float x2 = x1 + spacing;

		float y = 0;
		GRStaff *staff = getGRStaff();
// -----------------------------------------------
// this is an ugly hack to fix incorrect bar length (didn't found another solution yet)
		GRSystemSlice * slice = staff->getGRSystemSlice();
		if (slice && slice->hasSystemBars()) {
			const StaffVector* staves = slice->getGRSystem()->getStaves();
			int n = staves->size();
			for( int i = staves->GetMinimum(); i <= staves->GetMaximum(); ++i) {
				const GRStaff* tmp = staves->Get(i);
				float bottom = tmp ? (tmp->getPosition().y + tmp->getDredgeSize()) : 0;
				if (bottom > y) y = bottom;
			}
		}
// -----------------------------------------------

		if (fRanges.empty()) {
			const float y1 = mPosition.y + mBoundingBox.top - fDy; // * fSize;
			const float y2 = y ? y : mPosition.y + mBoundingBox.bottom  - fDy; // * fSize;
			hdc.Rectangle(x1, y1, x1 + fBaseThickness, y2);
			hdc.Rectangle(x2, y1, x2 + rightLineThickness, y2);
		}
		else for (size_t i=0; i< fRanges.size(); i++) {
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
