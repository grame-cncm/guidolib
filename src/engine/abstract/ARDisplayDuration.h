#ifndef ARDisplayDuration_H
#define ARDisplayDuration_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"

class TagParameterInt;

/** \brief not yet documented
*/
class ARDisplayDuration : public ARMTParameter, public ARPositionTag
{
	public:

				 ARDisplayDuration(const ARDisplayDuration * dspdur);
				 ARDisplayDuration();
		virtual ~ARDisplayDuration() {}

		virtual bool MatchEndTag(const char * s);

		virtual ARMusicalObject * Copy() const		{ return new ARDisplayDuration(this); }

		virtual const char*	getParamsStr() const	{ return kARDisplayDurationParams; };
		virtual const char*	getTagName() const		{ return "ARDisplayDuration"; };
		virtual std::string getGMNName() const		{ return "\\displayDuration"; };

		virtual void setTagParameters (const TagParameterMap& params);
	
		void setDisplayDuration(const TYPE_DURATION & tmp);
		void setDisplayDuration(const TYPE_DURATION & tmp, int numdots) { 
			fDuration = tmp;
			fDots = numdots; 
		}

		TYPE_DURATION	getDisplayDuration() const	{ return fDuration; }
		int				getDots() const				{ return fDots; }
	
		ARMusicalObject *isARDisplayDuration() { return this; }

	private:
		// the parameters ...
		TYPE_DURATION	fDuration; // the dur of this tuplet
		int				fDots;
};

#endif
