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

#include <cassert>
#include "ARSpace.h"
#include "GRSpace.h"

GRSpace::GRSpace( ARSpace * arspc, bool p_ownsar )
					: GRARNotationElement(arspc,p_ownsar)
{
	mBoundingBox.left = 0;

//	double tmp = arspc->getValue();

	// no, grspace does not need a spring
	mNeedsSpring = 0;

	mBoundingBox.bottom = 0;
	mBoundingBox.top  = 0;
}

GRSpace::~GRSpace()
{
}

void GRSpace::OnDraw( VGDevice & hdc) const
{
}

void GRSpace::print() const
{
}

const ARSpace * GRSpace::getARSpace() const
{
	return /*dynamic*/static_cast<const ARSpace *>(getAbstractRepresentation());
}

int GRSpace::getStandardSpacing() const
{
	assert(false);
	return 0;
}



