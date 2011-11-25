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

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

#include "GREmpty.h"
#include "VGDevice.h"

#include <iostream>
using namespace std;

GREmpty::GREmpty(GRStaff * inStaff,
					ARMusicalEvent * abstractRepresentationOfEmpty, bool p_ownsAR )
				: GREvent( inStaff, abstractRepresentationOfEmpty,p_ownsAR )
{
	mNeedsSpring = 1;
	mLeftSpace = 0;
	mRightSpace = 0;
}

GREmpty::GREmpty( GRStaff * inStaff, ARMusicalEvent * abstractRepresentationOfEmpty,
				const TYPE_TIMEPOSITION theRelativeTimePositionOfGR, const TYPE_DURATION theDurationOfGR )
				: 
	GREvent( inStaff, abstractRepresentationOfEmpty, theRelativeTimePositionOfGR, theDurationOfGR)
{
	mNeedsSpring = 1;
	mLeftSpace = 0;
	mRightSpace = 0;
}

GREmpty::~GREmpty()
{

}

void GREmpty::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent) {
		SendMap (f, getRelativeTimePosition(), getDuration(), kEmpty, infos);
	}
}

void GREmpty::OnDraw( VGDevice & hdc ) const
{
	if (gBoundingBoxesMap & kEventsBB)
		DrawBoundingBox( hdc, kEventBBColor);
}

void GREmpty::updateBoundingBox()
{
	mBoundingBox.top = LSPACE * 1.5f;
	mBoundingBox.left = -LSPACE * 0.5f;
	mBoundingBox.right = mBoundingBox.left + LSPACE;
	mBoundingBox.bottom = mBoundingBox.top + LSPACE;
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

void GREmpty::tellPosition( GObject * caller, const NVPoint & newPosition )
{
	if (mNeedsSpring == 0 && mSpringID == -1)
		setHPosition(newPosition.x);
}



