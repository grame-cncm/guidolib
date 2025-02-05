/*
  Copyright � Grame 2003,2007

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

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifdef MIDIEXPORT

#include "midifile.h"
#include "midiconverter.h"
#include "tree_browser.h"

using namespace std;
namespace guido {

//________________________________________________________________________
// durations adjustment constants definition 
//________________________________________________________________________
#define kStaccatoCoef	0.5f
#define kSlurCoef		1.01f
#define kNormalCoef		0.90f

//________________________________________________________________________
// midiconverter 
//________________________________________________________________________
midiconverter::~midiconverter()
{
	if (fSeq) midi()->FreeSeq (fSeq);
}

//________________________________________________________________________
bool  midiconverter::score2midifile (Sguidoelement& score, const char* fileName)
{
	bool ret = false;
	if (getMidi(score)) {
		if (fMidifile.Create(fileName, midifile1, TicksPerQuarterNote, fTPQ))
			ret = fMidifile.WriteTrack (fSeq);
		midi()->FreeSeq (fSeq);
		fSeq = 0;
	}
	return ret;
}

//________________________________________________________________________
bool midiconverter::getMidi (Sguidoelement& score)
{
	if (!score) return false;

	if (fSeq) midi()->ClearSeq (fSeq);
	else fSeq = midi()->NewSeq();
	if (fSeq) {
		fTimeSignDone = false;
		fVoiceNumber  = 0;
		midicontextvisitor midivisitor (fTPQ, this);
		midivisitor.visit (score);
		return true;
	}
	return false;
}

//________________________________________________________________________
void midiconverter::setCommon (MidiEvPtr ev, long date) const
{
	Date(ev)	= date;
	RefNum(ev)	= fVoiceNumber;
	Chan(ev)	= fVoiceNumber - 1;
}

//________________________________________________________________________
// midiwriter interface support
//________________________________________________________________________
void midiconverter::startVoice ()
{
	fVoiceNumber++;
}

void midiconverter::endVoice (long date)
{
	MidiEvPtr ev = midi()->NewEv(typeEndTrack);
	if (ev) {
		setCommon(ev, date);
		midi()->AddSeq (fSeq, ev);
	}	
}

void midiconverter::newNote (long date, int pitch, int vel, int duration, int art)
{
	MidiEvPtr ev = midi()->NewEv(typeNote);
	if (ev) {
		setCommon(ev, date);
		Pitch(ev)	= pitch;
		Vel(ev)		= vel;
		if (art == midiwriter::kStaccato)
			duration *= kStaccatoCoef;
		else if (art == midiwriter::kSlur)
			duration *= kSlurCoef;
		else 
			duration *= kNormalCoef;
		Dur(ev)		= duration;
		midi()->AddSeq (fSeq, ev);
	}	
}

void midiconverter::tempoChange (long date, int bpm)
{
	MidiEvPtr ev = midi()->NewEv(typeTempo);
	if (ev) {
		setCommon(ev, date);
		Tempo(ev)	= 60000000 / bpm;
		midi()->AddSeq (fSeq, ev);
	}	
}

void midiconverter::progChange (long date, int prog)
{
	MidiEvPtr ev = midi()->NewEv(typeProgChange);
	if (ev) {
		setCommon(ev, date);
		Data(ev)[0]	= prog;
		midi()->AddSeq (fSeq, ev);
	}	
}

void midiconverter::timeSignChange (long date, unsigned int num, unsigned int denom)
{
	if (fTimeSignDone) return;
	
	MidiEvPtr ev = midi()->NewEv(typeTimeSign);
	if (ev) {
		setCommon(ev, date);
		TSNum(ev)	= num;
		TSDenom(ev)	= denom;

		int clocksDiv = 1;
		while (denom > 1) {
			denom--;
			clocksDiv *= 2;
		}
		TSClocks(ev)= 96 / clocksDiv;	
		TS32nd(ev)	= 8;		// number of 32th note in a quarter note
		midi()->AddSeq (fSeq, ev);
		fTimeSignDone = true;
	}	
}

void midiconverter::keySignChange (long date, int signature, bool major)
{
	MidiEvPtr ev = midi()->NewEv(typeKeySign);
	if (ev) {
		setCommon(ev, date);
		KSTon(ev)	= signature;
		KSMode(ev)	= major ? 0 : 1;
		midi()->AddSeq (fSeq, ev);
	}	
}

} // end namespace

#endif

