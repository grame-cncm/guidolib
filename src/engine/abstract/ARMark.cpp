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

#include "ARMark.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;

static const char* kNoneStr		= "none";
static const char* kSquareStr	= "square";
static const char* kRectangleStr= "rectangle";
static const char* kOvalStr		= "oval";
static const char* kCircleStr	= "circle";
static const char* kBracketStr	= "bracket";
static const char* kTriangleStr	= "triangle";
static const char* kDiamondStr	= "diamond";

map<std::string, int>	ARMark::fEnclosureShapes;
static const TagParameterMap sARMarkMap (kARMarkParams);

//--------------------------------------------------------------------------
ARMark::ARMark() : fEnclosure(kNoEnclosure)
{
	setupTagParameters (sARMarkMap);
	rangesetting = NO;
	if (!fEnclosureShapes.size()) {
		fEnclosureShapes[kNoneStr]		= kNoEnclosure;
		fEnclosureShapes[kSquareStr]	= kSquare;
		fEnclosureShapes[kRectangleStr] = kRectangle;
		fEnclosureShapes[kOvalStr]		= kOval;
		fEnclosureShapes[kCircleStr]	= kCircle;
		fEnclosureShapes[kBracketStr]	= kBracket;
		fEnclosureShapes[kTriangleStr]	= kTriangle;
		fEnclosureShapes[kDiamondStr]	= kDiamond;
	}
}

//--------------------------------------------------------------------------
void ARMark::setTagParameters (const TagParameterMap& params)
{
	ARText::setTagParameters (params);
    const TagParameterString* p = getParameter<TagParameterString>(kEnclosureStr);
	if (p) fEnclosure = fEnclosureShapes[p->getValue()];
}

