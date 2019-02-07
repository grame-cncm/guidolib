/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "GRFingering.h"
#include "ARFingering.h"
#include "GRStaff.h"
#include "GRSingleNote.h"
#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "TagParameterFloat.h"
#include "VGDevice.h"

using namespace std;

extern GRStaff * gCurStaff;

GRFingering::GRFingering(GRStaff * staff, const ARText * ar) : GRText(staff, ar)
{
	mMustFollowPitch = true;
}

// -----------------------------------------------------------------------------
void GRFingering::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
const ARFingering * GRFingering::getARFingering() const
{
	return static_cast<const ARFingering*>(getAbstractRepresentation());
}

// -----------------------------------------------------------------------------
void GRFingering::tellPosition(GObject * caller, const NVPoint & inPosition)
{
	GRSingleNote * note =  dynamic_cast<GRSingleNote *>(caller);
	if( note == 0 ) {
		mDraw = false;
		return;
	}

//cerr << "GRFingering::tellPosition caller 1: " << note << " " << inPosition << endl;

	const GRStaff * staff = note->getGRStaff();
	if( staff == 0 ) return;

	const ARFingering * fing = getARFingering();
	if( fing == 0 ) return;

	mStaffBottom = staff->getStaffBottom();
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	GRNotationElement * startElement = sse->startElement;
	NVPoint newPos( inPosition );

	if (note == startElement)
	{
		float  hspace = staff->getStaffLSPACE() / 2;
		newPos.y = note->getPosition().y;
		st->position = newPos;

		const char* text = fing->getText();
		if (text) st->text = text;
		int placement = fing->getFingeringPosition ();
		float dy = fing->getDY()->getValue( staff->getStaffLSPACE() );
		float dx = fing->getDX()->getValue( staff->getStaffLSPACE() );

		FloatRect r = getTextMetrics (*gGlobalSettings.gDevice, staff);
		NVRect ebb = note->getEnclosingBox(false, false, true);
		NVPoint spos = staff->getPosition();
		float xpos = note->getPosition().x - r.Width()/2 + dx;

		switch (placement) {
			// here positionning takes account of the implicit dy=1 inherited from ARText (see TagParameterStrings.cpp)
			case ARFingering::kAbove:
				setPosition (NVPoint(xpos, min(ebb.top, 0.f) - hspace - r.Height() - dy));
				break;
			case ARFingering::kBelow:
				setPosition (NVPoint(xpos, max(ebb.bottom, staff->getDredgeSize()) - dy));
				break;
			default:
				setPosition (NVPoint(xpos, r.top ));
		}
		
		NVRect bb (0, 0, r.Width(), r.Height());
		mBoundingBox = bb + NVPoint(0,dy);
	}
}
