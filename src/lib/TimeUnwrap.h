#ifndef __TimeUnwrap__
#define __TimeUnwrap__

/*
  GUIDO Library
  Copyright (C) 2010  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
						kMarcato, kIntens, kTie, kGlissando, kChordComma } EventType;

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
