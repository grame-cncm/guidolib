/*
  GUIDO Library
  Copyright (C) 2016 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "TCollisions.h"

#include "ARMusicalObject.h"
#include "ARSpace.h"
#include "GRNotationElement.h"
#include "GRARNotationElement.h"
#include "GRSystemSlice.h"
#include "GRBar.h"

using namespace std;

// ----------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& os, const TCollisions& c)	{ c.print(os); return os; }

// ----------------------------------------------------------------------------
void TCollisions::print(ostream& os) const
{
	size_t n = fCollisions.size();
	for (size_t i=0; i<n; i++) {
		os << fCollisions[i] << endl;
	}
}

// ----------------------------------------------------------------------------
void TCollisionInfo::print(ostream& os) const
{
	if (fARObject) os << "voice " << fVoice << ": " << fARObject->getRelativeTimePosition() << " " << fARObject << " -> " <<  fSpace;
	else os << "empty";
}
std::ostream& operator<< (std::ostream& os, const TCollisionInfo* ci) { ci->print(os); return os; }
std::ostream& operator<< (std::ostream& os, const TCollisionInfo& ci) { ci.print(os); return os; }

// ----------------------------------------------------------------------------
TCollisions::TCollisions () : fLastSlice(0), fStaff(-1), fSystem(-1) {}

// ----------------------------------------------------------------------------
void TCollisions::setStaff (int num)			{ fStaff = num; }
void TCollisions::setSystem (int num)			{ fSystem = num; }
const NVPoint TCollisions::yOffset() const		{ return NVPoint(0, LSPACE * 36 * getStaff()); }

// ----------------------------------------------------------------------------
void TCollisions::reset (bool resetSystem)
{
	if (resetSystem) fSystem = -1;
	fStaff = -1;
	fLastSlice = 0;
	clearElements();
}

// ----------------------------------------------------------------------------
void TCollisions::clear ()
{
	reset(true);
	fCollisions.clear();
}

// ----------------------------------------------------------------------------
void TCollisions::clearElements ()
{
	fLastElements.clear();
	fLastBB.clear();
}

// ----------------------------------------------------------------------------
bool TCollisions::check (const NVRect& r, bool slice)
{
	return slice ? checkSlice(r) : checkElement(r);
}

// ----------------------------------------------------------------------------
bool TCollisions::checkSlice (const NVRect& r)
{
	if (fLastSlice) {
		const NVRect& lastrect = fLastSliceBB;
		float gap = lastrect.Collides(r) ? lastrect.right - r.left : 0;
		if (gap > 0) {
			const GRBar* bar = fLastSlice->getBar ();
			if (bar) {
				resolve (bar->getAbstractRepresentation(), gap);
				return true;
			}
			else cerr << "TCollisionsState::resolve slice: no bar !" << endl;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void TCollisions::resolve (ARMusicalObject* ar, int gap)
{
	ARSpace * arspace = new ARSpace (gap);
	arspace->setRelativeTimePosition (ar->getRelativeTimePosition());
	TCollisionInfo ci (ar, ar->getVoiceNum(), arspace);
	fCollisions.push_back (ci);
}

// ----------------------------------------------------------------------------
bool TCollisions::checkElement (const NVRect& r)
{
	const GRNotationElement * last = lastElement();
	if (last) {
		const NVRect& lastrect = lastBB();
		const NVRect ry = r + yOffset();
		if (lastrect == ry) return false;		// likely duplicate elements like auto clefs

		float gap = lastrect.Collides(ry) ? lastrect.right - ry.left : 0;
		if (gap > 0) {
			const GRARNotationElement* el = dynamic_cast<const GRARNotationElement*>(last);
			if (el) {
				resolve (el->getAbstractRepresentation(), lastrect.Width());
				return true;
			}
			else cerr << "TCollisionsState::checkElement: NOT a GRARNotationElement: " << last << endl;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void TCollisions::update (const GRSystemSlice * slice, const NVRect& r)
{
	fLastSlice = slice;
	fLastSliceBB = r;
}

// ----------------------------------------------------------------------------
void TCollisions::update (const GRNotationElement * e, const NVRect& r)
{
	fLastElements[fStaff] = e;
	fLastBB[fStaff] = r + yOffset();
}

