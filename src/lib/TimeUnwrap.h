#ifndef __TimeUnwrap__
#define __TimeUnwrap__

/*
	GUIDO Library
	Copyright (C) 2010  Grame

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

#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"
#include "GUIDOTypes.h"
#include "defines.h"

#include <map>

class ARMusicalObject;
class ARMusicalVoice;
//------------------------------------------------------------------------------
// mapping collector abstract class definition
class TimeUnwrap
{
	private:
		typedef const ARMusicalObject * Position;
		
		TYPE_TIMEPOSITION	fRPosition;		// the current rolled time position
		ARMusicalVoice*		fVoice;			// the musical voice we're browsing
		
		Position	fStartPos;		// the score start position
		Position	fFinePos;		// the score fine position
		Position	fCodaPos;		// the score coda position
		Position	fSegnoPos;		// the score segno position
		Position	fRepeatPos;		// the score repeat begin position
		Position	fBackJump;		// indicates a backward jump position (da capo or dal segno)
		bool		fPendingFine;
		bool		fPendingCoda;
		bool		fDone;

		std::map<Position,int>	fRepeatMap;			// used to avoid loops in scores
		std::map<Position,int>	fJumpsMap;			// used to avoid loops in scores

	public:
		typedef enum {	kNote, kRest, kRepeatBegin, kRepeatEnd, 
						kFine, kCoda, kSegno,
						kDaCapo, kDaCapoAlFine, kDaCoda,
						kDalSegno, kDalSegnoAlFine, kVolta, kTempo,
						kFermata, kAccent, kSlur, kStaccato, kTenuto,
						kMarcato, kIntens, kTie } EventType;

				 TimeUnwrap(ARMusicalVoice* voice) : fVoice(voice),
												fStartPos(0), fFinePos(0), fCodaPos(0),
												fSegnoPos(0), fRepeatPos (0), fBackJump(0),
												fPendingFine(false), fPendingCoda(false), fDone(false) {}
		virtual ~TimeUnwrap() {}
		
		virtual void	AtPos (const ARMusicalObject * ev, EventType type);

	protected:
		virtual void		Event (const ARMusicalObject * ev, EventType type) {}
		virtual void		Note (const ARMusicalObject * ev);
		virtual void		Rest (const ARMusicalObject * ev);
		const TYPE_TIMEPOSITION& PrevPosition() const	{ return fRPosition; }
		bool					 StartPos() const		{ return fStartPos != 0; }

	private:
		void	NewEv (const ARMusicalObject * ev);
		void	Jump (const ARMusicalObject* pos, const ARMusicalObject* to, const ARMusicalObject* end );
};


#endif
