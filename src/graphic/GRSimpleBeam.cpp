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

#include "GRSimpleBeam.h"
#include "GRBeam.h"
#include "VGDevice.h"

GRSimpleBeam::GRSimpleBeam (GRBeam * p_parent, const NVPoint par[4] )
{
	parent = p_parent;
	p[0] = par[0];
	p[1] = par[1];
	p[2] = par[2];
	p[3] = par[3];
}

void GRSimpleBeam::GGSOutput() const
{
}

void GRSimpleBeam::OnDraw( VGDevice & hdc ) const
{
	const unsigned char * colref = getColRef();

	if (colref) {
		VGColor color ( colref ); 	// custom or black
		hdc.PushFillColor( color );
		hdc.PushPen( color, 1 );
	}
	float ax [4] = { p[0].x, p[1].x, p[3].x, p[2].x };
	float ay [4] = { p[0].y, p[1].y, p[3].y, p[2].y };
	
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
	return parent ? parent->getColRef() : 0;
}
