/*
  Copyright © Grame 2008

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

#ifndef __pitchApplyOperation__
#define __pitchApplyOperation__

#include <vector>
#include <iostream>

#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "arexport.h"
#include "clonevisitor.h"
#include "operation.h"
#include "guidorational.h"
#include "pitchvisitor.h"
#include "transposeOperation.h"
#include "tree_browser.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief the base visitor of pitch application.

	Intended to minimize code size.
	Can only be instantiated by derived class.
*/
class gar_export pitchApplyBaseOperation :
	public operation,
	public clonevisitor
{		
    protected:
 				 pitchApplyBaseOperation(chordPitchMode m) : fMode(m) { fBrowser.set(this); }
		virtual ~pitchApplyBaseOperation()	{}


		bool	fInChord;
		chordPitchMode fMode;	// the chord pitch extraction and application mode;
		tree_browser<guidoelement>	fBrowser;
		int		fCurrentScoreOctave;	// the original score current octave
		int		fCurrentOctave;			// used to code implicit octave
		int		fLastOctave;			// used to restore correct octave at iterator end pos and at chords beginning
		int		fChordBase;				// used to transpose chords		

		Sguidoelement browse		( const Sguidoelement& score );
		virtual void visitStart		( SARVoice& elt );
		virtual void setPitch		( SARNote& note, const pitchvisitor::TPitch& pitch, int& currentOctave ) const;
		virtual void startChord		( SARChord& elt, bool clone );
		virtual void endChord		( SARChord& elt, int targetpitch,  bool clone );
		virtual void setChordBase	( SARNote& elt );
		virtual void octaveCheck	( SARNote& elt );
		virtual SARNote startNote	( SARNote& elt );
};

/*!
\brief A visitor that applies a list of pitches to a score.
*/
template <typename T> class gar_export pitchApplyOperation : public pitchApplyBaseOperation
{		
    public:
		typedef typename T::iterator pitchIterator;
 				 pitchApplyOperation(chordPitchMode m) : pitchApplyBaseOperation(m) {}
		virtual ~pitchApplyOperation()	{}

		/*! applies a list of pitches to a score
			\param score the taret score
			\param start an iterator on pitch values
			\param end an iterator on the end of pitch values
			\return a new score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, pitchIterator start, pitchIterator end ) {
			fPos = start;
			fEndPos = end;
			return browse(score);
		}

		/*! applies a list of durations to a score
			\param score1 the taret score
			\param score2 a score which pitch is applied to the target score 
			\return a new score
			\note pitch is extracted from the first voice only.
		*/
		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 )
		{
			pitchvisitor pv(fMode);
			T list;
			pv.pitch(score2, 0, &list);
			Sguidoelement elt = (*this)(score1, list.begin(), list.end());
			return dynamic_cast<ARMusic*>((guidoelement*)elt);
		}

    protected:
		typename T::iterator fPos, fEndPos;

		virtual void visitStart( SARNote& elt ) {
			SARNote note = startNote(elt);
			if (note) {
				if (fPos != fEndPos) {
					pitchvisitor::TPitch pitch = *fPos;
					if (!fInChord) {
						setPitch( note, pitch, fCurrentOctave);
						push( note );
						fPos++;
					}
					else setChordBase (elt);
				}
				else {
					octaveCheck (note);
					push( note );
				}
			}
		}

		virtual void visitStart( SARChord& elt ) { startChord (elt, fPos == fEndPos); }
		virtual void visitEnd  ( SARChord& elt ) {
			if (fPos == fEndPos) endChord (elt, 0, true);
			else {
				endChord (elt, pitchvisitor::midiPitch (*fPos), false);
				fPos++;
			}
		}
};

/// \brief a specialized class on low chords pitches
template <typename T> class gar_export pitchLowApplyOperation : public pitchApplyOperation<T>
{		
    public: pitchLowApplyOperation() : pitchApplyOperation<T>(kUseLowest) {}
};

/// \brief a specialized class on high chords pitches
template <typename T> class gar_export pitchHighApplyOperation : public pitchApplyOperation<T>
{		
    public: pitchHighApplyOperation() : pitchApplyOperation<T>(kUseHighest) {}
};

/*! @} */

} // namespace MusicXML


#endif
