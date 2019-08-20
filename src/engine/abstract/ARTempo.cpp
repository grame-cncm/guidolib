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

#ifdef WIN32
# pragma warning (disable : 4996)
#endif

#include <iostream>
#include <string>

#include "ARTempo.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

#include "TimeUnwrap.h"

using namespace std;


// --------------------------------------------------------------------------
ARTempo::ARTempo()
{
	setupTagParameters (gMaps->sARTempoMap);

	mBpmUnit.set( 1, 4 );		// arbitrary default value
	mBpmValue.set( 120 );		// idem
	mBpmNoteEquiv = false;
	mHasBpmInfos = false;
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

// ----------------------------------------------------------------------------
TYPE_DURATION ARTempo::getDuration (const char * str) const
{
	int num, denom;
	TYPE_DURATION duration;
	if( sscanf( str,"%d/%d", &num, &denom ) == 2 ) {
		duration.set ( num, denom );
		duration.normalize();
	}
	return duration;
}

// --------------------------------------------------------------------------
void ARTempo::setTagParameters(const TagParameterMap& map)
{
	ARFontAble::setTagParameters (map);
	const TagParameterString * tempo = getParameter<TagParameterString>(kTempoStr);
	if (tempo) {
		std::string value (tempo->getValue());
		if (value.size()) {
			FormatStringParser p;
			mTempoMark.clear();
			p.parse (value.c_str(), mTempoMark);
		}
	}
	const TagParameterString * bpm = getParameter<TagParameterString>(kBPMStr);
	if (bpm) ParseBpm (bpm);
}

// --------------------------------------------------------------------------
void ARTempo::ParseBpm(const TagParameterString * inTag )
{
	if( inTag == 0 ) return;
	if( inTag->TagIsNotSet() ) return;

	const NVstring & bpmString = inTag->getValue();

	int num1, num2, denom1, denom2;

	// - Look for something like "a/b=x/y" (note equivalent format)
	if (sscanf(bpmString.c_str(),"%d/%d=%d/%d", &num1, &denom1, &num2, &denom2 ) == 4) {
		mBpmUnit.set( num1, denom1 );
		mBpmValue.set( num2, denom2 );
		mBpmNoteEquiv = true;
		mHasBpmInfos = true;
	}

	// Look for something like "a/b=x"
	else if (sscanf( bpmString.c_str(),"%d/%d=%d", &num1, &denom1, &num2 ) == 3) {
		mBpmUnit.set( num1, denom1 );
		mBpmValue.set( num2, 1 );
		mBpmNoteEquiv = false;
		mHasBpmInfos = true;
	}
	else {
		// - The format is incorrect. TODO: report the error.
	}
}


