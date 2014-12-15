#ifndef __MidiMapper__
#define __MidiMapper__

/*
  GUIDO Library
  Copyright (C) 2011  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <map>

#include "TimeUnwrap.h"
#include "MidiShareLight.h"

typedef struct TMidiSeq *MidiSeqPtr;
struct Guido2MidiParams;

//------------------------------------------------------------------------------
class MidiMapper : public TimeUnwrap
{
	private:
		const Guido2MidiParams* fParams;
		MidiLight*		fMidi;
		MidiSeqPtr		fSeq;
		MidiSeqPtr		fTempoMap;
		int				fChan;
		float			fLastTempo;
		TYPE_TIMEPOSITION	fUPosition;		// the current unrolled time position
		int				fFlags;				// a bit field carrying accents, stacc, tenuto, etc...
		int				fCurrVelocity;		// the current velocity, updated by \intens tags
		std::map<char, MidiEvPtr>	fTiedNotes;		// tied notes mapped by Midi pitch
		TYPE_DURATION	fEmptyDur;			// empty duration storage: used for chords
		bool			fChord;				// a flag to indicate that we're in a chord
		
		enum { knoflag, hasStaccato=1, hasSlur=hasStaccato*2, hasTenuto=hasSlur*2, hasFermata = hasTenuto*2,
			hasAccent=hasFermata*2,  hasMarcato=hasAccent*2, hasTie=hasMarcato*2 };

		int			Ticks (TYPE_DURATION duration);
		int			AdjustDuration (int duration, const Guido2MidiParams* p) const;
		int			AdjustVelocity (int vel, const Guido2MidiParams* p) const;
		void		MoveTime (TYPE_DURATION duration);
		void		TempoChge (const ARMusicalObject * ev);
		void		IntensChge (const ARMusicalObject * ev);
		void		TiedNote (MidiEvPtr note);

	protected:
		virtual void		Event (const ARMusicalObject * ev, EventType type);
		virtual void		Note (const ARMusicalObject * ev);
		virtual void		Rest (const ARMusicalObject * ev);

	public:
				 MidiMapper(ARMusicalVoice* voice, MidiLight* midi, const Guido2MidiParams* p, int chan, MidiSeqPtr outseq, MidiSeqPtr tmap);
		virtual ~MidiMapper();
		
		virtual void	AtPos (const ARMusicalObject * ev, EventType type);

		void		End();
		void		Clear();
};


#endif
