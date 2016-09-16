/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <sstream>

#include "MIDI2GMNConverter.h"
#include "Fraction.h"

using namespace std;

//----------------------------------------------------------------------
// MIDI2GMNConverter interface
//----------------------------------------------------------------------
MIDI2GMNConverter::MIDI2GMNConverter(short tpqn)
{
	if (tpqn & 0x8000) {
		
	}
	else fTicksPerQuarterNote = tpqn;
	fCurrentPitch = -1;
	fCurrentTime = 0;
	fMinimumTime = fTicksPerQuarterNote / 48;
//cout << "min time is " << fMinimumTime << " tpqn; " << fTicksPerQuarterNote << endl;
}

string	MIDI2GMNConverter::Stop	(int date)
{
	if (fCurrentPitch > 0) {
		return NoteOff2GMN (date, fCurrentPitch);
	}
	return "";
}

string	MIDI2GMNConverter::NoteOn2GMN (int date, int pitch)
{
	note n;
	n.fStart = date;
	n.fEnd	 = 0;
	n.fPitch = pitch;
	n.fType  = kNote;
	fNotesList.push_back(n);
	fOpenedNotes[pitch] = fNotesList.size();

	string gmn;
	if (fCurrentPitch >= 0) {
		// there is already an opened note: close it
		gmn = NoteOff2GMN (date, fCurrentPitch);
		fCurrentPitch = pitch;
		return gmn;
	}

	int interval = date - fCurrentTime;
	if (interval > 0) {
		// there is some silence between the notes
		gmn = Rest2GMN (date);
	}
	fCurrentPitch = pitch;
	return gmn;
}

#define DEBUG 0

string	MIDI2GMNConverter::Rest2GMN (int date)
{
	stringstream s;
	s << '_' << '*' << midiTime2gmnTime(date - fCurrentTime) << " ";
	fCurrentTime = date;
#if DEBUG
cout << "  rest " << s.str() << endl;
#endif
	return s.str();
}

string	MIDI2GMNConverter::NoteOff2GMN (int date, int pitch)
{
	int index = fOpenedNotes[pitch];
	if (index) {
		fNotesList[index-1].fEnd = date;
	}
	else cerr << "warning: at date " << date << " try to close note " << pitch << " that was not opened" << endl;

	int interval = date - fCurrentTime;
	if (!interval) {
//cout << "0 interval date / pitch " << date << " - " << pitch << endl;
		interval = 1;
	}
	if (pitch != fCurrentPitch) return "";

	stringstream s;
	s << midiPitch2gmnPitch(pitch) << '*' << midiTime2gmnTime(interval) << " ";
	fCurrentPitch = -1;
	fCurrentTime = date;
#if DEBUG
cout << "  note off " << pitch << " - " << s.str() << endl;
#endif
	return s.str();
}


//----------------------------------------------------------------------
// private methods
//----------------------------------------------------------------------
string	MIDI2GMNConverter::midiPitch2gmnPitch	(int pitch) const
{
	// MIDI pitch 60 corresponds to gmn pitch c1
	int octave	= pitch/12 - 4;		// first get the octave number
	int remain	= pitch%12;			// get the remaining pitch

	stringstream s;
	int step = (remain >= 5) ? (remain+1)/2 : remain/2;
	char c = ((step > 4) ? 'a' + step - 5 : 'c' + step);
	s << c;

	remain = remain % 2;			// get accidental
	if (step < 3) {
		if (remain) s << '#';
	}
	else if (!remain) s << '#';
	s << octave;
	return s.str();
}

string	MIDI2GMNConverter::midiTime2gmnTime	(int interval) const
{
	if (interval < fMinimumTime) interval = fMinimumTime;
	Fraction d (interval, fTicksPerQuarterNote * 4);
	d.normalize();
	stringstream s;
	s << d;
	return s.str();
}
