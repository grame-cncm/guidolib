#ifndef __MIDI2GMNConverter__
#define __MIDI2GMNConverter__

/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <map>
#include <vector>

/*!
\addtogroup midi MIDI support
@{
*/

typedef struct note {
	int fStart;
	int fEnd;
	int fType;
	int fPitch;
};

class MIDI2GMNConverter {
	short fTicksPerQuarterNote;
	short fCurrentPitch;
	int	  fCurrentTime;
	int	  fMinimumTime;
	std::vector<note>	fNotesList;
	std::map<int, int>	fOpenedNotes;	// a map between midi pitch and index in fNotesList.
	
	enum	{ kNote, kRest, kTieBeg, kTieEnd, kChordBeg, kChordEnd };
	std::string midiPitch2gmnPitch	(int pitch) const;
	std::string midiTime2gmnTime	(int interval) const;
	
	public:
		/*! \brief a MIDI2GMNConverter constructor
			\param tpqn tick count per quarter note
					The value could be directly read from a MIDI file.
					Convertion from smpte time are handled by the constructor
					Default value assumes events time stamped in milliseconds with
					a tempo value of 120 bpm
		*/
				 MIDI2GMNConverter(short tpqn=500);
		virtual ~MIDI2GMNConverter() {}

	/*! \brief sets the minimum time for duration offset
			\param min a time value in ticks. Defaults to tpqs/64.
	*/
	void		MinTime		(int min)			{ fMinimumTime = min; }
	std::string	Stop		(int date);
	std::string	NoteOn2GMN	(int date, int pitch);
	std::string	NoteOff2GMN (int date, int pitch);
	std::string	Rest2GMN	(int date);
};

/*! @} */

#endif
