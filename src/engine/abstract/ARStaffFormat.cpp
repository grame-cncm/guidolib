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

#include <iostream>

#include "ARStaffFormat.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

#include "GRDefine.h"
#include "gmntools.h" // for gd_convertUnits


static const TagParameterMap sARStaffFormatMap (kARStaffFormatParams);

ARStaffFormat::ARStaffFormat() : fLineThickness(kLineThick)
{
	setupTagParameters (sARStaffFormatMap);	
}

ARStaffFormat::ARStaffFormat(const ARStaffFormat &stffrmt)
{
	setupTagParameters (sARStaffFormatMap);
	copyParameters(stffrmt.getTagParameters());
	setTagParameters (stffrmt.getTagParameters());
	fSize = stffrmt.fSize;
	fLineThickness = stffrmt.getLineThickness();
}

const TagParameterString * ARStaffFormat::getStyle() const			{ return getParameter<TagParameterString>(kStyleStr); }
const TagParameterFloat * ARStaffFormat::getStaffDistance() const	{ return getParameter<TagParameterFloat>(kDistanceStr); }
const TagParameterFloat * ARStaffFormat::getSize() const
{
	return fSize.TagIsSet() ? &fSize : ARMTParameter::getSize();
}

void ARStaffFormat::setTagParameters (const TagParameterMap& params)
{
	const TagParameterFloat* thickness =  getParameter<TagParameterFloat>(kLineThicknesStr);
	if (thickness) fLineThickness = (LSPACE * thickness->getValue() >= 0 ? LSPACE * thickness->getValue() : 0);
	const TagParameterFloat * size = getParameter<TagParameterFloat>(kSizeStr);
	if (size) {
		// one idea is to adjust the size, so that it matches an integer (internally) per halfspace ...
		// Integer internal units 
		const int val  = (int)(size->getValue() + 0.5);
		const double cmunits = val * kVirtualToCm;

		double result;
		if (gd_convertUnits(cmunits, "cm", size->getUnit(), result)) {
			fSize.setValue ((float)result);
			fSize.setUnit(size->getUnit());
			fSize.setBySet();
		}
	}
}

