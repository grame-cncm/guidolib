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
#include <string.h>

#include "ARTuplet.h"
#include "ARBase.h"
#include "FormatStringParser.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

using namespace std;


/*
 the Begin End implementation is currently in progress.
 yet it doesn't work since an auto tuplet is created by the voice manager
*/


//--------------------------------------------------------------------------
ARTuplet::ARTuplet() : fTupletFormat(""), fPosition(0), fDy1(0), fDy2(0), fLineThickness(kDefaultThickness),
    fTextBold(false), fTextSize(1), fDispNote("") /*, fFormatSet(false), fDy1TagIsSet(false), fDy2TagIsSet(false)*/
{
	setupTagParameters (gMaps->sARTupletMap);

	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	fBaseNumerator   = 0;	// 0 means: do not display
	fBaseDenominator = 0;	// 0 means: do not display
	fLeftBrace       = true;
	fRightBrace      = true;
}

//--------------------------------------------------------------------------
void ARTuplet::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* p = getParameter<TagParameterString>(kFormatStr);
	if (p) {
		fTupletFormat = p->getValue();
		parseTupletFormatString();
	}
	fPosition = getParameter<TagParameterString>(kPositionStr, true);
	fDy1 = getParameter<TagParameterFloat>(kDy1Str);
	fDy2 = getParameter<TagParameterFloat>(kDy2Str);
	fLineThickness = getParameter<TagParameterFloat>(kLineThicknesStr, true)->getValue();
	
	p = getParameter<TagParameterString>(kBoldStr);
	if (p) fTextBold = p->getBool();

	const TagParameterFloat* f = getParameter<TagParameterFloat>(kTextSizeStr);
	if (f) fTextSize = f->getValue();

	p = getParameter<TagParameterString>(kDispNoteStr);
	if (p) fDispNote = p->getValue();
}


bool ARTuplet::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\tupletEnd",s))
		return true;
	return false;
}


/** \brief Called for auto tuplet.
*/
void ARTuplet::setupTuplet( ARBase * inBase )
{
	// 
	const TYPE_DURATION & baseDur = inBase->getBase();

	const int num = baseDur.getNumerator();
	fBaseNumerator = num;
}

/** \brief Parses the format string  of tuplet.

	Format can be "x", "-x-", "x:y", "-x:y-", or "--"
	Where 'x' and 'y' are the numerator and the denominator.
	And '-' the presence of left and right tuplet braces.
*/
void ARTuplet::parseTupletFormatString()
{
	bool hasLeftBrace  = false;
	bool hasRightBrace = false;
	bool rightPart     = false;
	int  numerator     = 0;
	int  denominator   = 0;

	size_t len = fTupletFormat.length();

	for (size_t pos = 0; pos < len; ++ pos) {
		const char & c = fTupletFormat[pos];

		if (c == '-') {
			if (hasLeftBrace || rightPart || (numerator != 0)) { 
				hasRightBrace = true; 
				break; 
			}
			else 
				hasLeftBrace = true;
		}
		else if( c == ':' )
			rightPart = true;
		else if ((c >= '0') && (c <= '9')) {
			if (rightPart)
                denominator = (denominator * 10) + (c - '0');
			else
                numerator = (numerator * 10) + (c - '0');
		}
        else {
            hasLeftBrace  = true;
            hasRightBrace = true;
            numerator     = 0;
            denominator   = 0;
            break;
        }
	}

	// Store
	fBaseNumerator   = numerator < 100 ? numerator : 0;
	fBaseDenominator = denominator < 100 ? denominator : 0;
	fLeftBrace       = hasLeftBrace;
	fRightBrace      = hasRightBrace;
}

void ARTuplet::setAuto()								{ fTupletFormat = "-auto-"; }

int ARTuplet::isPositionAbove() const
{
	if(fPosition && fPosition->TagIsSet()) {
		string pos = fPosition->getValue();
		return (pos == "below" ? -1 : 1);
	}
	return 0;
}
