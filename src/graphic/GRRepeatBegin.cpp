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

#include "ARRepeatBegin.h"
#include "GRRepeatBegin.h"
#include "GUIDOInternal.h"

#include "VGDevice.h"
#include "GRStaff.h"

#include <iostream>
using namespace std;

class GRStaff;

NVPoint GRRepeatBegin::refpos;

// --------------------------------------------------------------------------
GRRepeatBegin::GRRepeatBegin(ARRepeatBegin * arrb, bool p_ownsar)
					: GRTagARNotationElement(arrb, LSPACE, p_ownsar) 
//GRRepeatBegin::GRRepeatBegin( ARRepeatBegin * arrb, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos )
//					: GRBar(arrb, inStaff, inTimePos) 
{
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;
	mSymbol = kRepeatBeginSymbol;
	mLeftSpace = mRightSpace = 0;
	refpos = NVPoint (0, 4 * LSPACE );
}

// --------------------------------------------------------------------------
GRRepeatBegin::~GRRepeatBegin()
{
}

// --------------------------------------------------------------------------
void GRRepeatBegin::updateBoundingBox()
{
	const float halfExtent = GetSymbolExtent( mSymbol ) * 0.5f;

	mBoundingBox.top = 0;
	mBoundingBox.left = -halfExtent;
	mBoundingBox.right = halfExtent;
	mBoundingBox.bottom = 4 * LSPACE;

	GRStaff * staff = getGRStaff();
	if (staff) {
		mTagSize *= staff->getSizeRatio();
		int linesOffset = staff->getNumlines() - 5;
		if (linesOffset) {
			mPosition.y += staff->getStaffLSPACE() * linesOffset / 2;
		}
	}
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRRepeatBegin::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoBar)
		SendMap (f, getRelativeTimePosition(), getDuration(), kRepeatBegin, infos);
}

// --------------------------------------------------------------------------
void GRRepeatBegin::setHPosition( float nx )
{
	GRTagARNotationElement::setHPosition( nx);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
void GRRepeatBegin::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRTagARNotationElement::tellPosition(caller, newPosition);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
void GRRepeatBegin::OnDraw(VGDevice & hdc ) const
{
	GRTagARNotationElement::OnDraw (hdc);
	if (gBoundingBoxesMap & kMeasureBB)
		DrawBoundingBox( hdc, kMeasureBBColor);
}


// --------------------------------------------------------------------------
unsigned int GRRepeatBegin::getTextAlign() const
{
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase);
}
