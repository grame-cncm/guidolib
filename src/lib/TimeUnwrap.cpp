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

#include "TimeUnwrap.h"
#include "ARMusicalObject.h"
#include "ARMusicalVoice.h"

#include <iostream>
using namespace std;


//------------------------------------------------------------------------------
void TimeUnwrap::NewEv (const ARMusicalObject * ev)
{
	fRPosition = ev->getRelativeTimePosition();
	if (!fStartPos)
		fRepeatPos = fStartPos = ev;
}

void TimeUnwrap::Note (const ARMusicalObject * ev)	{ NewEv (ev); }
void TimeUnwrap::Rest (const ARMusicalObject * ev)	{ NewEv (ev); }

//------------------------------------------------------------------------------
void TimeUnwrap::AtPos (const ARMusicalObject * ev, EventType type)
{
	if (fDone) return;
	if (fPendingCoda && (type != kCoda)) return;

	Event (ev, type);
	switch (type) {

		case kNote:			Note (ev); break;
		case kRest:			Rest (ev); break;

		case kRepeatEnd:		
			if (fRepeatPos && !fRepeatMap[ev]) {
				fRepeatMap[ev]++;
				fRPosition.set(-1);								// to force mapping, in case a single
																// note is enclosed in the repeat
				fVoice->browse (*this, fRepeatPos, ev);			// jump to repeat begin
			}
			break;

		case kDaCapo:			fBackJump = ev; 
								Jump (ev, fStartPos, ev);		// restart from the beginning
			break;
		case kDaCapoAlFine:		fPendingFine = true;
								Jump (ev, fStartPos, fFinePos);	// restart from the beginning to Fine
			break;
		case kDaCoda:			
			if (fBackJump) {
				fPendingCoda = true;
				if (fCodaPos) Jump (ev, fCodaPos, ev);			// jump to coda
			}
			break;
		case kDalSegno:			
			if (fSegnoPos) {
				fBackJump = ev; 
				Jump (ev, fSegnoPos, ev);						// jump to segno
			}
			break;
		case kDalSegnoAlFine:
			if (fSegnoPos) {
				Jump (ev, fSegnoPos, fFinePos);
				fPendingFine = true;
			}
			break;

		case kRepeatBegin:		fRepeatPos = ev;
			break;
		case kFine:				fFinePos = ev;
			if (fPendingFine)	fDone = true;
			break;
		case kCoda:				fCodaPos = ev;
			if (fPendingCoda)	fPendingCoda = false;
			break;
		case kSegno:			fSegnoPos = ev;
			break;
		case kVolta:
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------
void TimeUnwrap::Jump (const ARMusicalObject* pos, const ARMusicalObject* from, const ARMusicalObject* to)
{
	if (!fJumpsMap[pos]) {
		fJumpsMap[pos]++;
		fRepeatMap.clear();
		fVoice->browse (*this, from, to);
	}
}

