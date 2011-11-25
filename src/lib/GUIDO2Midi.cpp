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

#ifdef MIDIEXPORT

#include <iostream>
#include <vector>

#include "GUIDO2Midi.h"
#include "GUIDOInternal.h"

#include "ARMusicalVoice.h"
#include "ARMusic.h"
#include "MIDIMapper.h"
#include "midifile.h"

using namespace std;

// --------------------------------------------------------------------------
static void setDefault (Guido2MidiParams& params )
{
	params.fTempo = 120;
	params.fTicks = 960;		// (64*3*5)
	params.fChan  = 1;

	params.fIntensity		= 0.8f;
	params.fAccentFactor	= 1.1f;
	params.fMarcatoFactor	= 1.2f;
	params.fDFactor			= 0.8f;
	params.fStaccatoFactor	= 0.5f;
	params.fSlurFactor		= 1.0f;
	params.fTenutoFactor	= 0.90f;
	params.fFermataFactor	= 2.0f;
	params.fVChans.clear();
}

// --------------------------------------------------------------------------
static int getChan (int voice, const Guido2MidiParams* params)
{
	map<int, int>::const_iterator i = params->fVChans.find(voice);
	if (i != params->fVChans.end()) return i->second;
	return params->fChan;
}

// --------------------------------------------------------------------------
static MidiSeqPtr getVoice (ARMusicalVoice * voice, int index, MidiSeqPtr tmap, MidiLight* midi, const Guido2MidiParams* p)
{
	MidiSeqPtr seq = midi->NewSeq();
	MidiMapper gmn2midi (voice, midi, p, getChan(index, p), seq, tmap);
	voice->browse(gmn2midi);
	if (!seq->first) {
		midi->FreeSeq (seq);
		return 0;
	}
	gmn2midi.End();
	return seq;
}

// --------------------------------------------------------------------------
static bool WriteSeqs (vector<MidiSeqPtr>& seqs, MIDIFile& file)
{
	bool ret = true;
	for (size_t i=0; i<seqs.size(); i++) {
		ret &= file.WriteTrack (seqs[i]);
		file.midi()->FreeSeq (seqs[i]);
	}
	seqs.clear();
	return ret;
}

// --------------------------------------------------------------------------
static void CheckTempoMap (MidiSeqPtr tmap, MidiLight* midi, int defaultTempo)
{
	MidiEvPtr e = tmap->first;
	if (!e || (Date(e)>0)) {
		MidiEvPtr t = midi->NewEv (typeTempo);
		Tempo(t) = 60000000/defaultTempo;
		midi->AddSeq (tmap, t);
	}
	e = midi->NewEv (typeEndTrack);
	Date(e) = Date(tmap->last);
	midi->AddSeq (tmap, e);
}

// ==========================================================================
// - Guido Midi API
// ==========================================================================
GUIDOAPI(GuidoErrCode) GuidoAR2MIDIFile( const ARHandler ar, const char* filename, const Guido2MidiParams* params )
{
	if( ar == 0 )		return guidoErrInvalidHandle;
	if( !filename )		return guidoErrBadParameter;

	ARMusic * arMusic = ar->armusic;
	if( arMusic == 0 ) return guidoErrInvalidHandle;

	Guido2MidiParams defaultParams;
	setDefault (defaultParams);

	const Guido2MidiParams* p = params ? params : &defaultParams;
	MIDIFile file;
	if (file.Create (filename, midifile1, TicksPerQuarterNote, p->fTicks))
	{
		int index = 1;
		GuidoPos pos = arMusic->GetHeadPosition();
		ARMusicalVoice * voice = arMusic->GetNext(pos);
		vector<MidiSeqPtr> seqs;
		MidiSeqPtr tempoMap = file.midi()->NewSeq();
		seqs.push_back (tempoMap);
		while (voice) {
			MidiSeqPtr seq = getVoice (voice, index, tempoMap, file.midi(), p);
			if (seq) seqs.push_back (seq);
			index++;
			if (pos) voice = arMusic->GetNext(pos);
			else break;
		}
		CheckTempoMap (tempoMap, file.midi(), p->fTempo);
		WriteSeqs (seqs, file);
		file.Close();
		return guidoNoErr;
	}
	return guidoErrActionFailed;
}

#endif

