/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003	Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
