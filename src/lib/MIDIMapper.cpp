/*
  GUIDO Library
  Copyright (C) 2011  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef MIDIEXPORT

#include <iostream>

#include "ARMusicalObject.h"
#include "ARMusicalVoice.h"
#include "ARMusicalVoiceState.h"
#include "ARNote.h"
#include "ARIntens.h"
#include "ARTempo.h"

#include "MIDIMapper.h"
#include "MidiShareLight.h"
#include "GUIDO2Midi.h"

using namespace std;

//------------------------------------------------------------------------------
MidiMapper::MidiMapper(ARMusicalVoice* voice, MidiLight* midi, const Guido2MidiParams* p, int chan, MidiSeqPtr seq, MidiSeqPtr tmap)
	: TimeUnwrap(voice), fParams(p), fMidi(midi), fSeq(seq), fTempoMap(tmap), fChan(chan), fLastTempo(0), fFlags(0)
{
	fCurrVelocity = 90;
}

MidiMapper::~MidiMapper()	{}

//------------------------------------------------------------------------------
void MidiMapper::Clear()
{
	fMidi->ClearSeq(fSeq);
	for (map<char, MidiEvPtr>::iterator i = fTiedNotes.begin(); i != fTiedNotes.end(); i++)
		fMidi->FreeEv(i->second);
	fTiedNotes.clear();
}

//------------------------------------------------------------------------------
void MidiMapper::TiedNote (MidiEvPtr note)
{
	MidiEvPtr e = fTiedNotes[Pitch(note)];
	if (!e) fTiedNotes[Pitch(note)] = note;
	else {
		Dur(e) += Dur(note);
		fMidi->FreeEv (note);
	}
}

void MidiMapper::MoveTime (TYPE_DURATION dur)	{ fUPosition += dur; }


//------------------------------------------------------------------------------
void MidiMapper::TempoChge(const ARMusicalObject * ev)
{
	const ARTempo * t = dynamic_cast<const ARTempo*>(ev);
	if (t) {
		MidiEvPtr ev = 0;
		if (t->isNoteEquiv() && (t->getBpmUnit() != t->getBpmValue())) {
			ev = fMidi->NewEv (typeTempo);
			Fraction ratio (t->getBpmUnit().getNumerator()*t->getBpmValue().getDenominator(),
							t->getBpmValue().getNumerator()*t->getBpmUnit().getDenominator());
			float factor = (float)ratio;
			fLastTempo = fLastTempo ? fLastTempo*factor : fParams->fTempo * factor;
		}
		else if (t->hasBpmInfos()) {
			ev = fMidi->NewEv (typeTempo);
			fLastTempo = t->getQpmValue();
		}
		if (ev) {
			Tempo(ev) = int(60000000/fLastTempo);
			Date(ev)  = Ticks (fUPosition);
			fMidi->AddSeq (fTempoMap, ev);
		}
	}
}

//------------------------------------------------------------------------------
void MidiMapper::IntensChge(const ARMusicalObject * ev)
{
	const ARIntens * i = dynamic_cast<const ARIntens*>(ev);
	if (i) {
		if (i->getText() == "ppp")			fCurrVelocity = 20;
		else if (i->getText() == "pp")		fCurrVelocity = 30;
		else if (i->getText() == "p")		fCurrVelocity = 50;
		else if (i->getText() == "mf")		fCurrVelocity = 70;
		else if (i->getText() == "f")		fCurrVelocity = 90;
		else if (i->getText() == "ff")		fCurrVelocity = 110;
		else if (i->getText() == "fff")		fCurrVelocity = 120;
	}
}

//------------------------------------------------------------------------------
void MidiMapper::Event(const ARMusicalObject * ev, EventType type)
{
	switch (type) {
		case kTempo:	TempoChge(ev);
			break;
		case kFermata:	fFlags |= hasFermata;
			break;
		case kSlur:		fFlags |= hasSlur;
			break;
		case kStaccato:	fFlags |= hasStaccato;
			break;
		case kTenuto:	fFlags |= hasTenuto;
			break;

		case kAccent:	fFlags |= hasAccent;
			break;
		case kMarcato:	fFlags |= hasMarcato;
			break;
		case kIntens:	IntensChge (ev);
			break;
		case kTie:		fFlags |= hasTie;
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------
int MidiMapper::AdjustDuration(int dur, const Guido2MidiParams* p) const
{
	if (!fFlags) return dur * p->fDFactor;

	if (fFlags & hasStaccato)	dur *= p->fStaccatoFactor;
	if (fFlags & hasSlur)		dur *= p->fSlurFactor;
	if (fFlags & hasTenuto)		dur *= p->fTenutoFactor;
	if (fFlags & hasFermata)	dur *= p->fFermataFactor;
	return dur;
}

//------------------------------------------------------------------------------
int MidiMapper::AdjustVelocity(int vel, const Guido2MidiParams* p) const
{
	if (!fFlags) return vel * p->fIntensity;

	if (fFlags & hasAccent)		vel *= p->fAccentFactor;
	if (fFlags & hasMarcato)	vel *= p->fMarcatoFactor;
	return (vel > 127 ? 127 : vel);
}

//------------------------------------------------------------------------------
void MidiMapper::Note(const ARMusicalObject * ev)
{
	if (ev->getDuration().getNumerator() <= 0) return;

	const ARNote* arn = dynamic_cast<const ARNote*>(ev);
	if (arn && (arn->getName() != ARNoteName::empty)) {
		MidiEvPtr note = fMidi->NewEv(typeNote);
		Chan(note)	= fChan;
		Dur(note)	= AdjustDuration(Ticks (ev->getDuration()), fParams);
		Vel(note)	= char(AdjustVelocity (fCurrVelocity, fParams));
		Pitch(note)	= arn->midiPitch();
		Date(note)  = Ticks (fUPosition);
		if (fFlags & hasTie) TiedNote (note);
		else fMidi->AddSeq (fSeq, note);
	}
	TYPE_DURATION offset = ev->getDuration();
	if (fFlags & hasFermata) offset *= fParams->fFermataFactor;
	if (!StartPos() || (ev->getRelativeTimePosition() != PrevPosition())) MoveTime (offset);
	TimeUnwrap::Note(ev);
	fFlags = knoflag;
}

//------------------------------------------------------------------------------
void MidiMapper::Rest(const ARMusicalObject * ev)
{
	MoveTime (ev->getDuration());
	TimeUnwrap::Rest(ev);
}

//------------------------------------------------------------------------------
void MidiMapper::End()
{
	for (map<char, MidiEvPtr>::iterator i = fTiedNotes.begin(); i != fTiedNotes.end(); i++)
		fMidi->AddSeq (fSeq, i->second);
	fTiedNotes.clear();

	MidiEvPtr end = fMidi->NewEv(typeEndTrack);
	Date(end)  = Ticks (fUPosition);
	fMidi->AddSeq (fSeq, end);
}

//------------------------------------------------------------------------------
int MidiMapper::Ticks (TYPE_DURATION dur)
{
	if (!dur.getNumerator() || !dur.getDenominator()) return 0;
	return int((float)dur * 4 * fParams->fTicks);
}

#endif

