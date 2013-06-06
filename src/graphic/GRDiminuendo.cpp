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
// GRDiminuendo

#include "GRDynamics.h"
#include "GRDiminuendo.h"
#include "GRPage.h"
#include "ARDiminuendo.h"
#include "GRVoice.h"
#include "VGDevice.h"


GRDiminuendo::GRDiminuendo(GRStaff *pstaff)
 : GRDynamics(pstaff)
{
}

GRDiminuendo::GRDiminuendo(GRStaff *pstaff,
	ARDiminuendo *abstractRepresentationOfDecresc)
 : GRDynamics(pstaff,
	/*dynamic cast<ARDynamics *>*/ (abstractRepresentationOfDecresc))
{
}

GRDiminuendo::~GRDiminuendo()
{
}


void GRDiminuendo::OnDraw( VGDevice & hdc) const 
{
	if(!mDraw)
		return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	assert(sse);

	GRDynamicsSaveStruct * st = (GRDynamicsSaveStruct *) sse->p;
	if (mColRef) hdc.PushPen( VGColor( mColRef ), 1 );
	hdc.Line( st->position.x , st->position.y - st->boundingBox.top, st->position.x + st->boundingBox.right, st->position.y );
	hdc.Line( st->position.x , st->position.y + st->boundingBox.bottom, st->position.x +  st->boundingBox.right, st->position.y );
	if (mColRef) hdc.PopPen();
}



void GRDiminuendo::print() const
{
}



