#ifndef ARShareStem_H
#define ARShareStem_H

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

/** \brief not yet documented
*/
class ARShareStem : public ARMTParameter, public ARPositionTag
{
	public:
				 ARShareStem(const ARShareStem * stm)
					: ARMTParameter(-1,stm)	{ rangesetting = ONLY; }

				 ARShareStem() { rangesetting = ONLY; }
		virtual ~ARShareStem() {}

		virtual ARMusicalObject * Copy() const		{ return new ARShareStem(this); }
		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARShareStem"; };
		virtual std::string getGMNName() const		{ return "\\shareStem"; };
};

#endif
