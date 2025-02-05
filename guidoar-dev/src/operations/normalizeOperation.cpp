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

#include <iostream>
#include <string>

#include "ARNote.h"
#include "ARFactory.h"
#include "AROthers.h"
#include "ARTag.h"
#include "clonevisitor.h"
#include "normalizeOperation.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//_______________________________________________________________________________
Sguidoelement normalizeOperation::operator() ( const Sguidoelement& score )
{
	Sguidoelement outscore;
	if (score) {
		tree_browser<guidoelement> tb(this);
		tb.browse (*score);
		outscore = fStack.top();
		fStack.pop();
	}
	return outscore;
}

//________________________________________________________________________
// The visit methods
//________________________________________________________________________
void normalizeOperation::visitStart ( SARVoice& elt )
{
	fCurrentOctave = ARNote::kDefaultOctave;
	fCurrentDuration.set(1,4);
	clonevisitor::visitStart (elt);
}

//________________________________________________________________________
void normalizeOperation::visitStart ( SARChord& elt )
{
	clonevisitor::visitStart (elt);
}

//________________________________________________________________________
void normalizeOperation::visitStart ( SARNote& elt )
{
	SARNote copy = clonevisitor::copy(elt);

	int octave = elt->GetOctave();
	
	if (!ARNote::implicitOctave(octave)) fCurrentOctave = octave;
	else copy->SetOctave (fCurrentOctave);

	rational duration = elt->duration();
	if (!ARNote::implicitDuration(duration)) fCurrentDuration = duration;
	else (*copy) = fCurrentDuration;
	push (copy);	
}

//________________________________________________________________________
void normalizeOperation::visitEnd ( SARChord& elt )
{
	clonevisitor::visitEnd (elt);
}

//________________________________________________________________________
void normalizeOperation::visitEnd ( SARVoice& elt )
{
	clonevisitor::visitEnd (elt);
}


}
