/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#ifdef MIDIEXPORT

#include <iostream>
#include <string>

#include "Midi2GUIDO.h"
#include "MIDI2GMNConverter.h"
#include "midifile.h"

using namespace std;

#define DEBUG 0

// ==========================================================================
// - Midi 2 Guido API
// ==========================================================================
GUIDOAPI(GuidoErrCode) GuidoMIDIFile2AR (const char* filename, int tracknum, string& outgmn)
{
	MIDIFile mf;
	if (!mf.Open(filename, MidiFileRead)) return guidoErrFileAccess;
	if (mf.ChooseTrack(tracknum)) {
		MidiSeqPtr seq = mf.ReadTrack();
		mf.Close();
		
		if (seq) {
			MIDI2GMNConverter converter(mf.infos().time);
			MidiEvPtr ev = FirstEv(seq);
			while (ev) {
				switch (EvType(ev)) {
					case typeKeyOn: {
#if DEBUG
cout << "note ON: " << Date(ev) << " - " << int(Pitch(ev)) << endl;
#endif
						outgmn += converter.NoteOn2GMN (Date(ev), Pitch(ev));
						break;
					}
					case typeKeyOff: {
#if DEBUG
cout << "note OFF: " << Date(ev) << " - " << int(Pitch(ev)) << endl;
#endif
						string gmn = converter.NoteOff2GMN (Date(ev), Pitch(ev));
						if (!gmn.empty()) {
							outgmn += gmn;
						}
						break;
					}
						
					case typeEndTrack: {
						outgmn += converter.Stop (Date(ev));
						break;
					}
				}
				ev = Link(ev);
			}
			mf.midi()->FreeSeq (seq);
		}
	}
	else return guidoErrBadParameter;
	return guidoNoErr;
}


// ==========================================================================
GUIDOAPI(MIDI2GMNConverter*) GuidoMIDI2GMNStart (short tpqn)
{
	return new MIDI2GMNConverter(tpqn);
}

GUIDOAPI(string) GuidoMIDI2GMNStop (MIDI2GMNConverter* converter, int date)
{
	return converter ? converter->Stop (date) : "";
}

GUIDOAPI(string) GuidoMIDINoteOn2GMN (MIDI2GMNConverter* converter, int date, int pitch)
{
	return converter ? converter->NoteOn2GMN (date, pitch) : "";
}

GUIDOAPI(string) GuidoMIDINoteOff2GMN (MIDI2GMNConverter* converter, int date, int pitch)
{
	return converter ? converter->NoteOff2GMN (date, pitch) : "";
}

#endif
