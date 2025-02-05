/*

  MusicXML Library
  Copyright (C) 2006,2007  Grame

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
#include "event2timevisitor.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
rational event2timevisitor::event2time (const Sguidoelement& score, unsigned int evIndex, unsigned int voiceIndex)
{
	fTargetVoice = voiceIndex;
	fTargetDate.set(-1,1);
	fTargetEvent = evIndex;
	init();
	fBrowser.browse(*score);
	return fDone ? currentVoiceDate().rationalise() : rational(-1,1);
}

//______________________________________________________________________________
int	event2timevisitor::time2event (const Sguidoelement& score, const rational& time, unsigned int voiceIndex)
{
	fTargetVoice = voiceIndex;
	fTargetDate = time;
	fTargetEvent = -1;
	init();
	fBrowser.browse(*score);
	int evIndex = -1;
	if (fDone)
	// target found in the normal curse of browsing
		evIndex = fCountVisitor.currentCount();
	// but also check for the last event of the voice that may extend over the target time
	else if (currentVoiceDate() > fTargetDate) 
		evIndex = fCountVisitor.currentCount() - 1;
	return evIndex;
}

//______________________________________________________________________________
void event2timevisitor::init ()
{
	fCurrentVoice = 0;
	fDone = false;
	durationvisitor::reset();
	fCountVisitor.reset();
}

//______________________________________________________________________________
bool event2timevisitor::done ()
{
	fDone = false;
	if (fTargetEvent >= 0) fDone = (fCountVisitor.currentCount() == (int)fTargetEvent);
	else fDone = (currentVoiceDate() > fTargetDate);
	return fDone;
}

//________________________________________________________________________
// The visit methods
//________________________________________________________________________
void event2timevisitor::visitStart ( SARVoice& elt )
{
	if (fCurrentVoice == fTargetVoice) {
		durationvisitor::visitStart(elt);
	}
	else {
		fBrowser.stop();		// this is to visit only the target voice
	}
}

//________________________________________________________________________
void event2timevisitor::visitEnd ( SARVoice& elt )
{
	if (fCurrentVoice == fTargetVoice) {
		durationvisitor::visitEnd(elt);
	}
	fBrowser.stop(false);
	fCurrentVoice++;
}

//______________________________________________________________________________
void event2timevisitor::visitStart( SARChord& elt )
{
	if (done()) fBrowser.stop();
	else {
		durationvisitor::visitStart(elt);
		fCountVisitor.visitStart(elt);
	}
}

//______________________________________________________________________________
void event2timevisitor::visitStart( SARNote& elt )
{
	if (!fInChord && done()) fBrowser.stop();
	else {
		durationvisitor::visitStart(elt);
		fCountVisitor.visitStart(elt);
	}
}

//______________________________________________________________________________
void event2timevisitor::visitEnd  ( SARChord& elt )
{ 
	if (done()) fBrowser.stop();
	else {
		durationvisitor::visitEnd(elt);
		fCountVisitor.visitEnd(elt);
	}
}

}
