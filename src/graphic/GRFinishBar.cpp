/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


/** \bug The end bar height is wrong when the staff has more or less than 5 lines.

	 // TODO: handle staff line count != 5
*/

#include "ARFinishBar.h"
#include "GRFinishBar.h"
#include "GuidoDefs.h"
#include "GRStaff.h"
#include "VGDevice.h"
#include "ARStaffFormat.h"

#include <iostream>
using namespace std;


NVPoint GRFinishBar::refpos;

GRFinishBar::GRFinishBar( ARFinishBar * p_ardbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & p_tp )
				: GRBar(p_ardbar,inStaff ,p_tp)
{
	mSymbol = kFinishBarSymbol;

	mLeftSpace = 55;	// hardcoded
	mRightSpace = 0;

	refpos.x = -40;		// hardcoded
//	refpos.y = inStaff ? inStaff->getDredgeSize() : (4 * LSPACE);
	refpos.y = 4 * LSPACE;
	updateBoundingBox();

    fLineNumber = inStaff->getNumlines();
    fStaffThickness = inStaff->getLineThickness();
}

GRFinishBar::GRFinishBar(ARFinishBar * p_arbar, GRSystem * p_grsystem, GRStaff * inStaff,
														const TYPE_TIMEPOSITION & p_timeposition )
			: GRBar(p_arbar,p_grsystem,inStaff,p_timeposition)
{
	mSymbol = kFinishBarSymbol;

	mLeftSpace = 55;	// hardcoded
	mRightSpace = 0;

	refpos.x = -40;		// hardcoded
//	refpos.y = inStaff ? inStaff->getDredgeSize() : (4 * LSPACE);
	refpos.y = 4 * LSPACE;
	updateBoundingBox();

    fLineNumber = inStaff->getNumlines();
    fStaffThickness = inStaff->getLineThickness();
}

GRFinishBar::~GRFinishBar()
{
}

// --------------------------------------------------------------------------
void GRFinishBar::updateBoundingBox()
{
	mThickness = LSPACE * 0.6f;
	const float spacing = LSPACE * 0.4f;
	const float extend = (spacing + mThickness);
	mBoundingBox.left  = -extend;
	mBoundingBox.right = 0;
}

// --------------------------------------------------------------------------
void GRFinishBar::DrawWithLines( VGDevice & hdc ) const
{
	if ((getTagType() != GRTag::SYSTEMTAG) && isSystemSlice())
		return;			// don't draw staff bars on system slices

    /* Vertical adjustement according to staff's line number */
    float offsety1 = 0;
    float offsety2 = 0;

    if (fLineNumber == 0)
    {
        offsety1 = - LSPACE / 2;
    }
    else if (fLineNumber == 1)
    {
        offsety1 = - LSPACE;
    }
    else if (fLineNumber == 2)
    {
        offsety1 = LSPACE * (float)1.5;
        offsety2 = - 3 * LSPACE;

    }
    else if (fLineNumber == 3)
    {
        offsety1 = LSPACE;
        offsety2 = - 2 * LSPACE;

    }
    else if (fLineNumber == 4)
    {
        offsety1 = LSPACE / 2;
        offsety2 = - LSPACE;

    }
    else if (fLineNumber == 6)
    {
        offsety1 = - LSPACE / 2;
        offsety2 = LSPACE;

    }
    else if (fLineNumber == 7)
    {
        offsety1 = - LSPACE;
        offsety2 = 2 * LSPACE;
    }
    /*********************************************************/

	const float spacing = LSPACE * 0.4f;
	const float x1 = mPosition.x - mBoundingBox.Width() + 1 + fStaffThickness / 2;
	const float x2 = x1 + spacing;
    const float y1 = mPosition.y + offsety1;
	const float y2 = y1 + mBoundingBox.bottom + offsety2;

	hdc.PushPenWidth( kLineThick * 1.8f );
	hdc.Line( x1, y1 + kLineThick, x1, y2 - kLineThick);
	hdc.Rectangle( x2, y1, x2 + mThickness, y2 );
	hdc.PopPenWidth();
}

ARFinishBar * GRFinishBar::getARFinishBar()
{
	return static_cast/*dynamic cast*/<ARFinishBar *>(mAbstractRepresentation);
}