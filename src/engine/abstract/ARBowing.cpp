/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARBowing.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;


ARBowing::ARBowing()
{
	setupTagParameters (gMaps->sARBowingMap);

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	fDx1 = fDx2 = fDy1 = fDy2 = 0;
	fCtrlPoint = undefined();
	fCtrlPointOffset = undefined();
	fCurveDir = kUndefined;
	fParSet = false;
}

ARBowing::ARBowing(const ARBowing * bowing) : ARMTParameter(-1,bowing)
{
	setupTagParameters (gMaps->sARBowingMap);
	copyParameters(bowing->getTagParameters());
	setTagParameters ( bowing->getTagParameters());

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	fDx1 = bowing->getDX1();
	fDx2 = bowing->getDX2();
	fDy1 = bowing->getDY1();
	fDy2 = bowing->getDY2();
	fCtrlPoint = bowing->getR3();
	fCtrlPointOffset = bowing->getH();
	fCurveDir = bowing->getCurve();
	fParSet = bowing->getParSet();
}


void ARBowing::setTagParameters (const TagParameterMap& params)
{
	const TagParameterFloat* dx1 = getParameter<TagParameterFloat> (kDx1Str);
	const TagParameterFloat* dx2 = getParameter<TagParameterFloat> (kDx2Str);
	const TagParameterFloat* dy1 = getParameter<TagParameterFloat> (kDy1Str);
	const TagParameterFloat* dy2 = getParameter<TagParameterFloat> (kDy2Str);
	const TagParameterFloat* dx = getParameter<TagParameterFloat> (kDxStr);
	const TagParameterFloat* dy = getParameter<TagParameterFloat> (kDyStr);
	fDx1 = dx1 ? dx1->getValue() : 0;
	fDx2 = dx2 ? dx2->getValue() : 0;
	fDy1 = dy1 ? dy1->getValue() : 0;
	fDy2 = dy2 ? dy2->getValue() : 0;
	if (dx) {
		fDx1 += dx->getValue();
		fDx2 += dx->getValue();
	}
	if (dy) {
		fDy1 += dy->getValue();
		fDy2 += dy->getValue();
	}
	fParSet = (dx1 || dx2 || dy1 || dy2 || dx || dy);

	const TagParameterString* dir = getParameter<TagParameterString> (kCurveStr);
	if (dir) {
		std::string down ("down");
		TagParameterFloat pf (true);
		pf.setUnit ("hs");
		if (down == dir->getValue()) {
			fCurveDir = kDown;
			pf.setValue (-2.f);
		}
		else {		// assume an upward curve
			fCurveDir = kUp;
			pf.setValue (2.f);
		}
	}
//	else {
		const TagParameterFloat* r3 = getParameter<TagParameterFloat> (kR3Str);
		const TagParameterFloat* h = getParameter<TagParameterFloat> (kHStr);
		fCtrlPoint = r3 ? r3->getValue() : undefined();
		fCtrlPointOffset = h ? h->getValue() : undefined();
		fParSet |= (r3 || h);
//	}
}
