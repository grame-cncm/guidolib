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

#include "NEPointerList.h"	// TEMP

#include "GRRepeatEnd.h"

#include "ARRepeatEnd.h"
#include "ARRepeatEndRangeEnd.h"
#include "GUIDOInternal.h"
#include "VGDevice.h"

#include <iostream>
using namespace std;

class GRStaff;

NVPoint GRRepeatEnd::refpos;

// --------------------------------------------------------------------------
/*
GRRepeatEnd::GRRepeatEnd( ARRepeatEnd * ar, bool ownsar )
				: GRTagARNotationElement( ar, LSPACE, ownsar )
//				: GRPTagARNotationElement( ar, ownsar )
*/
GRRepeatEnd::GRRepeatEnd( ARRepeatEnd * arre, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos )
					: GRBar(arre, inStaff, inTimePos) 
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

// --------------------------------------------------------------------------
void GRRepeatEnd::InitRepeatEnd()
{
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;
//	sconst = 5; //SCONST_BAR;

	mSymbol = kRepeatEndSymbol; 
	mLeftSpace =  mRightSpace = 0;
	refpos.Set( -LSPACE * 0.8, 4 * LSPACE );
}

// --------------------------------------------------------------------------
void GRRepeatEnd::setHPosition( float nx )
{
	GRBar::setHPosition (nx);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset() - NVPoint(15,0);
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRRepeatEnd::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
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
void GRRepeatEnd::OnDraw( VGDevice & hdc ) const
{
	DrawWithGlyphs (hdc);
	if (gBoundingBoxesMap & kMeasureBB)
		DrawBoundingBox( hdc, kMeasureBBColor);
}

// --------------------------------------------------------------------------
unsigned int GRRepeatEnd::getTextAlign() const
{
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase);
}
