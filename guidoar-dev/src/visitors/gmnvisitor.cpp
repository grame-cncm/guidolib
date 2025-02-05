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
#pragma warning (disable : 4786)
#endif

#include <algorithm>

#include "gmnvisitor.h"
#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "ARTag.h"

using namespace std;

namespace guido 
{

//_______________________________________________________________________________
struct count_notes {
	bool operator () (const Sguidoelement elt) const { 
		return dynamic_cast<ARNote*>((guidoelement*)elt)!=0;
	}
};

//______________________________________________________________________________
void gmnvisitor::visitStart ( Sguidotag& tag )
{
	if (tag->getAuto() && !fVisitAuto) return; // auto elements are not printed
	fOut << string(*tag);
	fOut.pbreak();
	if (tag->size() > 0) { 
		fOut << '(';
		if (fChordNotes > 1) fInsideTag++;
	}
	fOut << ' ';
}

//______________________________________________________________________________
void gmnvisitor::visitEnd ( Sguidotag& tag )
{
	if (tag->getAuto() && !fVisitAuto) return; // auto elements are not printed
	if (tag->size() > 0) {
		fOut << ") ";
		fOut.pbreak();
		if (fInsideTag) fInsideTag--;
		// here we check whether the container is inside a chord and whether
		// we need to print a note separator
		if (!fInsideTag && (fChordNotes > 1)) {
			fOut << ", ";
			fChordNotes--;
		}
	}
}

//______________________________________________________________________________
void gmnvisitor::visitStart ( SARNote& note )
{
	if (note->getAuto() && !fVisitAuto) return; // auto elements are not printed
	if (note->size() && fVisitAuto) return;		// the note has been automatically splitted

	fOut << string(*note);
	// inside a chord, notes are separated by a comma (',') 
	// but when the note is inside a container tag, the comma is printed when
	// the container is closed (ie after the ')'
	if ((fChordNotes > 1) && !fInsideTag) {
		fChordNotes--;
		fOut << ", ";
	}
	fOut << ' ';
	fOut.pbreak();
}

//______________________________________________________________________________
void gmnvisitor::barline ( Sguidotag& bar )
{
	if (bar->getAuto() && !fVisitAuto) return; // auto elements are not printed
	fOut << "\n| ";
}

//______________________________________________________________________________
void gmnvisitor::visitStart ( SARBar& bar )			{ Sguidotag t(bar); barline(t); }
void gmnvisitor::visitStart ( SARRepeatBegin& bar )	{ fOut << "\n " << string(*bar) << " "; }
void gmnvisitor::visitStart ( SARRepeatEnd& bar )	{ fOut << "\n " << string(*bar) << " "; }

//______________________________________________________________________________
void gmnvisitor::visitStart ( SARMusic& music )
{
	fVoicesCount = music->size();
	fOut << "{";
	if (fVoicesCount >= 1)
		fOut++ << "\n";
}

//______________________________________________________________________________
void gmnvisitor::visitEnd ( SARMusic& music )
{
	if (music->size() >= 1) {
		fOut--;
	}
	fOut << "}";
}

//______________________________________________________________________________
void gmnvisitor::visitStart ( SARChord& chord )
{
	if (chord->getAuto() && !fVisitAuto) return;
	count_notes p;
	fChordNotes = count_if(chord->begin(), chord->end(), p);
	fOut << "{ ";
}

//______________________________________________________________________________
void gmnvisitor::visitEnd ( SARChord& chord )
{
	if (chord->getAuto() && !fVisitAuto) return;
	fChordNotes = 0;
	fOut << "} ";
	fOut.pbreak();
}

//______________________________________________________________________________
void gmnvisitor::visitStart ( SARVoice& voice )
{
	fOut << "["; 
	if (voice->size () > 10) fOut++ << '\n';
}

//______________________________________________________________________________
void gmnvisitor::visitEnd ( SARVoice& voice )
{
	if (voice->size () > 10) --fOut << '\n';
	fOut << ']';
	if  (--fVoicesCount) fOut << ",\n\n";
	else --fOut << "\n" ;
}

} // namespace
