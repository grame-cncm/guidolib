/*
	GUIDO Library
	Copyright (C) 2009  Grame

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

