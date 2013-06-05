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

//#include <iostream>

#include "GRSimpleBeam.h"
#include "GRBeam.h"
#include "VGDevice.h"

GRSimpleBeam::GRSimpleBeam (GRBeam * p_parent, const NVPoint par[4] )
{
	fParent = p_parent;
	fPoints[0] = par[0];
	fPoints[1] = par[1];
	fPoints[2] = par[2];
	fPoints[3] = par[3];
}

void GRSimpleBeam::GGSOutput() const
{
}

void GRSimpleBeam::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw)
		return;
	const unsigned char * colref = getColRef();

	if (colref) {
		VGColor color ( colref ); 	// custom or black
		hdc.PushFillColor( color );
		hdc.PushPen( color, 1 );
	}
	float ax [4] = { fPoints[0].x, fPoints[1].x, fPoints[3].x, fPoints[2].x };
	float ay [4] = { fPoints[0].y, fPoints[1].y, fPoints[3].y, fPoints[2].y };


// DF added to check for incorrect coordinates
// makes sure that the right point is not to the left of the left point :-)
// actually this should be checked at coordinates computation time
// todo: check the object that computes the beam coordinates
	if (ax[0] > ax[2]) { ax[2] = ax[0]; }
	if (ax[1] > ax[3]) { ax[3] = ax[1]; }
	
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
