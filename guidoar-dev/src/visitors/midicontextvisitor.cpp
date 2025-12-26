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

#ifdef WIN32
# pragma warning (disable : 4786)
# define _CRT_SECURE_NO_DEPRECATE
#endif

#include <algorithm>
#include <ctype.h>
#include <stdio.h>

#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "ARTag.h"
#include "midicontextvisitor.h"
#include "transposeOperation.h"
#include "unrolled_guido_browser.h"

using namespace std;
namespace guido {

//________________________________________________________________________
// midicontextvisitor 
//________________________________________________________________________
midicontextvisitor::~midicontextvisitor() {}

//________________________________________________________________________
midicontextvisitor::midicontextvisitor(long tpq, midiwriter* writer) 
{ 
	fTPQ = tpq;
	fMidiWriter = writer;
	reset ();
}

//________________________________________________________________________
void midicontextvisitor::visit(Sguidoelement& elt) 
{ 
	reset();
	fEndTie = elt->end();
	unrolled_guido_browser tb(this);
	tb.browse (elt);
}

//________________________________________________________________________
void midicontextvisitor::reset() 
{ 
	fInChord =  fInSlur = fInStaccato = fInGrace = false;
	fTieState = kNoTie;
	// reset to the default guido implicit values
    fCurrentDate = fChordDuration = 0;
	fCurrentDuration = rational (1,4);
    fTranspose = fCurrentDots = 0;
	fCurrentOctave = 1;
	fCurrentVel = 90;
	fTiedMap.clear();
}

//________________________________________________________________________
int midicontextvisitor::moveTime(int dur) 
{
	if (!fInChord) return dur;
	if (dur > fChordDuration) fChordDuration = dur;
	return 0;
}

//________________________________________________________________________
int midicontextvisitor::rational2ticks(const rational& dur) const
{
	// dur is expressed in whole note fraction
	float fdur = float(dur * 4);
	return int(fdur * fTPQ); 
}

//________________________________________________________________________
/*
int midicontextvisitor::midiPitch (const SARNote& elt)  const
{
	int alter; int midi = -1;
	ARNote::pitch pitch = elt->GetPitch (alter);
	if (pitch != ARNote::kNoPitch) {
		// offset in octave numeration between guido and midi is 4
		int midioctave = (fCurrentOctave + 4) * 12;
		midi = midioctave + (pitch*2) + alter;
		if (pitch > ARNote::E) midi--;
	}
	return midi;
}
*/

//________________________________________________________________________
rational midicontextvisitor::noteduration (const SARNote& elt, rational& currentDuration, int& currentDots ) const
{
	rational dur = elt->duration();
	if (!ARNote::implicitDuration (dur)) {
		currentDuration = dur;
		currentDots = 0;
	}
	int dots = elt->GetDots();
	if (dots) currentDots = dots;
	dur = currentDuration;
	dots = currentDots;
	rational dotdur = dur;
	while (dots--) {
		dotdur /= 2;
		dotdur.rationalise();
		dur += dotdur;
		dur.rationalise();
	}
	return dur;
}

//________________________________________________________________________
// the various play methods
//________________________________________________________________________
void midicontextvisitor::playTempoChange (long bpm)
{
	if (fMidiWriter && bpm)
		fMidiWriter->tempoChange (fCurrentDate, bpm);
}

void midicontextvisitor::playProgChange (long prog)
{
	if (fMidiWriter)
		fMidiWriter->progChange (fCurrentDate, prog);
}

//________________________________________________________________________
void midicontextvisitor::playNote (long date, int pitch, int duration)
{
	if (fMidiWriter) {
		int articulation = midiwriter::kNone;
		if (fInSlur) articulation += midiwriter::kSlur;
		if (fInStaccato) articulation += midiwriter::kStaccato;
		fMidiWriter->newNote (date, pitch + fTranspose, fCurrentVel, duration, articulation);
	}	
}

//________________________________________________________________________
void midicontextvisitor::playMidiInstrument (int progChange)
{
	if (fMidiWriter) {
		fMidiWriter->progChange (fCurrentDate, progChange);
	}
}

//________________________________________________________________________
void midicontextvisitor::playMeterChange (unsigned int num, unsigned int denum)
{
	if (fMidiWriter) {
		fMidiWriter->timeSignChange (fCurrentDate, num, denum);
	}
}

//________________________________________________________________________
void midicontextvisitor::playKeySignChange (int sign, bool major)
{
	if (fMidiWriter) {
		fMidiWriter->keySignChange (fCurrentDate, sign, major);
	}
}


//________________________________________________________________________
// ties management
//________________________________________________________________________
void midicontextvisitor::startTie(Sguidoelement tie, bool storeEnd)	
{ 
	fTiedMap.clear();
	fTieState = kInTie;
	if (storeEnd) {
		guidoIterator i = find(fCurrentVoice->begin(), fCurrentVoice->end(), tie);
		i.rightShift();
		fEndTie = i;
	}
	else fEndTie = fCurrentVoice->end();
}

//________________________________________________________________________
void midicontextvisitor::stopTie()	
{ 
	fTiedMap.clear();
	fTieState = kNoTie;
}

//________________________________________________________________________
void midicontextvisitor::storeNotes( SARChord& elt, ARNotes& dest )	
{
	guidoIterator i;
	for (i = elt->begin(); i != elt->end(); i++) {
		SARNote note = dynamic_cast<ARNote*>((guidoelement*)(*i));
		if (note) dest.push_back(note); 
	}
}

//________________________________________________________________________
// note: similar pitch from different name space are different (like g and sol)
// this is to comply with the current layout result  
bool midicontextvisitor::equalPitch (const SARNote& n1, const SARNote& n2) const
{
	int octave1 = n1->GetOctave();
	if (ARNote::implicitOctave(octave1)) octave1 = fCurrentOctave;
	int octave2 = n2->GetOctave();
	if (ARNote::implicitOctave(octave2)) octave2 = fCurrentOctave;

	return	(n1->getName() == n2->getName()) &&
			(octave1 == octave2) &&
			(n1->GetAccidental() == n2->GetAccidental());
}

//________________________________________________________________________
rational midicontextvisitor::totalDuration( const ARNotes& list ) const
{
	rational listDuration (0,1);
	rational currentDuration = fCurrentDuration;
	int currentDots = fCurrentDots;
	for (ARNotes::const_iterator i = list.begin(); i != list.end(); i++) {
		listDuration += noteduration(*i, currentDuration, currentDots);
		listDuration.rationalise();
	}
	return listDuration;
}

//________________________________________________________________________
void midicontextvisitor::lookupTied(guidoIterator start, guidoIterator end, const SARNote& note, ARNotes& list)	
{ 
	int chordnotes = 0;
	while (start != end) {
		// end of a tie depends on the end iterator or on the \tieEnd tag
		SARTieEnd tieEnd = dynamic_cast<ARTag <kTTieEnd>* >((guidoelement*)(*start));
		if (tieEnd) break;

		SARNote next = dynamic_cast<ARNote*>((guidoelement*)(*start));
		if (next) {
			if (chordnotes) chordnotes--;		// notes inside a chord are handled below
			else if ( equalPitch(note, next) ) {
				 list.push_back(next);
				 fTiedMap[next] = 1;
			}
			else {
				fTieState = kInTie;		// return to tie begin state
				break;					// which allows for further ties to operate
			}
		}

		SARChord chord = dynamic_cast<ARChord*>((guidoelement*)(*start));
		if (chord) {
			ARNotes clist;
			storeNotes (chord, clist);		// get the notes from the chord
			chordnotes = clist.size();		// stores the notes count (to be ignored above)
			bool chordTied = false;			// a flag to check for effective tied notes
			for (ARNotes::const_iterator i = clist.begin(); i != clist.end(); i++) {
				if ( equalPitch(note, *i) ) {
					list.push_back(*i);
					fTiedMap[*i] = 1;
					chordTied = true;
				}
			}
			if (!chordTied) break;			// chords without a tie breaks the tie processing
		}
		start++;
	}
}

//________________________________________________________________________
// the visitxxx methods
//________________________________________________________________________
void midicontextvisitor::visitStart ( SARVoice& elt )
{
	reset ();
	fCurrentVoice = elt;
	if (fMidiWriter) fMidiWriter->startVoice();
}
void midicontextvisitor::visitEnd ( SARVoice& elt )		{ if (fMidiWriter) fMidiWriter->endVoice (fCurrentDate); }

//________________________________________________________________________
void midicontextvisitor::visitStart( SARNote& elt )
{
	int noteDuration = rational2ticks (noteduration(elt, fCurrentDuration, fCurrentDots));
	if (fTiedMap[elt]) {
		fCurrentDate += moveTime (noteDuration);	// moves the current time
		return;										// but skip already tied notes
	}
	
//	int octave = elt->GetOctave();
//	if (octave != ARNote::kUndefined) fCurrentOctave = octave;
	int dur = 0;

	if (fTieState == kInTie) {				// we've just entered a tie
		fTieState = kTiedNote;

		ARNotes list;
		list.push_back(elt);
		// 		
		guidoIterator i = find(fCurrentVoice->begin(), fCurrentVoice->end(), elt);
		if (i != fCurrentVoice->end()) {
			lookupTied (++i, fEndTie, elt, list);			// get the list of tied notes
			dur = rational2ticks (totalDuration(list));		// and compute the total duration
		}
	}
	else dur = noteDuration;
//	int pitch = midiPitch(elt);
	int pitch = elt->midiPitch(fCurrentOctave);
	if (pitch >= 0)	{
		if (fInGrace) {
			// play grace notes ahead of current position
			playNote (fCurrentDate - (noteDuration/2), pitch, dur);
			// and do not move time
			noteDuration = 0;
		}
		else
			playNote (fCurrentDate, pitch, dur);
	}
	fCurrentDate += moveTime (noteDuration);
}

//________________________________________________________________________
void midicontextvisitor::visitStart( SARTie& elt )			{ startTie(elt, true); }
void midicontextvisitor::visitEnd  ( SARTie& elt )			{ stopTie(); }
void midicontextvisitor::visitStart( SARTieBegin& elt )		{ startTie(elt); }
void midicontextvisitor::visitStart( SARTieEnd& elt )		{ stopTie(); }

//________________________________________________________________________
void midicontextvisitor::visitStart( SARChord& elt )	{ 
	fChordDuration = 0; 
	fInChord = true; 
	if (fTieState == kInTie) {
		fTieState = kTiedChord;
		ARNotes cnotes;
		storeNotes (elt, cnotes);
		guidoIterator i = find(fCurrentVoice->begin(), fCurrentVoice->end(), elt);
		if (i == fCurrentVoice->end()) return;

		for (ARNotes::const_iterator note = cnotes.begin(); note != cnotes.end(); note++) {
			if (fTiedMap[*note]) continue;

			ARNotes list;			
			lookupTied (i, fEndTie, *note, list);
			int dur = rational2ticks (totalDuration(list));
//			int pitch = midiPitch(*note);
			int pitch = (*note)->midiPitch(fCurrentOctave);
			if (pitch >= 0)	playNote (fCurrentDate, pitch, dur);
		}
	}
}
void midicontextvisitor::visitEnd  ( SARChord& elt )	{ fInChord = false; fCurrentDate += moveTime (fChordDuration); }

//________________________________________________________________________
void midicontextvisitor::visitStart( SARTempo& elt )
{
	string val = elt->getAttributeValue(0);
	int num, denum, tempo;
	int n = sscanf ( val.c_str(), "%d/%d=%d", &num, &denum, &tempo);
	if (n != 3) {
		val = elt->getAttributeValue(1);
		n = sscanf ( val.c_str(), "%d/%d=%d", &num, &denum, &tempo);
	}
	if (n == 3) {
		rational beat(num, denum);
		rational quarter(1,4);
		rational mult = beat / quarter;
		tempo *= mult.getNumerator();
		tempo /= mult.getDenominator();
		playTempoChange (tempo);
	}
}

//________________________________________________________________________
void midicontextvisitor::visitStart( SARIntens& elt )
{
	float intens = elt->getAttributeFloatValue (1, 0.);
	if (intens) fCurrentVel = int(127 * intens);
}

//________________________________________________________________________
void midicontextvisitor::visitStart( SAROct& elt )
{
	fTranspose = int(elt->getAttributeFloatValue (0, 0) * 12);
}
void midicontextvisitor::visitEnd( SAROct& elt )
{
	if (elt->size()) fTranspose = 0;
}

//________________________________________________________________________
void midicontextvisitor::visitStart( SARInstr& elt )
{
	string midi = elt->getAttributeValue (1);
	int prog;
	if (sscanf ( midi.c_str(), "MIDI %d", &prog) == 1)
		playProgChange (prog);
}

//________________________________________________________________________
void midicontextvisitor::visitStart( SARKey& elt )
{
	Sguidoattribute attr = elt->getAttribute(0);
	if (attr) {
		if (attr->quoteVal()) {		// key is specified as a string
			string val = attr->getValue();
			int key = transposeOperation::convertKey (val);
			if (key != transposeOperation::kUndefinedKey) {
				playKeySignChange (key, isupper (val[0]) ? true : false);				
			}
		}
		else playKeySignChange (int(*attr));
	}
}

//________________________________________________________________________
void midicontextvisitor::visitStart( SARMeter& elt )
{
	const string val = elt->getAttributeValue(0);
	unsigned int num, denum;
	if (val == "C")
		num = denum = 4;
	else if ((val == "C/") || (val == "c/"))
		num = denum = 2;
	else if (sscanf ( val.c_str(), "%d/%d", &num, &denum) != 2)
		return;
	// now converts the denominator into midifile compliant representation
	unsigned int mfdenum = 0;
	while (denum > 1) {
		mfdenum++;
		denum /= 2;
	}
	playMeterChange (num, mfdenum);	
}


} // end namespace
