#ifndef ARChordTag_H
#define ARChordTag_H

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

class TagParameterString;
class TagParameterInt;

/** \brief not yet documented
*/
class ARChordTag : public ARMTParameter, public ARPositionTag
{
	public:
				 ARChordTag()	{ rangesetting = ONLY; }
		virtual ~ARChordTag()	{}

		virtual ARMusicalObject * Copy() const		{ return new ARChordTag(); }

		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARChordTag"; };
		virtual std::string getGMNName() const		{ return "\\chordTag"; };

		ARMusicalObject *isARChordTag()				{ return this; }
};


#endif
