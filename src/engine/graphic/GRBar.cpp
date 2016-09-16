/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <sstream>
#include <string.h>

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

#include "ARBar.h"

#include "GRStaffManager.h"
#include "GRSystem.h"
#include "GRPage.h"
#include "ARMeter.h"
#include "GRBar.h"
#include "GRStaff.h"
#include "GRDoubleBar.h"
#include "GRSystemSlice.h"
#include "VGDevice.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "math.h"

#include "FontManager.h"

using namespace std;

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		std::cout << (void*)this << " GRBar::" << method << std::endl
#else
#define traceMethod(method)	
#endif
#define trace1Method(method)	std::cout << (void*)this << " GRBar::" << method << std::endl

NVPoint GRBar::sRefPos;
GRPage*	GRBar::fCurrentPage = 0;

// --------------------------------------------------------------------------
GRBar::GRBar(ARBar * p_arbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float propRender )
	: GRTagARNotationElement(p_arbar,inStaff->getStaffLSPACE()), proportionalRender(propRender)
{
	mGrStaff = inStaff;
	mSymbol = kBarSymbol;
	mNeedsSpring = 1;
	setTagType(GRTag::STAFFTAG);
	sconst = SCONST_BAR;
    mStaffRatio = 1.0f;
	InitGRBar( inTimePos, inStaff );

    fLineNumber = inStaff->getNumlines();
}

// --------------------------------------------------------------------------
// this constructor uses a GRSystem (it is a system (or accolade)-bar)
GRBar::GRBar(ARBar * p_arbar, GRSystem * , GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float propRender  )
	: GRTagARNotationElement(p_arbar,inStaff->getStaffLSPACE()), proportionalRender(propRender)
{
	mGrStaff = inStaff;
//	mGrStaff = NULL; //  inStaff // why is grstaff needed?
	// does not need a spring on its own (it is attached to another
	// (sometimes invisible) bar that is connected to a staff, and
	// that one has a spring already)
	mSymbol = kBarSymbol;
	mNeedsSpring = 0;
	setTagType(GRTag::SYSTEMTAG);
	InitGRBar( inTimePos, inStaff );
//	GRSystemSlice * systemslice = mGrStaff ? mGrStaff->getGRSystemSlice() : 0;
//	if (systemslice) {
//        StaffVector * staffs = systemslice->getStaves();
//		if (staffs) {
//			GRStaff * first = staffs->Get(staffs->GetMinimum());
//			GRStaff * last = staffs->Get(staffs->GetMaximum());
//			if (first && last) {
//				mBoundingBox.top = first.getBoundingBox().top;
//				mBoundingBox.bottom = last.getBoundingBox().bottom;
//			}
//		}
//	}

    fLineNumber = inStaff->getNumlines();
}

// --------------------------------------------------------------------------
void GRBar::InitGRBar( const TYPE_TIMEPOSITION & inTimePos, const GRStaff * inStaff )
{
	traceMethod("InitGRBar");
	mRelativeTimePositionOfGR = inTimePos;

	const float halfExtent = GetSymbolExtent( mSymbol ) * 0.5f; 
	const float curLSPACE = inStaff->getStaffLSPACE();

	// - Setup the bouding box
	// this is the space left and right of a barline ...
	// not sure, wether this should be done here?
	mBoundingBox.top = 0;
	mBoundingBox.left = -halfExtent;
	mBoundingBox.right = halfExtent;
	mBoundingBox.bottom = curLSPACE * 4;

	if (inStaff) {
		mStaffRatio = inStaff->getSizeRatio();
		int linesOffset = inStaff->getNumlines() - 5;

		if (linesOffset)
			mPosition.y += curLSPACE * linesOffset / 2;
	}
	
	// - Setup left and right spaces	
	mLeftSpace =  (GCoord)((curLSPACE * 0.5f > halfExtent ? curLSPACE * 0.5f : halfExtent) * mTagSize * mStaffRatio);
	mRightSpace = (GCoord)(halfExtent * mTagSize * mStaffRatio);

	sRefPos.y = (GCoord)(4 * LSPACE);
	sRefPos.x = (GCoord)(- halfExtent);
    
    // - Get the x/y offset
	const TagParameterFloat *paramDx = getARBar()->getDX();
	const TagParameterFloat *paramDy = getARBar()->getDY();

	mDx = (paramDx && paramDx->TagIsSet() ? paramDx->getValue() : 0);
	mDy = (paramDy && paramDy->TagIsSet() ? paramDy->getValue() : 0);    
}

