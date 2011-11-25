#ifndef __MidiMapper__
#define __MidiMapper__

/*
	GUIDO Library
	Copyright (C) 2011  Grame

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
		
		void		End();
		void		Clear();
};


#endif
