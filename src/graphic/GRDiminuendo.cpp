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