// --------------------------------------------------------------------------
GRBar::~GRBar()
{
}

// --------------------------------------------------------------------------
void GRBar::GGSOutput() const
{
	// this is a temporary change for Font stuff ...
	if( mSymbol == kBarSymbol )
		GGSOutputAt( kBarGSSymbol );	// kBarGSSymbol ?
	else
		GRTagARNotationElement::GGSOutput();
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRBar::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoBar)
		SendMap (f, getRelativeTimePosition(), getDuration(), kBar, infos);
}


// --------------------------------------------------------------------------
void GRBar::DrawWithGlyphs( VGDevice & hdc ) const
{
	const VGColor prevTextColor = hdc.GetFontColor();
    if (mColRef)
        hdc.SetFontColor(VGColor(mColRef));

	if (getTagType() != GRTag::SYSTEMTAG)
		GRTagARNotationElement::OnDraw(hdc);
	else {		// this is a system bar
		const float curLSPACE   = LSPACE;
		const float glyphHeight = 4 * curLSPACE;  // - The barline is 4 * currLSpace high
		
		bool  finished = false;
		float posy     = mBoundingBox.top;

		while (!finished && ( posy < mBoundingBox.bottom)) {
			if ((posy + glyphHeight) > (mBoundingBox.bottom)) {
				posy = (mBoundingBox.bottom - glyphHeight);
				finished = true;
			}

			OnDrawSymbol(hdc, mSymbol, 0, posy);
			posy += glyphHeight * 0.75f;	// 0.75 for test (was: 1, symbol did not connect each others)
		}
	}
    
	if (mColRef)
        hdc.SetFontColor(prevTextColor);
}

// --------------------------------------------------------------------------
bool GRBar::isSystemSlice() const
{
	GRSystemSlice * systemslice = mGrStaff ? mGrStaff->getGRSystemSlice() : 0;
	return systemslice ? systemslice->hasSystemBars() : true;
}

