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

#include "ARBarFormat.h"
#include "ARMusicalObject.h"

#include "GRBarFormat.h"
#include "GuidoDefs.h"
//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

GRBarFormat::GRBarFormat(ARBarFormat *par)
: GRTagARNotationElement(/*dynamic cast<ARMusicalObject *>*/(par), LSPACE )
{
	// no Spring here!
	mNeedsSpring = 0;
	mOwnsAR = true;
}

GRBarFormat::~GRBarFormat()
{

}

void GRBarFormat::print() const
{
}

void GRBarFormat::OnDraw( VGDevice & hdc) const
{
}
