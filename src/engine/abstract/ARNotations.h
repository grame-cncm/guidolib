#ifndef ARNotations_H
#define ARNotations_H

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

/** \brief The Pedal tag
*/
class ARNotations : public ARMTParameter
{
	public:
	enum  { kPedalBegin, kPedalEnd };

					 ARNotations(int type) : fType(type) {}
		virtual		~ARNotations() {}

		virtual const char*	getTagName() const		{ return "ARNotations"; }
		virtual std::string getGMNName() const;
	
		int getType() const 	{ return fType; }

	private:
		int fType;
};

#endif
 
