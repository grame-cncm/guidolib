/*

  MusicXML Library
  Copyright (C) 2006-2009  Grame

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

#include <iostream>

#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "pitchvisitor.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
void pitchvisitor::pitch(const Sguidoelement& score, int voice, std::vector<TPitch>* outpitch)
{ 
	fInChord = false;
	fTargetVoice = voice;
	fCurrentVoice = 0;
	fCurrentOctave = 1;
	fPitch = outpitch;
	if (score) fBrowser.browse (*score);
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void pitchvisitor::visitStart( SARVoice& elt )
{
	stop(fCurrentVoice != fTargetVoice);
	fCurrentOctave = 1;
	fInChord = false;
}

//______________________________________________________________________________
void pitchvisitor::visitEnd  ( SARVoice& elt )
{ 
	fCurrentVoice++;
	stop(fCurrentVoice > fTargetVoice);
}

//______________________________________________________________________________
void pitchvisitor::visitStart( SARChord& elt )	
{ 
	fInChord = true; 
	fCurrentChordPitch.fName = "";
	fCurrentChordPitch.fOctave = ARNote::getImplicitOctave();
	fCurrentChordPitch.fAlter = 0;
	fCurrentChordMidiPitch = (fCMode == kUseHighest ? -1 : 999);
}

//______________________________________________________________________________
void pitchvisitor::visitEnd  ( SARChord& elt )	
{ 
	fInChord=false; 
	fPitch->push_back(fCurrentChordPitch);
}

//______________________________________________________________________________
void pitchvisitor::storePitch( SARNote& elt, TPitch& dst )
{
	dst.fName = elt->getName();
	int octave = elt->GetOctave();
	if (ARNote::implicitOctave (octave)) 
		octave = fCurrentOctave;
	else 
		fCurrentOctave = octave;
	dst.fOctave = octave;
	dst.fAlter = elt->GetAccidental();
}

//______________________________________________________________________________
void pitchvisitor::visitStart( SARNote& elt )
{
	if (!fInChord) {
		TPitch pitch;
		storePitch(elt, pitch);
		fPitch->push_back(pitch);
	}
	else {
		int pitch = elt->midiPitch(fCurrentOctave);
		switch (fCMode) { 
			case kUseHighest:
				if (pitch > fCurrentChordMidiPitch) {
					storePitch(elt, fCurrentChordPitch);
					fCurrentChordMidiPitch = pitch;
				}
				break;
			case kUseLowest:
				if (pitch < fCurrentChordMidiPitch) {
					storePitch(elt, fCurrentChordPitch);
					fCurrentChordMidiPitch = pitch;
				}
				break;
		}
	}
}

//______________________________________________________________________________
int pitchvisitor::midiPitch( const TPitch& pitch )
{
	SARNote note = ARNote::create();
	if (note) {
		int octave = pitch.fOctave;
		note->setName(pitch.fName);
		note->SetOctave(octave);
		note->SetAccidental(pitch.fAlter);
		return note->midiPitch (octave);
	}
	return -1;
}

}
