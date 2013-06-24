/*
  GUIDO Library
  Copyright (C) 2009  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "TimeMapper.h"
#include "ARMusicalObject.h"
#include "ARMusicalVoice.h"

#include <iostream>
using namespace std;


//------------------------------------------------------------------------------
void TimeMapper::MoveUnrolledDate(TYPE_DURATION dur)	{ fUPosition += dur; }

//------------------------------------------------------------------------------
GuidoDate TimeMapper::UnrolledDate () const
{
	GuidoDate date = { fUPosition.getNumerator(), fUPosition.getDenominator() };
	return date;
}

//------------------------------------------------------------------------------
bool TimeMapper::FlushMap (const ARMusicalObject * ev)
{
	if (!StartPos()) return true;
	if ((ev->getRelativeTimePosition() != PrevPosition()) && (ev->getDuration().getNumerator() > 0))
		return true;
	return false;
}

//------------------------------------------------------------------------------
void TimeMapper::NewEv (const ARMusicalObject * ev)
{
	TYPE_TIMEPOSITION pos = ev->getRelativeTimePosition();
	TYPE_TIMEPOSITION end = pos + ev->getDuration();
	
	GuidoDate from	= { pos.getNumerator(), pos.getDenominator() };
	GuidoDate to	= { end.getNumerator(), end.getDenominator() };
	TimeSegment rolled (from, to);			// current rolled segment

	GuidoDate ufrom	= UnrolledDate();
	MoveUnrolledDate ( ev->getDuration() );
	GuidoDate uto	= UnrolledDate();
	TimeSegment unrolled(ufrom, uto);		// corresponding unrolled segment

	if (FlushMap(ev)) fCollector.Time2TimeMap (rolled, unrolled);
}

void TimeMapper::Note (const ARMusicalObject * ev)	{ NewEv (ev); }
void TimeMapper::Rest (const ARMusicalObject * ev)	{ NewEv (ev); }

