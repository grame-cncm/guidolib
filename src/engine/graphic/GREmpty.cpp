/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

#include "GREmpty.h"
#include "VGDevice.h"

#include <iostream>
using namespace std;

GREmpty::GREmpty(GRStaff * inStaff, ARMusicalEvent * ar, bool p_ownsAR )
				: GREvent( inStaff, ar,p_ownsAR )
{
	mNeedsSpring = 1;
	mLeftSpace = 0;
	mRightSpace = 0;
}

GREmpty::GREmpty( GRStaff * inStaff, ARMusicalEvent * ar, const TYPE_TIMEPOSITION timePos, const TYPE_DURATION dur )
				: GREvent( inStaff, ar, timePos, dur)
{
	mNeedsSpring = 1;
	mLeftSpace = 0;
	mRightSpace = 0;
}

GREmpty::~GREmpty()	{}

void GREmpty::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent || sel == kGuidoBarAndEvent) {
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
