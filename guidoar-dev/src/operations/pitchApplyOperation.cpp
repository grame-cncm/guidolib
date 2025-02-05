/*

  Copyright (C) 2007  Grame


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
#endif

#include "pitchApplyOperation.h"

using namespace std;

namespace guido 
{

//_______________________________________________________________________________
// octaveVisitor
//_______________________________________________________________________________
class octaveVisitor : public visitor<SARNote>
{		
    public:
 				 octaveVisitor() { fBrowser.set(this); }
		virtual ~octaveVisitor() {}

		int		getLastOctave(const Sguidoelement& elt);
		void	forceOctave(const Sguidoelement& elt, int currentOctave);
		
    protected:
		tree_browser<guidoelement>	fBrowser;
		int		fCurrentOctave;
		bool	fForceOctave;
		virtual void visitStart		( SARNote& elt );
};

//_______________________________________________________________________________
void octaveVisitor::forceOctave( const Sguidoelement& elt, int currentOctave ) 
{	
	fCurrentOctave = currentOctave;
	fForceOctave = true;
	if (elt) fBrowser.browse (*elt);
}

//_______________________________________________________________________________
int octaveVisitor::getLastOctave( const Sguidoelement& elt ) 
{	
	fCurrentOctave = ARNote::getDefaultOctave();
	fForceOctave = false;
	if (elt) fBrowser.browse (*elt);
	return fCurrentOctave;
}

//_______________________________________________________________________________
void octaveVisitor::visitStart( SARNote& elt ) 
{	
	int octave = elt->GetOctave();
	if (fForceOctave) {
		if (octave == ARNote::getImplicitOctave())
			elt->SetOctave(fCurrentOctave);
		fBrowser.stop();
	}
	else if (octave != ARNote::getImplicitOctave())
		fCurrentOctave = octave;
}

//_______________________________________________________________________________
// pitchApplyBaseOperation
//_______________________________________________________________________________
Sguidoelement pitchApplyBaseOperation::browse( const Sguidoelement& score ) {
	fInChord = false;
	Sguidoelement outscore;
	if (score) {
		fBrowser.browse (*score);
		outscore = fStack.top();
		fStack.pop();
	}
	return outscore;
}

//_______________________________________________________________________________
void pitchApplyBaseOperation::visitStart( SARVoice& elt ) 
{
	fInChord = false;
	fLastOctave = fCurrentOctave = fCurrentScoreOctave = ARNote::getDefaultOctave();
	clonevisitor::visitStart (elt);
}

//_______________________________________________________________________________
void pitchApplyBaseOperation::setPitch( SARNote& note, const pitchvisitor::TPitch& pitch, int& currentOctave ) const 
{
	int octave = pitch.fOctave;
	if (octave == currentOctave) octave = ARNote::getImplicitOctave();
	else currentOctave = octave;
	note->setName(pitch.fName);
	note->SetOctave(pitch.fOctave);
	note->SetAccidental(pitch.fAlter);
}

//_______________________________________________________________________________
void pitchApplyBaseOperation::startChord( SARChord& elt, bool clone ) 
{ 
	fInChord = true; 
	fChordBase = (fMode == kUseHighest) ? -1 : 999;
	if (clone) clonevisitor::visitStart (elt);
}

//_______________________________________________________________________________
void pitchApplyBaseOperation::endChord  ( SARChord& elt, int targetpitch,  bool clone ) 
{
	fInChord = false;
	if (!clone) {
		octaveVisitor cv;
		cv.forceOctave(elt, fLastOctave);
		transposeOperation transpose;
		Sguidoelement chord = transpose (Sguidoelement(elt), targetpitch - fChordBase);
		if (chord) {
			int octave = cv.getLastOctave(chord);
			if (octave != ARNote::getDefaultOctave())
				fCurrentOctave = octave;
			push (chord, false);
		}
	}
	else clonevisitor::visitEnd (elt);
}

//_______________________________________________________________________________
void pitchApplyBaseOperation::setChordBase  ( SARNote& currentelt )
{
	int currentpitch = currentelt->midiPitch(fCurrentScoreOctave);
	switch (fMode) {
		case kUseHighest: 
			if (currentpitch > fChordBase) fChordBase = currentpitch;
			break;
		case kUseLowest: 
			if (currentpitch < fChordBase) fChordBase = currentpitch;
			break;
	}
}

//_______________________________________________________________________________
void pitchApplyBaseOperation::octaveCheck	( SARNote& elt )
{
	if (fLastOctave != ARNote::getImplicitOctave()) {
		int octave = elt->GetOctave();
		if (ARNote::implicitOctave(octave)) elt->SetOctave(fLastOctave);
		fLastOctave = ARNote::getImplicitOctave();
	}
}

//_______________________________________________________________________________
SARNote pitchApplyBaseOperation::startNote	( SARNote& elt )
{
	if (!elt->isPitched()) { clonevisitor::visitStart (elt); return 0; }

	if (!elt->implicitOctave()) 
		fCurrentScoreOctave = elt->GetOctave();
	SARNote note = copy(elt);
	if (note) {
		if (!note->implicitOctave()) 
			fLastOctave = note->GetOctave();		// store the last explicit octave
	}
	return note;
}

}
