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

#include "ARRepeatEnd.h"

#include "GRRepeatEnd.h"
#include "GRStaff.h"
#include "VGDevice.h"

using namespace std;

NVPoint GRRepeatEnd::refpos;

// --------------------------------------------------------------------------
GRRepeatEnd::GRRepeatEnd(ARRepeatEnd *arre, bool p_ownsar)
				: GRTagARNotationElement(arre, LSPACE, p_ownsar)
//GRRepeatEnd::GRRepeatEnd( ARRepeatEnd * arre, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos )
//					: GRBar(arre, inStaff, inTimePos) 
{
	mNeedsSpring = 1;
	sconst = SCONST_BAR - 2;
	mSymbol = kRepeatEndSymbol; 
	mLeftSpace =  mRightSpace = 0;
    refpos = NVPoint (0, 4 * LSPACE);

    fLineNumber = 5;
    fStaffThickness = 0.08f;
    fSize = 1;
    fBaseThickness = LSPACE * 0.6f;
}

// --------------------------------------------------------------------------
GRRepeatEnd::~GRRepeatEnd()
{
}

// --------------------------------------------------------------------------
void GRRepeatEnd::updateBoundingBox()
{
	const float halfExtent = GetSymbolExtent(mSymbol) * 0.5f;

	mBoundingBox.top    = 0;
	mBoundingBox.left   = - halfExtent;
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
void GRRepeatEnd::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoBar)
		SendMap (f, getRelativeTimePosition(), getDuration(), kRepeatEnd, infos);
}

// --------------------------------------------------------------------------
void GRRepeatEnd::setHPosition( float nx )
{
	GRTagARNotationElement::setHPosition(nx);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
void GRRepeatEnd::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRTagARNotationElement::tellPosition(caller, newPosition);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// --------------------------------------------------------------------------
void GRRepeatEnd::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw)
		return;

    // - Vertical adjustement according to staff's line number
    float offsety1 = (fmod(- 0.5f * fLineNumber - 2, 3) + 1.5f) * LSPACE;
    float offsety2 = 0;

    if (fLineNumber != 0 && fLineNumber != 1)
        offsety2 = ((fLineNumber - 5) % 6) * LSPACE;

    const float spacing = LSPACE * 0.4f * fSize;
	const float x1 = mPosition.x - mBoundingBox.Width() + 2;
	const float x2 = x1 + spacing;
    const float y1 = mPosition.y + offsety1 * fSize;
	const float y2 = y1 + (mBoundingBox.bottom + offsety2) * fSize;

    float leftLineThickness = 1.8f * kLineThick * fSize;

    hdc.Rectangle(x1, y1, x1 + leftLineThickness, y2);
	hdc.Rectangle(x2, y1, x2 + fBaseThickness, y2);

    /* Two points drawing */
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
    float pointOffsetx = -30 * fSize - 90;
    float pointSize = 0.4f * fSize;

    DrawSymbol(hdc, pointSymbol, pointOffsetx, pointOffsety1, pointSize);
    DrawSymbol(hdc, pointSymbol, pointOffsetx, pointOffsety2, pointSize);
    /**********************/
}

// --------------------------------------------------------------------------
unsigned int GRRepeatEnd::getTextAlign() const
{
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase);
}
