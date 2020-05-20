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

NVPoint     GRBar::sRefPos;
GRPage*     GRBar::fCurrentPage = 0;
GRSystem*   GRBar::fCurrentSystem = 0;

// --------------------------------------------------------------------------
GRBar::GRBar(const ARBar * ar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float propRender )
	: GRTagARNotationElement(ar,inStaff->getStaffLSPACE()), mProportionalRender(propRender)
{
	mGrStaff = inStaff;
	mSymbol = kBarSymbol;
	mNeedsSpring = 1;
	setTagType(GRTag::STAFFTAG);
	sconst = SCONST_BAR;
    mStaffRatio = 1.0f;
    fLinesCount = inStaff->getNumlines();
	InitGRBar( inTimePos, inStaff );
	if (ar->hidden()) mShow = false;
}

// --------------------------------------------------------------------------
// this constructor uses a GRSystem (it is a system (or accolade)-bar)
GRBar::GRBar(const ARBar * ar, GRSystem * , GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float propRender  )
	: GRTagARNotationElement(ar,inStaff->getStaffLSPACE()), mProportionalRender(propRender)
{
	mGrStaff = inStaff;
	// does not need a spring on its own (it is attached to another
	// (sometimes invisible) bar that is connected to a staff, and
	// that one has a spring already)
	mSymbol = kBarSymbol;
	mNeedsSpring = 0;
	setTagType(GRTag::SYSTEMTAG);
    fLinesCount = inStaff->getNumlines();
	InitGRBar( inTimePos, inStaff );
	if (ar->hidden()) mShow = false;
}

// -----------------------------------------------------------------------------
void GRBar::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// --------------------------------------------------------------------------
void GRBar::InitGRBar( const TYPE_TIMEPOSITION & inTimePos, const GRStaff * inStaff )
{
	traceMethod("InitGRBar");
	setRelativeTimePosition (inTimePos);

	const float halfExtent = GetSymbolExtent( mSymbol ) * 0.5f; 
	const float curLSPACE = inStaff->getStaffLSPACE();

	// - Setup the bouding box
	// this is the space left and right of a barline ...
	// not sure, wether this should be done here?
	float topoffset = 0;
	float botoffset = 4 * curLSPACE;
	if (fLinesCount > 1) botoffset = inStaff->getDredgeSize();
	else {
		topoffset = botoffset = 2 * curLSPACE;
	}

	mBoundingBox.top = -topoffset;
	mBoundingBox.left = -halfExtent;
	mBoundingBox.right = halfExtent;
	mBoundingBox.bottom = botoffset;

	if (inStaff) mStaffRatio = inStaff->getSizeRatio();

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

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRBar::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoBar || sel == kGuidoBarAndEvent)
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
void GRBar::DisplayMeasureNum( VGDevice & hdc ) const
{
    const ARBar *arBar = getARBar();
    const ARBar *analyzedBar = NULL;
    bool systemNumbering = false;
    bool pageNumbering = false;
    
	if (gCurStaff) {
		//check for new page
		GRSystem* system = gCurStaff->getGRSystem();
        if (system) {
            analyzedBar = arBar->previousBar();
            
			GRPage* page = system->getGRPage();
            bool newPage = !fCurrentPage || (page != fCurrentPage);
            bool newSystem = !fCurrentSystem || (system != fCurrentSystem);
            
            fCurrentPage = page;
            fCurrentSystem = system;
            
            if (analyzedBar != NULL && analyzedBar->getMeasureNumber() != 1) {
            	int disp = analyzedBar->getMeasureNumberDisplayed();
                pageNumbering = newPage && (disp == ARBar::kNumPage);
                systemNumbering = newSystem && ((disp == ARBar::kNumSystem) || (disp == ARBar::kNumAll));
            }
		}
	}

    if ((arBar->getMeasureNumberDisplayed() == ARBar::kNumAll || systemNumbering || pageNumbering) && (arBar->getMeasureNumber() != 0)) {
        const VGFont* hmyfont = FontManager::FindOrCreateFont((int) (80.0f * mTagSize), "Arial", "");
		hdc.SetTextFont(hmyfont);

		int num = arBar->getMeasureNumber();
		string barNumberString;
		if (pageNumbering || systemNumbering) {
            if (analyzedBar->getMeasureNumberDisplayed() != ARBar::kNoNum) {
                // float measureNumDxOffset = analyzedBar ? analyzedBar->getMeasureNumberDxOffset() : 0;
                float measureNumDyOffset = analyzedBar ? -analyzedBar->getMeasureNumberDyOffset() : 0;
                // float x = gCurStaff->getBoundingBox().left + ( 10 * mTagSize) + measureNumDxOffset;
                // float x = mLeftSpace + measureNumDxOffset;
                float x = 0; //measureNumDxOffset;
                float y = mPosition.y - 40 - 110 * (mTagSize - 1) - mDy + measureNumDyOffset;
                ostringstream barNumberStream;
                barNumberStream << num - 1;
                barNumberString = barNumberStream.str();
                int fontalign = hdc.GetFontAlign();
                hdc.SetFontAlign (VGDevice::kAlignCenter + VGDevice::kAlignBase);
                hdc.DrawString(x, y, barNumberString.c_str(), int(barNumberString.size()));
                hdc.SetFontAlign (fontalign);
            }
		}
		ostringstream barNumberStream;
		barNumberStream << num;
		barNumberString = barNumberStream.str();

		// Prendre en compte la staffSize ?
		float measureNumDxOffset =   arBar->getMeasureNumberDxOffset();
		float measureNumDyOffset = - arBar->getMeasureNumberDyOffset();

        float totalXOffset = mPosition.x - 18  - 20 * (mTagSize - 1) + measureNumDxOffset + mDx;
        float totalYOffset = mPosition.y - 40 - 110 * (mTagSize - 1) + measureNumDyOffset - mDy;

		float endgap = fCurrentSystem->getBoundingBox().Width() - getPosition().x;		// used to detect last bar of the system
		if ((arBar->getMeasureNumberDisplayed() == ARBar::kNumAll) && (endgap > 5)) {
			int fontalign = hdc.GetFontAlign();
			hdc.SetFontAlign (0);
			hdc.DrawString	 (totalXOffset, totalYOffset, barNumberString.c_str(), (int)barNumberString.size());
			hdc.SetFontAlign (fontalign);
		}
	}
}

