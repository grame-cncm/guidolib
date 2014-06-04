/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "GUIDOEngine.h"
#include "Guido2PianoRoll.h"
#include "ARMusicalVoiceState.h"
#include "ARMusicalTag.h"
#include "ARNote.h"
#include "ARRest.h"
#include "ARChordComma.h"
#include "ARNoteFormat.h"
#include "ARTagEnd.h"
#include "ARTie.h"
#include "GuidoTypes.h"
#include "ObjectList.h"
#include "TagParameterRGBColor.h"
#include "TagParameterString.h"
#include "VGDevice.h"

#include "MidiShareLight.h"
#include "midifile.h"

using namespace std;

#define kMinDist	4		// the minimum distance between lines of the grid

//-------------------------------------------------------------------
GuidoPianoRoll::GuidoPianoRoll(TYPE_TIMEPOSITION start, TYPE_TIMEPOSITION end, int width, int height)
	: fWidth(width), fHeight(height), fStartDate(start), fEndDate(end), fDuration(double(end-start)), fLowPitch(0), fHighPitch(127)
{
	fNoteHeight = fHeight / pitchrange();
	if (! fNoteHeight) fNoteHeight = 1;
}

//-------------------------------------------------------------------
int	GuidoPianoRoll::date2xpos (TYPE_TIMEPOSITION pos) const		{ return int(fWidth * double(pos-fStartDate) / fDuration); }
int	GuidoPianoRoll::duration2width (TYPE_DURATION dur) const		{ return int(fWidth * double(dur) / fDuration); }

//-------------------------------------------------------------------
int	GuidoPianoRoll::pitch2ypos (int midipitch) const
{
	int p = midipitch - fLowPitch;
	return fHeight - int((fHeight * p) / pitchrange());
}

//-------------------------------------------------------------------
void GuidoPianoRoll::DrawGrid (VGDevice* dev) const
{
	dev->PushPenWidth(0.3);
	for (int i=fLowPitch; i < fHighPitch; i++) {
		int y = pitch2ypos (i);
		int step = i % 12;		// the note in chromatic step
		if (fNoteHeight < kMinDist) {
			switch (step) {
				case 0 :			// C notes are highlighted
					dev->PushPenWidth((i==60) ? 1.0 : 0.6);
					dev->Line( 0, y, fWidth, y);
					dev->PopPenWidth();
					break;
				case 7:				// G
					dev->Line( 0, y, fWidth, y);
					break;
			}
		}
		else {
			switch (step) {
				case 0 :			// C notes are highlighted
					dev->PushPenWidth((i==60) ? 1.0 : 0.6);
					dev->Line( 0, y, fWidth, y);
					dev->PopPenWidth();
					break;
				case 2:				// D
				case 4:				// E
				case 5:				// F
				case 7:				// G
				case 9:				// A
				case 11:			// B
					dev->Line( 0, y, fWidth, y);
					break;
			}
		}
	}
	dev->PopPenWidth();
}

//-------------------------------------------------------------------
bool GuidoPianoRoll::handleColor (ARNoteFormat* nf, VGDevice* dev)
{
	if (nf) {
		const TagParameterString *s = nf->getColor();
		unsigned char colref[4];
		if (s && s->getRGB( colref )) {
			VGColor c(colref[0], colref[1], colref[2], colref[3]);
			dev->PushFillColor(c);
			fColored = true;
		}
		else if (fColored) {
			fColored = false;
			dev->PopFillColor();
		}
		return true;
	}
	return false;
}

//-------------------------------------------------------------------
void GuidoPianoRoll::Draw(ARMusicalObject* e, TYPE_TIMEPOSITION date, TYPE_DURATION dur, VGDevice* dev)
{
	ARNote * note = dynamic_cast<ARNote*>(e);

	if (note) {
		int pitch = note->midiPitch();
		if (pitch < 0)
			fChordDuration = dur;		// prepare for potential chord

		else {
			if (note->getName() != ARNoteName::empty) {
				int x = date2xpos (date);
				int w = duration2width (dur);
				int y = pitch2ypos (note->midiPitch());
				int halfstep = stepheight() / 2;
				if (!halfstep) halfstep = 1;
				
				dev->Rectangle( x, y-halfstep, x+(w ? w : 1), y+halfstep);
			}
//			cerr << e->getRelativeTimePosition() << ": note duration " <<  dur << " pitch " << note->midiPitch() << " (" << note->getName() <<")" << endl;
			fChord = false;
		}
	}
}

//-------------------------------------------------------------------
void GuidoPianoRoll::Draw(ARMusicalVoice* v, VGDevice* dev)
{
//cerr << "--------------------------" << endl;
	fColored = fChord = false;
	ObjectList * ol = (ObjectList*)v;
	GuidoPos pos = ol->GetHeadPosition();
	while(pos)
	{
		ARMusicalObject * e = ol->GetNext(pos);
		TYPE_DURATION dur = e->getDuration();
		TYPE_TIMEPOSITION date = e->getRelativeTimePosition();
		if (fChord) {
			dur = fChordDuration;
			date -= dur;
		}
		TYPE_TIMEPOSITION end = date + dur;

//		if ((date >= fStartDate) && (date < fEndDate))
//			Draw (e, date, dur, dev);
		if (date >= fStartDate) {
			if (end > fEndDate) dur = fEndDate - date;
			Draw (e, date, dur, dev);
		}
		else if (end > fStartDate) {
			date = fStartDate;	
			if (end > fEndDate) dur = fEndDate - date;
			else dur = end - date;
			Draw (e, date, dur, dev);
		}
		if (dynamic_cast<ARRest*>(e))
			fChord = false;
		else if (dynamic_cast<ARChordComma*>(e))
			fChord = true;
		else handleColor (dynamic_cast<ARNoteFormat*>(e), dev);
	}
	if (fColored) dev->PopFillColor();
}



//-------------------------------------------------------------------
// MIDI Piano roll
//-------------------------------------------------------------------
GuidoErrCode GuidoPianoRoll::Draw(const char* file, VGDevice* dev)
{
	if (!file || !dev) return guidoErrBadParameter;

	return guidoNoErr;
}

