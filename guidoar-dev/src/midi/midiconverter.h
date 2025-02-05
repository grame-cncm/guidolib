/*

  guidoar Library
  Copyright (C) 2006,2007  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/
#ifdef MIDIEXPORT

#ifndef __midiConverter__
#define __midiConverter__

#include "export.h"
#include "guidoelement.h"
#include "midicontextvisitor.h"

#include "MidiShareLight.h"
#include "midifile.h"

struct TMidiSeq;

namespace guido 
{
/*!
\brief a midi converter

	It is based on MidiShare and on the Player libraries.
	It supports MIDIfile export as well as MidiShare player interface.
*/
class gar_export midiconverter : public midiwriter{
	public:
				 midiconverter(long tpq=480) : fTPQ(tpq), fSeq(0), fTimeSignDone(false), fVoiceNumber(0) {}
		virtual ~midiconverter();
		
		// main services are provided under the form of a midifile export
		virtual bool   score2midifile (Sguidoelement& score, const char* fileName);

	protected:
		MidiLight* midi()		{ return fMidifile.midi(); }

		// midiwriter interface support
		virtual void startVoice ();
		virtual void endVoice (long date);

		virtual void newNote (long date, int pitch, int vel, int duration, int art);
		virtual void tempoChange (long date, int bpm);
		virtual void progChange (long date, int prog);
		virtual void timeSignChange (long date, unsigned int num, unsigned int denom);
		virtual void keySignChange (long date, int signature, bool major);

		virtual bool getMidi (Sguidoelement& score);
		virtual void setCommon (MidiEvPtr ev, long date) const;
		
		long		fTPQ;
		MidiSeqPtr	fSeq;
		bool		fTimeSignDone;
		short		fVoiceNumber;
		MIDIFile	fMidifile;
};

}

#endif // __midiConverter__
#endif // MIDIEXPORT

