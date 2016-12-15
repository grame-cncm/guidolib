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

#include <iostream>

#include "ARMark.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

using namespace std;

static const char* kNoneStr		= "none";
static const char* kSquareStr	= "square";
static const char* kRectangleStr= "rectangle";
static const char* kOvalStr		= "oval";
static const char* kCircleStr	= "circle";
static const char* kBracketStr	= "bracket";
static const char* kTriangleStr	= "triangle";
static const char* kDiamondStr	= "diamond";

ListOfTPLs				ARMark::ltpls(1);
map<std::string, int>	ARMark::fEnclosureShapes;

ARMark::ARMark() : fEnclosure(kNoEnclosure)
{
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

ARMark::~ARMark()
{
}

void ARMark::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,text,,r;S,enclosure,none,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		if (ret==0) {
			text = TagParameterString::cast(rtpl->RemoveHead());
			assert(text);

            TagParameterString *s = TagParameterString::cast(rtpl->RemoveHead());
			assert(s);
			if (s->TagIsSet()) {
				string value = s->getValue();
				fEnclosure = fEnclosureShapes[value];
			}
            delete s;
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARMark::printName(std::ostream& os) const
{
    os << "ARMark";
}

void ARMark::printGMNName(std::ostream& os) const
{
    os << "\\mark";
}

void ARMark::printParameters(std::ostream& os) const
{
    if (text)
        os << "text: \"" << text->getValue() << "\"; ";

    ARMusicalTag::printParameters(os);
}
