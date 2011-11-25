/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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

#include <iostream>
#include <string.h>

#include "secureio.h"
#include "ARTempo.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

ListOfTPLs ARTempo::ltpls(1);


// --------------------------------------------------------------------------
ARTempo::ARTempo()
{
	// mType = kString;

	mBpmUnit.set( 1, 4 );		// arbitrary default value
	mBpmValue.set( 120 );		// idem
	mBpmNoteEquiv = false;
	mHasBpmInfos = false;
}

// --------------------------------------------------------------------------
ARTempo::~ARTempo()
{
}

// --------------------------------------------------------------------------
void ARTempo::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kTempo);
}

// --------------------------------------------------------------------------
float ARTempo::getQpmValue() const
{
	if (hasBpmInfos()) {
		float unitfactor = float(getBpmUnit() * 4);
		return float(getBpmValue()) * unitfactor;
	}
	return 0.;
}

// --------------------------------------------------------------------------
void ARTempo::print() const
{
}

// --------------------------------------------------------------------------
void
ARTempo::setTagParameterList( TagParameterList & tpl )
{
	if( ltpls.GetCount() == 0 ) {
		ListOfStrings lstrs;
		// A required tempo string and an optional bpm string.
		lstrs.AddTail( "S,tempo,,r;S,bpm,,o;U,dy,0,o" );
		CreateListOfTPLs( ltpls, lstrs );
	}

	TagParameterList * rtpl = 0;
	const int ret = MatchListOfTPLsWithTPL( ltpls, tpl, &rtpl );

	/*
		We may have :
		\tempo <s1, s2>		// s1 is pure graphical, s2 (optional) pure musical
		\tempo <"Andante [1/4]=60">					will be the kString type
		\tempo <"Andante","1/4=60">					will be the kBPM type
		\tempo <"Andante [1/4]=[1/8]","1/4=1/8">	will be the kNoteEquiv type
	*/
	if( ret >= 0 && rtpl )
	{
		// we found a match!
		if( ret == 0 )
		{
			// - extract tempo mark informations.
			TagParameterString * tps = TagParameterString::cast(rtpl->RemoveHead());
			if (tps && strlen(tps->getValue())) {
				FormatStringParser p;
				mTempoMark.clear();
				p.parse (tps->getValue(), mTempoMark);
			}
			delete tps;
//			ParseTempoMark( tps );

			// - extract bpm informations
			tps = TagParameterString::cast(rtpl->RemoveHead());
			ParseBpm( tps );
			delete tps;

			delete mDy;
			mDy = TagParameterFloat::cast(rtpl->RemoveHead());
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
std::ostream & ARTempo::operator<<(std::ostream & os) const
{
/* (JB) out of date
	os << "\\tempo<\"";
	os << mName << "\"";
	if (mBpmstring.length()>0)
		os << ",\"" << mBpmstring << "\"";
	os << "> ";
*/
	return os;
}

// --------------------------------------------------------------------------
/*
void ARTempo::ParseTempoMark( TagParameterString * inTag )
{
	if( inTag )
		mTempoMark = inTag->getValue();
}
*/

// --------------------------------------------------------------------------
void ARTempo::ParseBpm( TagParameterString * inTag )
{
	if( inTag == 0 ) return;
	if( inTag->pflag == TagParameter::NOTSET ) return;

	const NVstring & bpmString = inTag->getValue();

	int num1, num2, denom1, denom2;

	// - Look for something like "a/b=x/y" (note equivalent format)
	if( sscanf(bpmString.c_str(),"%d/%d=%d/%d", &num1, &denom1, &num2, &denom2 ) == 4 )
	{
		mBpmUnit.set( num1, denom1 );
		mBpmValue.set( num2, denom2 );
		mBpmNoteEquiv = true;
		mHasBpmInfos = true;
	}

	// Look for something like "a/b=x"
	else if( sscanf( bpmString.c_str(),"%d/%d=%d", &num1, &denom1, &num2 ) == 3 )
	{
		mBpmUnit.set( num1, denom1 );
		mBpmValue.set( num2, 1 );
		mBpmNoteEquiv = false;
		mHasBpmInfos = true;

	}

	else
	{
		// - The format is incorrect. TODO: report the error.
	}
}

