#ifndef ARBase_H
#define ARBase_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/
#include "ARMusicalTag.h"
#include "ARPositionTag.h"

class ARMusicalEvent;

/** \brief not yet documented
*/
class ARBase :  public ARMusicalTag, public ARPositionTag
{
public:

					ARBase();
	virtual			~ARBase() {}

	virtual void print() const { }

	virtual void PrintName( std::ostream & os) const;
	virtual void PrintParameters( std::ostream & os) const;


	void setBaseDuration(const TYPE_DURATION & tmp)
		{ dur = tmp; }

	void setBase(const TYPE_DURATION & tmp)
		{ base = tmp; }

	const TYPE_DURATION & getBaseDuration() const
		{ return dur; }

	const TYPE_DURATION & getBase() const
		{ return base; }

	void addEvent(const ARMusicalEvent * ev);

	// called, to finish a base. this calculates
	// the equivalence ....
	// different algorithms can be used (hindemith ...)
	// right now, the next smaller note is taken
	void finish()
		{
			// needs to be revised ...
			// TYPE_DURATION tmp(1,2);
			// equiv2 = equiv1 * base * tmp;
		}

protected:

	// the parameters ...
	TYPE_DURATION dur; // the dur of this tuplet
	TYPE_DURATION base;   // the base of this tuplet
	// that is, a note of duration equiv1 is displayed
	// as a note of duration equiv2,
	// sample: 1/6 == 1/4 means, that a note of duration
	// 1/6 is displayed as a quarter note.

};


#endif
