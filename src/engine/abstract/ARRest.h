#ifndef ARRest_H
#define ARRest_H

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

#include "ARMusicalEvent.h"

/** \brief Abstract representation of the rest event.
*/
class ARRest : public ARMusicalEvent 
{
	public:
				 ARRest (const TYPE_DURATION & duration)
					: ARMusicalEvent(MIN_TIMEPOSITION,(TYPE_DURATION) duration) {}

				 ARRest (const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration)
					: ARMusicalEvent((TYPE_TIMEPOSITION) date,(TYPE_DURATION) duration) {}
					
				 ARRest (int num, int denum) : ARMusicalEvent(num,denum) {}
	
				 ARRest (const ARRest &arrest);
		virtual ~ARRest() {}

		virtual ARMusicalObject * Copy() const		{ return new ARRest(*this); }

		virtual void print(std::ostream & os) const { os << getGMNName(); }
		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARRest"; };
		virtual std::string getGMNName() const;

		virtual void browse(TimeUnwrap& mapper) const;
		virtual void forceNoteAppearance(NVstring appearance) { fAppearance = appearance; };
		virtual NVstring getAppearance() const				  { return fAppearance; }

		ARMusicalObject *isARRest() 				{ return this; }

	private:
		NVstring fAppearance;
};;

#endif


