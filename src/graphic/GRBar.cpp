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

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

#include "ARBar.h"

#include "GRBar.h"
#include "GRStaff.h"
#include "GRDoubleBar.h"
#include "GRSystemSlice.h"
#include "VGDevice.h"

#include <iostream>
using namespace std;

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		std::cout << (void*)this << " GRBar::" << method << std::endl
#else
#define traceMethod(method)	
#endif
#define trace1Method(method)	std::cout << (void*)this << " GRBar::" << method << std::endl

NVPoint GRBar::sRefPos;

// --------------------------------------------------------------------------
GRBar::GRBar(ARBar * p_arbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos )
	: GRTagARNotationElement(p_arbar,inStaff->getStaffLSPACE())
{
	mGrStaff = inStaff;
	mSymbol = kBarSymbol;
	mNeedsSpring = 1;
	setTagType(GRTag::STAFFTAG);
	sconst = SCONST_BAR;
	InitGRBar( inTimePos, inStaff );
}

// --------------------------------------------------------------------------
// this constructor uses a GRSystem (it is a system (or accolade)-bar)
GRBar::GRBar(ARBar * p_arbar, GRSystem * , GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos )
	: GRTagARNotationElement(p_arbar,inStaff->getStaffLSPACE())
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
	if( inStaff )
	{
		mTagSize *= inStaff->getSizeRatio();
		int linesOffset = inStaff->getNumlines() - 5;
		if (linesOffset) {
			mPosition.y += curLSPACE * linesOffset / 2;
		}
	}
	
	// - Setup left and right spaces	
	mLeftSpace =  (GCoord)((curLSPACE * 0.5f > halfExtent ? curLSPACE * 0.5f : halfExtent) * mTagSize);
	mRightSpace = (GCoord)(halfExtent * mTagSize);

	sRefPos.y = (GCoord)(4 * LSPACE);
	sRefPos.x = (GCoord)(- halfExtent);
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
void GRBar::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoBar)
		SendMap (f, getRelativeTimePosition(), getDuration(), kBar, infos);
}


// --------------------------------------------------------------------------
void GRBar::DrawWithGlyphs( VGDevice & hdc ) const
{
//	if (mGrStaff)
	if (getTagType() != GRTag::SYSTEMTAG)
	{
		GRTagARNotationElement::OnDraw(hdc);
	}
	else		// this is a system bar
	{
		float curLSPACE = LSPACE;
		const float glyphHeight = 4 * curLSPACE;  // - The barline is 4 * currLSpace high
		
		bool finished = false;
		float posy = mBoundingBox.top;
		while ( !finished && ( posy < mBoundingBox.bottom ))
		{
			if(( posy + glyphHeight ) > ( mBoundingBox.bottom))
			{
				posy = ( mBoundingBox.bottom - glyphHeight );
				finished = true;
			}
			OnDrawSymbol( hdc, mSymbol, 0, posy );
			posy += glyphHeight * 0.75f;	// 0.75 for test (was: 1, symbol did not connect each others)
		}
	}
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

	const float x = mPosition.x;
	const float y1 = mPosition.y + mBoundingBox.top;
	const float y2 = y1 + mBoundingBox.bottom;
	hdc.PushPenWidth( kLineThick );
	hdc.Line( x, y1, x, y2 );
	hdc.PopPenWidth();
}

// --------------------------------------------------------------------------
// - TODO: draw only one scaled symbol instead of repeating one symbol.
void GRBar::OnDraw(VGDevice & hdc ) const
{
	traceMethod("OnDraw");
#if 1
	DrawWithLines (hdc);
#else
	DrawWithGlyphs (hdc, map);
#endif
	
//	Map (hdc, map);
	if (gBoundingBoxesMap & kMeasureBB)
		DrawBoundingBox( hdc, kMeasureBBColor);
}

// --------------------------------------------------------------------------
void GRBar::print() const
{
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
