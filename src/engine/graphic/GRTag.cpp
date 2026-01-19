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

#include "ARMusicalTag.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "GRDefine.h"
#include "GRTag.h"
#include "GRStaff.h"
#include "GRNotationElement.h"

GRTag::GRTag( const ARMusicalTag * artag, float curLSPACE )
	: isautotag(0), sconst(SCONST_DEFAULT)
{
	if (artag) {
		if (artag->IsStateTag())
			fTagType = STAFFTAG;
		if (artag->getIsAuto())
			isautotag = 1;
		if (artag->getError())
			fError = artag->getError();
	}
	mColRef = NULL;
	fFontSize = (int) (1.5f * LSPACE); // default ...
	
	if (artag) {
		const TagParameterString *color = artag->getColor();
		if (color) {
			mColRef = new unsigned char [4];
			color->getRGB(mColRef);
		}

		const TagParameterFloat * dx = artag->getDX();
		const TagParameterFloat * dy = artag->getDY();
		if (dx)	{
			if (dx->isDuration()) {
				fHasDurationDx = true;
				fDxDuration = dx->getDuration();
			}
			else mTagOffset.x = (GCoord) dx->getValue(curLSPACE);
		}
		if (dy)	mTagOffset.y -= (GCoord) dy->getValue(curLSPACE);

		const TagParameterFloat * tps = artag->getSize();
		mTagSize = tps ? tps->getValue() : 1.0f;
	}
}

GRTag::~GRTag()
{
	delete [] mColRef;
    mColRef = 0;
}

void GRTag::RangeEnd(GRStaff * grstaff)			{}
void GRTag::StaffFinished(GRStaff * grstaff)	{}
void GRTag::StaffBegin(GRStaff * grstaff)		{}

// changed: the abstract representation is no longer notified ....
// errors are not brought back from graphics ...
bool GRTag::IsStateTag() const			{ return (fTagType == STAFFTAG); }
int  GRTag::getIsAuto() const			{ return isautotag; }
bool GRTag::operator==(const GRTag & tag) const	{ return false; }

void GRTag::applyDurationDx(GRStaff * grstaff)
{
	if (!fHasDurationDx || fDurationDxApplied || !grstaff)
		return;

	GRNotationElement * ne = dynamic_cast<GRNotationElement*>(this);
	if (!ne) return;

	TYPE_TIMEPOSITION baseTime = ne->getRelativeTimePosition();
	TYPE_TIMEPOSITION measStart, measEnd;
	grstaff->getMeasureBounds(baseTime, measStart, measEnd);

	// derive spatial offset from time offset
	const TYPE_TIMEPOSITION targetTime = baseTime + fDxDuration;
	TYPE_TIMEPOSITION targetMeasStart = measStart;
	TYPE_TIMEPOSITION targetMeasEnd   = measEnd;

	// If the target time crosses a barline, compute offsets in the measure where it lands.
	if (targetTime < measStart || targetTime > measEnd) {
		grstaff->getMeasureBounds(targetTime, targetMeasStart, targetMeasEnd);
	}

	const float baseX = grstaff->getXForTime(baseTime);

	float targetX = baseX;
	if (targetTime >= targetMeasStart && targetTime <= targetMeasEnd) {
		targetX = grstaff->getXForTime(targetTime);
	}
	else if (targetMeasEnd != targetMeasStart) {
		const double ratio = double(targetTime - targetMeasStart) /
							 double(targetMeasEnd - targetMeasStart);
		const float targetStartX = grstaff->getXForTime(targetMeasStart);
		const float targetEndX   = grstaff->getXForTime(targetMeasEnd);
		const float targetWidth  = targetEndX - targetStartX;
		targetX = targetStartX + float(ratio * targetWidth);
	}
	const float dx = targetX - baseX;

//	std::cerr << "[duration-dx] baseTime=" << double(baseTime)
//			  << " targetTime=" << double(targetTime)
//			  << " baseMeas=[" << double(measStart) << "," << double(measEnd) << "]"
//			  << " targetMeas=[" << double(targetMeasStart) << "," << double(targetMeasEnd) << "]"
//			  << " baseX=" << baseX << " targetX=" << targetX
//			  << " dx=" << dx << std::endl;

	mTagOffset.x += dx;

	fDurationDxApplied = true;
}
