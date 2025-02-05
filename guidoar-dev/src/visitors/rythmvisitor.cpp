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
#include "rythmvisitor.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
void rythmvisitor::rythm(const Sguidoelement& score, int voice, std::vector<rational>* outrythm)
{ 
	fInChord = false;
	fTargetVoice = voice;
	fCurrentVoice = 0;
	fRythm = outrythm;
	fCurrentNoteDuration = ARNote::getDefaultDuration();
	fCurrentDots = 0;
	if (score) fBrowser.browse (*score);
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void rythmvisitor::visitStart( SARVoice& elt )
{
	stop(fCurrentVoice != fTargetVoice);
	fInChord = false;
	fCurrentNoteDuration = ARNote::getDefaultDuration();
	fCurrentDots = 0;
}

//______________________________________________________________________________
void rythmvisitor::visitEnd  ( SARVoice& elt )
{ 
	fCurrentVoice++;
	stop(fCurrentVoice > fTargetVoice);
}

//______________________________________________________________________________
void rythmvisitor::visitStart( SARChord& elt )		{ fInChord = true; }
void rythmvisitor::visitEnd  ( SARChord& elt )		{ stop(fInChord=false); }

//______________________________________________________________________________
void rythmvisitor::visitStart( SARNote& elt )
{
	rational duration = elt->totalduration(fCurrentNoteDuration, fCurrentDots);
	fRythm->push_back(duration);
	if ( fInChord ) stop(true);			// done for chords: get the first note duration only
}


}
