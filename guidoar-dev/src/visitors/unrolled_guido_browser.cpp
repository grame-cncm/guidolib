/*

  MusicXML Library
  Copyright (C) 2006  Grame

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

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <algorithm>
#include <vector>

#include "ARNote.h"
#include "AROthers.h"
#include "ARTag.h"
#include "guidoelement.h"
#include "unrolled_guido_browser.h"

using namespace std;
namespace guido 
{

//______________________________________________________________________________
unrolled_guido_browser::unrolled_guido_browser(basevisitor* v) : fVisitor(v)	{ reset(); }
void unrolled_guido_browser::enter (Sguidoelement& t)		{ t->acceptIn(*fVisitor); }
void unrolled_guido_browser::leave (Sguidoelement& t)		{ t->acceptOut(*fVisitor); }

//______________________________________________________________________________
void unrolled_guido_browser::reset()
{
	fRepeatMap.clear();					// clear the map of backward repeat measures
	fJumpsMap.clear();					// clear the map of jumps locations
	fWriteImplicit = true;				// for repeat bars to the top of the score 
	fCurrentNoteState.duration = rational(1,4);
	fCurrentNoteState.dots = 0;
	fCurrentNoteState.octave = 1;
}

//______________________________________________________________________________
void unrolled_guido_browser::browse (Sguidoelement& elt) { elt->acceptIn(*this); }

void unrolled_guido_browser::visitStart( Sguidoelement& elt)
{ 
	enter(elt);
	ctree<guidoelement>::literator iter;
	for (iter = elt->lbegin(); iter != elt->lend(); iter++)
		browse(*iter);
	leave(elt);
}

//______________________________________________________________________________
// we need to force writing implicit notes values (duration, dots, octave) at each
// possible jump point in order to avoid incorrect current notes status at jump time
void unrolled_guido_browser::visitStart( SARNote& elt)
{
	int octave = elt->GetOctave();
	if (!ARNote::implicitOctave(octave)) fCurrentNoteState.octave = octave;
	rational dur = elt->duration();
	if (!ARNote::implicitDuration (dur)) {
		fCurrentNoteState.duration = dur;
		fCurrentNoteState.dots = 0;
	}
	int dots = elt->GetDots();
	if (dots) fCurrentNoteState.dots = dots;

	if (fWriteImplicit) {
		elt->SetOctave( fCurrentNoteState.octave );
		elt->SetDots( fCurrentNoteState.dots );
		*(elt) = fCurrentNoteState.duration;
		fWriteImplicit = false;
	}
	elt->acceptIn(*fVisitor);
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARRepeatBegin& elt)
{
	fStoreIterator = &fForwardRepeat;
	fWriteImplicit = true;  // force writing implicit note values (duration, octave and dots)
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARRepeatEnd& elt)
{
	if (fRepeatMap[elt] == 0) {
		fNextIterator = fForwardRepeat;			// jump to fForwardRepeat at next iteration
		fRepeatMap[elt] = 1;
	}
}

//______________________________________________________________________________
bool unrolled_guido_browser::jump(ctree<guidoelement>::literator where, Sguidoelement elt)
{
	if (fJumpsMap[elt] == 0) {
		fRepeatMap.clear();				// reset repeat sections
		fNextIterator = where;			// set next iteration
		fJumpsMap[elt] = 1;
		return true;
	}
	return false;
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARDaCapo& elt)
{
	jump (fFirstMeasure, elt);
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARDaCoda& elt)
{
	if (fCodaIterator != fEndIterator)
		jump (fCodaIterator, elt);		// only if coda sign has already been met
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARDalSegno& elt)
{
	if (fSegnoIterator != fEndIterator)
		jump (fSegnoIterator, elt);	// only if segno sign has already been met
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARDalSegnoAlFine& elt)
{
	if ( (fSegnoIterator != fEndIterator) && (jump (fSegnoIterator, elt)))
		fEndIterator = fFineIterator;
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARDaCapoAlFine& elt)
{
	if (jump (fFirstMeasure, elt))
		fEndIterator = fFineIterator;
}

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARSegno& elt )		
				{ fStoreIterator = &fSegnoIterator; fWriteImplicit = true; }

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARCoda& elt )			
				{ fStoreIterator = &fCodaIterator; fWriteImplicit = true; }

//______________________________________________________________________________
void unrolled_guido_browser::visitStart( SARFine& elt )			{ fStoreIterator = &fFineIterator; }


//______________________________________________________________________________
// control all of the tree browsing is in charge of the SARVoice visit.
//
// Jumps are under control of fNextIterator which could be modified by the
// measure visit. Anchor points for future jumps (like segno) are stored
// at SARVoice level using fStoreIterator pointer.
//
// visit methods can use the fStoreIterator pointer to store the current iterator 
// to any ctree<guidoelement>::literator.
//
void unrolled_guido_browser::visitStart( SARVoice& elt)
{
	// first initializes the iterators used to broswe the tree
	// segno and coda are initialized to the end of the measures list
	fEndIterator = fFineIterator = elt->elements().end();
	fSegnoIterator= fCodaIterator = fEndIterator;
	// stores the first measures and makes a provision for the forward repeat location
	ctree<guidoelement>::literator iter = elt->elements().begin();
	fFirstMeasure = fForwardRepeat = iter;
	fStoreIterator = 0;

	reset();
	
	Sguidoelement gelt = elt;
	enter(gelt);			// normal visit of the part (pass thru)
	// while we're not at the end location (elements().end() is checked for safety reasons only)
	while ((iter != fEndIterator) && (iter != elt->elements().end())) {
		fNextIterator = iter;
		fNextIterator++;				// default value for next iterator is the next measure
		browse(*iter);					// browse the measure
		if (fStoreIterator) {			// check if we need to store the current iterator
			*fStoreIterator = ++iter;	// actually stores the next item (jumps items are skipped)
			fStoreIterator = 0;
		}
		iter = fNextIterator;			// switch to next iterator (which may be changed by the measure visit)
	}
	leave(gelt);			// normal visit of the part (pass thru)
}

}
