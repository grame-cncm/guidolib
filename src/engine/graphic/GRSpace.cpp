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

#include <cassert>
#include "ARSpace.h"
#include "GRSpace.h"

GRSpace::GRSpace( const ARSpace * arspc, bool p_ownsar )
					: GRARNotationElement(arspc,p_ownsar)
{
	mBoundingBox.left = 0;
	// no, grspace does not need a spring
	mNeedsSpring = 0;
	mBoundingBox.bottom = 0;
	mBoundingBox.top  = 0;
}

void GRSpace::OnDraw( VGDevice & hdc) const
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



