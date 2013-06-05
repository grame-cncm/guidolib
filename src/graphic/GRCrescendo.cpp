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

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include "GRDynamics.h"
#include "GRCrescendo.h"
#include "VGDevice.h"

extern GRSystem * gCurSystem;

GRCrescendo::GRCrescendo(GRStaff * pstaff) : GRDynamics(pstaff)
{
}

GRCrescendo::GRCrescendo(GRStaff * pstaff,ARDynamics * abstractRepresentationOfCrescendo)
 : GRDynamics(pstaff,abstractRepresentationOfCrescendo)
{
}

GRCrescendo::~GRCrescendo()
{
}


void GRCrescendo::OnDraw( VGDevice & hdc) const
{
	if(!mDraw)
		return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	assert(sse);

	GRDynamicsSaveStruct * st = (GRDynamicsSaveStruct *) sse->p;
	hdc.Line( st->position.x , st->position.y, st->position.x +  st->boundingBox.right, st->position.y - st->boundingBox.top );
	hdc.Line( st->position.x, st->position.y, st->position.x + st->boundingBox.right, st->position.y + st->boundingBox.bottom );
}



void GRCrescendo::print() const
{
}



