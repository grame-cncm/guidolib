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

#include "ARTuplet.h"
#include "ARBase.h"
#include "FormatStringParser.h"

#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARTuplet::ltpls(1);

ARTuplet::ARTuplet() : fTupletFormat(""), fPosition(""), fDy1(0), fDy2(0), fLineThickness(kDefaultThickness),
    fTextBold(false), fTextSize(1), fDispNote(""), fFormatSet(false), fDy1TagIsSet(false), fDy2TagIsSet(false)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	fBaseNumerator   = 0;	// 0 means: do not display
	fBaseDenominator = 0;	// 0 means: do not display
	fLeftBrace       = true;
	fRightBrace      = true;
	fPositionIsSet	 = false;
}

void ARTuplet::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,format,,r;S,position,above,o;U,dy1,0,o;U,dy2,0,o;F,lineThickness,0.08,o;S,bold,,o;F,textSize,1,o;S,dispNote,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for  the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString *tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			if (tps->TagIsSet()) {
				fTupletFormat = tps->getValue();
				parseTupletFormatString();
			}

            tps = TagParameterString::cast(rtpl->GetNext(pos));
			fPosition = tps->getValue();
			fPositionIsSet = tps->TagIsSet();

            TagParameterFloat *tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
            if (tpf->TagIsSet()) {
				fDy1 = tpf->getValue();
                fDy1TagIsSet = true;
			}

			tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
            if (tpf->TagIsSet()) {
				fDy2 = tpf->getValue();
                fDy2TagIsSet = true;
			}

            tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
            if (tpf->TagIsSet())
				fLineThickness = tpf->getValue();

            tps = TagParameterString::cast(rtpl->GetNext(pos));
            if (tps->TagIsSet())
	            fTextBold = tps->getBool();

            tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
            if (tpf->TagIsSet())
				fTextSize = tpf->getValue();

            tps = TagParameterString::cast(rtpl->GetNext(pos));
            if (tps->TagIsSet())
				fDispNote = tps->getValue();
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

/** \brief Called for auto tuplet.
*/
void ARTuplet::setupTuplet( ARBase * inBase )
{
	// 
	const TYPE_DURATION & baseDur = inBase->getBase();

	const int num = baseDur.getNumerator();
	fBaseNumerator = num;

/* (JB) disabled, don't force the format string of auto tuplets anymore.
	const int denom = baseDur.getDenominator();

	fBaseDenominator = denom;

	char buffer[32];
	sprintf( buffer,"-%d:%d", num, denom );

	if( IsPowerOfTwoDenom( inBase->getBaseDuration())) //, 4 ))
		strcat( buffer, "-");

	setName(buffer);
*/
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

	if (len > 0)
		fFormatSet = true;

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
            fFormatSet    = false;

            break;
        }
	}

	// Store
	fBaseNumerator   = numerator < 100 ? numerator : 0;
	fBaseDenominator = denominator < 100 ? denominator : 0;
	fLeftBrace       = hasLeftBrace;
	fRightBrace      = hasRightBrace;
}

void ARTuplet::setAuto()		{ fTupletFormat = "-auto-"; }

void ARTuplet::printName(std::ostream& os) const
{
    os << "ARTuplet";
}

void ARTuplet::printGMNName(std::ostream& os) const
{
    os << "\\tuplet";
}

void ARTuplet::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

int ARTuplet::isPositionAbove() const
{
	if(fPositionIsSet)
		return (fPosition == "below" ? -1 : 1);
	return 0;
}
