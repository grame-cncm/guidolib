/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <algorithm>

#include "GRTrillsLinker.h"

#include "GRBar.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GREmpty.h"
#include "GRTrill.h"

#include "ARNote.h"

using namespace std;

std::ostream& operator << (std::ostream & os, const ChordTrills& ct) { ct.print(os); return os; }

//-------------------------------------------------------------------------------
void ChordTrills::print(std::ostream & os ) const
{
	for_each (fTrills.begin(), fTrills.end(), [&os] (pair<GRNote*, GRTrill*> elt)
		-> void { os << elt.first << " " << elt.second << " " << elt.second->getBegin() << endl;} );
	if (fFirstNote) os << "first note: " << fFirstNote << endl;
}

//-------------------------------------------------------------------------------
bool ChordTrills::sameNote (GRNote* n1, GRNote* n2) const
{
	const ARNote* prev = n1->getARNote();
	const ARNote* cur  = n2->getARNote();
	return (prev->getMidiPitch() == cur->getMidiPitch());
}

//-------------------------------------------------------------------------------
ChordTrills::TTrills::iterator ChordTrills::find (GRNote* note)
{
	for (TTrills::iterator i = fTrills.begin(); i != fTrills.end(); i++) {
		if (sameNote(i->first, note)) return i;
	}
	return end();
}

//-------------------------------------------------------------------------------
void ChordTrills::add(GRNote* note, GRTrill* trill)
{
	if (!fFirstNote) fFirstNote = note;
	fTrills[note] = trill;
}

//-------------------------------------------------------------------------------
void ChordTrills::check (ChordTrills& ct, GRBar* bar)
{
//cerr << "ChordTrills::check " << endl;
//cerr << *this << "with " << endl;
//cerr << ct << endl;

	for (TTrills::iterator i = fTrills.begin(); i != fTrills.end(); i++) {
		TTrills::iterator link = ct.find(i->first);
		if (link != ct.end()) {
			GRNotationElement* elt = link->second->getBegin() ? next(link->first, bar) : link->second;
			i->second->setNextEvent(elt);
		}
		else {
			i->second->setNextEvent(next(ct.firstNote(), bar));
		}
	}
}

//-------------------------------------------------------------------------------
void ChordTrills::check (GREvent* ev, GRBar* bar)
{
	GRNotationElement* link = next(ev, bar);
	for_each (fTrills.begin(), fTrills.end(), [link] (pair<GRNote*, GRTrill*> elt)
		-> void { elt.second->setNextEvent(link);} );
}

//-------------------------------------------------------------------------------
GRNotationElement* ChordTrills::next(GREvent* note, GRBar* bar) const
{
	if (bar) return bar;
	return note;
}

//-------------------------------------------------------------------------------
void GRTrillLinker::visitStart (GRVoice* o)
{
	fCurrentTrills.clear();
	fLastTrill = 0;
	fLastBar = 0;
	fInChord = false;
}

void GRTrillLinker::visitEnd   (GRVoice* o)	{}

bool GRTrillLinker::sameNote (GRNote* n1, GRNote* n2) const
{
	const ARNote* prev = n1->getARNote();
	const ARNote* cur  = n2->getARNote();
	return (prev->getMidiPitch() == cur->getMidiPitch());
}

GRNotationElement* GRTrillLinker::next(GRNotationElement* o) const
{
	if (fLastBar) return fLastBar;
	return o;
}


void GRTrillLinker::visitStart (GRSingleNote* o)
{
	if (fInChord && fLastTrill) {
		fCurrent->add (o, fLastTrill);
	}
	else if (!fInChord && fPrevious) {
		fPrevious->check (o, fLastBar);
		delete fPrevious;
		fPrevious = 0;
		fLastBar = 0;
	}
	fLastTrill = 0;
}

void GRTrillLinker::visitStart (GRSingleRest* o)
{
	if (!fInChord && fPrevious) {
		fPrevious->check (o, fLastBar);
		delete fPrevious;
		fPrevious = 0;
		fLastBar = 0;
	}
	fLastTrill = 0;
}

void GRTrillLinker::visitStart (GREmpty* o)
{
	const ARNote* note = static_cast<const ARNote*>(o->getAbstractRepresentation());
	if (note->getOctave() == 0) {
		fCurrent = new ChordTrills;
		fInChord = true;
	}
	else if (note->getDuration() == DURATION_0) {
		if (fPrevious && fCurrent) {
			fPrevious->check(*fCurrent, fLastBar);
			delete fPrevious;
			fLastBar = 0;
		}
		fPrevious = fCurrent;
		fCurrent = 0;
		fInChord = false;
	}
}

void GRTrillLinker::visitStart (GRBar* bar)			{ fLastBar = bar; }
void GRTrillLinker::visitStart (GRTrill* trill)		{ fLastTrill = trill; }

