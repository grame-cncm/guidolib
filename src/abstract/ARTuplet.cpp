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

#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARTuplet::ltpls(1);

ARTuplet::ARTuplet()
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	mDy1 = float(0.0);
	mDy2 = float(0.0);

	mBaseNumerator = 0;		// 0 means: do not display
	mBaseDenominator = 0;	// 0 means: do not display
	mLeftBrace = false;
	mRightBrace = false;

	mDy1Set = false;
	mDy2Set = false;
	mFormatSet = false;
}

void ARTuplet::setTagParameterList(TagParameterList & tpl)
{
//	mDy1 = mDy2 = float(0.0);

	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,format,,r;U,dy1,0,o;U,dy2,0,o" ));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast( rtpl->GetNext(pos));
			assert(tps);

			if( tps->TagIsSet())	// (JB) added
			{
				setName( tps->getValue());
				parseTupletFormatString();
			}

			TagParameterFloat * tpf = TagParameterFloat::cast( rtpl->GetNext(pos));
			assert(tpf);

			mDy1 = tpf->getValue();
			mDy1Set = tpf->TagIsSet();

			tpf = TagParameterFloat::cast( rtpl->GetNext(pos));
			assert(tpf);

			if (tpf->TagIsSet())
			{
				mDy2 = tpf->getValue();
				mDy2Set = true;
			}
			else
				mDy2 = mDy1;
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
void
ARTuplet::setupTuplet( ARBase * inBase )
{
	// 
	const TYPE_DURATION & baseDur = inBase->getBase();

	const int num = baseDur.getNumerator();
	mBaseNumerator = num;

/* (JB) disabled, don't force the format string of auto tuplets anymore.
	const int denom = baseDur.getDenominator();

	mBaseDenominator = denom;

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
void 
ARTuplet::parseTupletFormatString()
{
	bool hasLeftBrace = false;
	bool hasRightBrace = false;
	bool rightPart = false;

	int numerator = 0;
	int denominator = 0;

	size_t len = mTupletFormat.length();
	if( len > 0 )
		mFormatSet = true;

	for( size_t pos = 0; pos < len; ++ pos )
	{
		const char & c = mTupletFormat[ pos ];

		if( c == '-' )
		{
			if( hasLeftBrace || rightPart || (numerator != 0)) 
			{ 
				hasRightBrace = true; 
				break; 
			}
			else 
				hasLeftBrace = true;
		}
		else if( c == ':' )
		{
			rightPart = true;
		}
		else if(( c >= '0' ) && ( c <= '9' ))
		{
			if( rightPart )	denominator = (denominator * 10) + (c - '0');
			else numerator = (numerator * 10) + (c - '0');
		}
	}

	// Store
	mBaseNumerator = numerator;
	mBaseDenominator = denominator;
	mLeftBrace = hasLeftBrace;
	mRightBrace = hasRightBrace;
}

void 
ARTuplet::setName(const char * inName )
{
	mTupletFormat = inName;
}

std::ostream & ARTuplet::operator<<(std::ostream & os) const
{
	if (getRange())	os << "\\tuplet";
	else			os << "\\tupletBegin";

	int parset = 0;
	if (mTupletFormat.length()>0)
	{
		parset = 1;
		os << "<\"" << mTupletFormat << "\"";
		if (mDy1Set)
			os << ",";
	}
	if (mDy1Set)
	{
		if (!parset)
			os << "<";
		parset = 1;
		os << mDy1;
		if (mDy2Set)
			os << ",";
	}
	if (mDy2Set) 
	{
		if (!parset)
			os << "<";
		parset  = 1;
		os << mDy2;
	}
	if (parset)
	{
		os << ">";
	}
	if (getRange())
		os << "(";
	return os << " ";
}


