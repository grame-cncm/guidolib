/*
  GUIDO Library
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

#include <iostream>
#include <math.h>

#include "ARNote.h"
#include "ARFactory.h"
#include "AROthers.h"
#include "ARTag.h"
#include "clonevisitor.h"
#include "firstpitchvisitor.h"
#include "mirrorOperation.h"
#include "transposeOperation.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//________________________________________________________________________
// mirrorOperation implementation
//________________________________________________________________________
mirrorOperation::mirrorOperation () : fCurrentKey(0), fFixedPoint(0)	{}
mirrorOperation::~mirrorOperation() {}

//_______________________________________________________________________________
SARMusic mirrorOperation::operator() ( const SARMusic& score1, const SARMusic& score2 )
{
	if (!score1 || !score2) return 0;

	firstpitchvisitor fpv;
	int pitch = fpv.firstPitch (score2);
	if (pitch < 0) return score1;

	Sguidoelement elt = (*this)(score1, pitch);
	return dynamic_cast<ARMusic*>((guidoelement*)elt);
}

//_______________________________________________________________________________
Sguidoelement mirrorOperation::operator() ( const Sguidoelement& score, int midipitch )
{
	fFixedPoint = midipitch;
	fCurrentOctave = ARNote::kDefaultOctave;	// the default octave
	fCurrentKey = 0;

	tree_browser<guidoelement> tb(this);
	tb.browse (*score);
	Sguidoelement outscore = fStack.top();
	fStack.pop();
	return outscore;
}


//______________________________________________________________________________
void mirrorOperation::visitStart( SARNote& elt )	
{
	if (!elt->implicitOctave()) fCurrentOctave = elt->GetOctave();
	SARNote note = copy (elt);

	int midi = elt->midiPitch (fCurrentOctave);
	if (midi >= 0) {
		int targetInterval = (fFixedPoint - midi) * 2;
		int alter = 0;
		ARNote::pitch p = ARNote::chromaticOffsetPitch (elt->GetPitch(alter), targetInterval, fCurrentOctave, alter, (fCurrentKey>=0));

		string name; name += ARNote::NormalizedPitch2Name(p);
		note->setName (name);
		note->SetAccidental (alter);
		note->SetOctave (fCurrentOctave);
	}
	push (note);
}

//______________________________________________________________________________
void mirrorOperation::visitStart ( SARVoice& elt )
{
	fCurrentOctave = ARNote::kDefaultOctave;	// the default octave
	fCurrentKey = 0;
	clonevisitor::visitStart (elt);
}

//______________________________________________________________________________
void mirrorOperation::visitStart ( SARKey& elt )
{
	Sguidotag tag(elt);
	clonevisitor::visitStart (tag);
	Sguidoattribute attr = elt->getAttribute(0);
	if (attr) {
		if (attr->quoteVal()) {		// key is specified as a string
			int key = transposeOperation::convertKey (attr->getValue());
			if (key != transposeOperation::kUndefinedKey)
				fCurrentKey = key;
			else fCurrentKey = 0;
		}
		else  fCurrentKey = int(*attr);
	}
}

} // namespace
