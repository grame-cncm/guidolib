/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

//#include <iostream>

#include "GRSimpleBeam.h"
#include "GRBeam.h"
#include "VGDevice.h"

GRSimpleBeam::GRSimpleBeam (GRBeam * p_parent, const BeamRect& r)
{
	fParent = p_parent;
	fRect = r;
}

void GRSimpleBeam::setPoints( const BeamRect& r )
{
	BeamRect fixed = r;
	if (fRect.topLeft.x > r.topLeft.x) {		 // partial beam anchored to the right
		fixed.topLeft.x 	= fRect.topLeft.x;
		fixed.bottomLeft.x 	= fRect.bottomLeft.x;
		float y = (fRect.topLeft.x - r.topLeft.x) * r.slope();
		fixed.topLeft.y 	+= y;
		fixed.bottomLeft.y 	+= y;
	}
	else if (fRect.topRight.x < r.topRight.x) {	 // partial beam anchored to the left
		fixed.topRight.x 	= fRect.topRight.x;
		fixed.bottomRight.x = fRect.bottomRight.x;
		float y = (fRect.topRight.x - r.topLeft.x) * r.slope();
		fixed.topRight.y 	-= y;
		fixed.bottomRight.y -= y;
	}
	fRect = fixed;
}

void GRSimpleBeam::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow)
		return;
	const unsigned char * colref = getColRef();

	if (colref) {
		VGColor color ( colref ); 	// custom or black
		hdc.PushFillColor( color );
		hdc.PushPen( color, 1 );
	}
	float ax [4], ay[4];
	fRect.xList(ax);
	fRect.yList(ay);


// DF added to check for incorrect coordinates
// makes sure that the right point is not to the left of the left point :-)
// actually this should be checked at coordinates computation time
// todo: check the object that computes the beam coordinates
//	if (ax[0] > ax[2]) { ax[2] = ax[0]; }		// check fRect.topLeft.x > fRect.bottomRight.x
//	if (ax[1] > ax[3]) { ax[3] = ax[1]; }		// check bottomLeft.x > topRight.x
	
	// This does the drawing!
	hdc.Polygon( ax, ay, 4 );

	// - Cleanup
	if (colref) {
		hdc.PopPen();
		hdc.PopFillColor();
	}
}

const unsigned char * GRSimpleBeam::getColRef() const
{
	return fParent ? fParent->getColRef() : 0;
}
