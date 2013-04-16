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