// --------------------------------------------------------------------------
float GRBar::getXPos(float staffSize) const
{
    const float offsetX = 3 + (staffSize - 1) * 2;
    float proportionalRenderingOffset = mProportionalRender ? -90.f : 0.f; // REM: 30 hardcoded (1.5 * note extent)
    return mPosition.x + offsetX + mDx + proportionalRenderingOffset;
}

// --------------------------------------------------------------------------
float GRBar::getY1	(float top) const
{
    // - Vertical adjustement according to staff's lines count
	return mPosition.y + top - mDy;
}

// --------------------------------------------------------------------------
float GRBar::getY2	(float y1, float bottom) const
{
    // - Vertical adjustement according to staff's lines count
	return mPosition.y + bottom - mDy;
}

// --------------------------------------------------------------------------
void GRBar::DrawWithLines( VGDevice & hdc ) const
{
	if ((getTagType() != GRTag::SYSTEMTAG) && isSystemSlice())
		return;			// don't draw staff bars on system slices

    const float staffSize = mGrStaff->getSizeRatio();
    if (staffSize < kMinNoteSize) // Too small, don't draw
        return;
	
    VGColor prevFontColor = hdc.GetFontColor();
    if (mColRef) {
        hdc.SetFontColor(VGColor(mColRef));
        hdc.PushPenColor(VGColor(mColRef));
    }
	DisplayMeasureNum (hdc);

	const float lineThickness = (mGrStaff ? mGrStaff->currentLineThikness() : kLineThick) * staffSize * mTagSize;
	hdc.PushPenWidth(lineThickness);

    const float x  = getXPos(staffSize);
	if (fRanges.empty()) {
		float y1 = getY1(mBoundingBox.top) + lineThickness / 2;
		float y2 = getY2(y1, mBoundingBox.bottom) - lineThickness / 2;
		hdc.Line(x, y1, x, y2);
	}
	else
	for (size_t i=0; i< fRanges.size(); i++) {
		float y1 = getY1 (fRanges[i].first) + lineThickness / 2;
		float y2 = getY2 (-mDy, fRanges[i].second) - lineThickness / 2;
		hdc.Line(x, y1, x, y2);
	}

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
	if(!mDraw || !mShow)
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
const ARBar * GRBar::getARBar() const
{
	return /*dynamic*/static_cast<const ARBar *>(getAbstractRepresentation());
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
//	traceMethod("setPosTo");
//	mBoundingBox.bottom = posy;
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
	return (bar.getRelativeTimePosition() == getRelativeTimePosition());
}

bool GRBar::operator==(const GRTag & tag) const
{
	const GRBar * bar = dynamic_cast<const GRBar *>(&tag);
	if (bar)
		return this->operator==(*bar);
	return false;
}
