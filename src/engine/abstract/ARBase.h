#ifndef ARBase_H
#define ARBase_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMusicalTag.h"
#include "ARPositionTag.h"
#include "ARMusicalEvent.h"

/** \brief not yet documented
*/
class ARBase :  public ARMusicalTag, public ARPositionTag
{
public:
			 ARBase() { rangesetting = ONLY; setAssociation(ARMusicalTag::RA); }
	virtual	~ARBase() {}

	virtual std::string getGMNName() const			{ return "\\ARBase(" + std::string(dur) + ", " + std::string(base) + ")"; };

	void setBaseDuration(const TYPE_DURATION & tmp) { dur = tmp; }
	void setBase(const TYPE_DURATION & tmp)         { base = tmp; }
	const TYPE_DURATION & getBaseDuration() const   { return dur; }
	const TYPE_DURATION & getBase() const           { return base; }

	void addEvent(const ARMusicalEvent * ev)		{ dur += ev->getDuration(); }

	// called, to finish a base. this calculates the equivalence ....
	// different algorithms can be used (hindemith ...)
	// right now, the next smaller note is taken
	void finish()		{}

private:
	// the parameters ...
	TYPE_DURATION dur;		// the dur of this tuplet
	TYPE_DURATION base;		// the base of this tuplet
	// that is, a note of duration equiv1 is displayed
	// as a note of duration equiv2,
	// sample: 1/6 == 1/4 means, that a note of duration
	// 1/6 is displayed as a quarter note.
};

#endif