// --------------------------------------------------------------------------
void GRBar::DrawWithLines( VGDevice & hdc ) const
{
	if ((getTagType() != GRTag::SYSTEMTAG) && isSystemSlice())
		return;			// don't draw staff bars on system slices
    
    VGColor prevFontColor = hdc.GetFontColor();

    if (mColRef) {
        hdc.SetFontColor(VGColor(mColRef));
        hdc.PushPenColor(VGColor(mColRef));
    }

    const float staffSize = mGrStaff->getSizeRatio();

    if (staffSize < kMinNoteSize) // Too small, don't draw
        return;

    ARBar *arBar = getARBar();
	bool pageNumbering = false;
	if (gCurStaff) {
		//check for new page
		GRSystem* system = gCurStaff->getGRSystem();
		if (system) {
			GRPage* page = system->getGRPage();
			bool newPage = !fCurrentPage || (page != fCurrentPage);
			fCurrentPage = page;
			pageNumbering = newPage  && (arBar->getMeasureNumberDisplayed() == ARBar::kNumPage) && (arBar->getMeasureNumber() != 2);
		}
	}
	
    if (arBar->getMeasureNumberDisplayed() && arBar->getMeasureNumber() != 0) {
        const NVstring fontName("Arial");
        string attr("");
        const VGFont* hmyfont = FontManager::FindOrCreateFont((int) (80.0f * mTagSize), &fontName, &attr);
		hdc.SetTextFont(hmyfont);

		int num = arBar->getMeasureNumber();
		string barNumberString;
		if (pageNumbering) {
			const ARBar *lastbar = arBar->previousBar();
			float measureNumDxOffset = lastbar ? lastbar->getMeasureNumberDxOffset() : 0;
			float measureNumDyOffset = lastbar ? -lastbar->getMeasureNumberDyOffset() : 0;
			float x = gCurStaff->getBoundingBox().left + ( 10 * mTagSize) + measureNumDxOffset;
			float y = mPosition.y - 40 - 110 * (mTagSize - 1) - mDy + measureNumDyOffset;
			ostringstream barNumberStream;
			barNumberStream << num - 1;
			barNumberString = barNumberStream.str();
			hdc.DrawString(x, y, barNumberString.c_str(), barNumberString.size());
		}

		ostringstream barNumberStream;
		barNumberStream << num;
		barNumberString = barNumberStream.str();

		// Prendre en compte la staffSize ?
		float measureNumDxOffset =   arBar->getMeasureNumberDxOffset();
		float measureNumDyOffset = - arBar->getMeasureNumberDyOffset();

        float totalXOffset = mPosition.x - 18  - 20 * (mTagSize - 1) + measureNumDxOffset + mDx;
        float totalYOffset = mPosition.y - 40 - 110 * (mTagSize - 1) + measureNumDyOffset - mDy;

		if (arBar->getMeasureNumberDisplayed() != ARBar::kNumPage)
			hdc.DrawString(totalXOffset, totalYOffset, barNumberString.c_str(), barNumberString.size());
	}

    // - Vertical adjustement according to staff's line number
    float offsety1 = (fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
    float offsety2 = 0;

    if (fLineNumber != 0 && fLineNumber != 1)
        offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

    const float offsetX = 3 + (staffSize - 1) * 2;

    float proportionalRenderingXOffset = 0;
	if (this->proportionalRender != 0)
        proportionalRenderingXOffset = - 90; // REM: 30 hardcoded (1.5 * note extent)
    
    const float x  = mPosition.x + offsetX + mDx + proportionalRenderingXOffset;
	float y1 = mPosition.y + mBoundingBox.top + offsety1 * staffSize - mDy;
	float y2 = y1 + mBoundingBox.bottom + offsety2 * staffSize;
    float barLength = y2 - y1;
    y1 -= barLength / 2 * (mTagSize - 1);
    y2 += barLength / 2 * (mTagSize - 1);

    hdc.PushPenWidth(mGrStaff ? mGrStaff->currentLineThikness() * staffSize * mTagSize : kLineThick * staffSize * mTagSize);
    hdc.Line(x, y1, x, y2);
    hdc.PopPenWidth();

    if (mColRef) {
        hdc.SetFontColor(prevFontColor);
        hdc.PopPenColor();
    }
}

// --------------------------------------------------------------------------
// - TODO: draw only one scaled symbol instead of repeating one symbol.
void GRBar::OnDraw(VGDevice & hdc ) const
{
	if(!mDraw)
		return;
	traceMethod("OnDraw");
#if 1
	DrawWithLines (hdc);
#else
	DrawWithGlyphs (hdc, map);
#endif
	
//	Map (hdc, map);
	if (gBoundingBoxesMap & kMeasureBB)
		DrawBoundingBox(hdc, kMeasureBBColor);
}

// --------------------------------------------------------------------------
ARBar * GRBar::getARBar() const
{
	return /*dynamic*/static_cast<ARBar *>(getAbstractRepresentation());
}

// --------------------------------------------------------------------------
/** \brief Tell the bar on which staff it is. Recalculate the height of
	the bounding box
*/
void GRBar::setGRStaff(GRStaff * inStaff)
{
	traceMethod("setGRStaff");
	assert( inStaff );
	if (mGrStaff == NULL)
		mGrStaff = inStaff;

//	mBoundingBox.bottom = mGrStaff->getDredgeSize();
	GRARNotationElement::setGRStaff( inStaff );
}

// --------------------------------------------------------------------------
/** \brief Sets the starting position of the bar (vertically)
*/
void GRBar::setPosFrom(GCoord posy)	
{ 
	traceMethod("setPosFrom");
	mBoundingBox.top = posy; 
}

// --------------------------------------------------------------------------
/** \brief Sets the ending position of the bar (vertically)
*/
void GRBar::setPosTo(GCoord posy) 
{
	traceMethod("setPosTo");
	mBoundingBox.bottom = posy; 
}

// --------------------------------------------------------------------------
void GRBar::setHPosition( float nx )
{
	traceMethod("setHPosition");
	GRTagARNotationElement::setHPosition (nx);
	mMapping = mBoundingBox;
	mMapping.left -= LSPACE/6;
	mMapping.right += LSPACE/6;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
void GRBar::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	traceMethod("tellPosition");
	// if this is called, I know that the "staff"-GRBar has been moved ...
	mPosition.x = newPosition.x;
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
/** \brief Compare only the relative time positions.
*/
bool GRBar::operator==(const GRBar & bar) const
{
	return (bar.mRelativeTimePositionOfGR == mRelativeTimePositionOfGR);
}

bool GRBar::operator==(const GRTag & tag) const
{
	const GRBar * bar = dynamic_cast<const GRBar *>(&tag);
	if (bar)
		return this->operator==(*bar);
	return false;
}
