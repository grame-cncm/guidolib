/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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


/** \bug The end bar height is wrong when the staff has more or less than 5 lines.

	 // TODO: handle staff line count != 5
*/

#include "ARFinishBar.h"
#include "GRFinishBar.h"
#include "GuidoDefs.h"
#include "GRStaff.h"
#include "VGDevice.h"

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

	const float spacing = LSPACE * 0.4f;
	const float x1 = mPosition.x - mBoundingBox.Width() + 5;
	const float x2 = x1 + spacing;
	const float y1 = mPosition.y;
	const float y2 = y1 + mBoundingBox.bottom;

	hdc.PushPenWidth( kLineThick * 1.8f );
	hdc.Line( x1, y1, x1, y2 );
	hdc.Rectangle( x2, y1, x2+mThickness, y2 );
	hdc.PopPenWidth();
}

ARFinishBar * GRFinishBar::getARFinishBar()
{
	return static_cast/*dynamic cast*/<ARFinishBar *>(mAbstractRepresentation);
}
