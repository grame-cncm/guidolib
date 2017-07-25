#ifndef ARDRRENZ_H
#define ARDRRENZ_H

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
// ARDrHoos.h: declaration/interface of class ARDrHoos.
//
//////////////////////////////////////////////////////////////////////

#include "ARMTParameter.h"

/** \brief not yet documented
*/
class ARDrRenz : public ARMTParameter
{
	public:
				 ARDrRenz();
		virtual ~ARDrRenz() {}

		virtual const char*	getParamsStr() const	{ return kARDrRenzParams; };
		virtual const char*	getTagName() const		{ return "ARDrRenz"; };
		virtual std::string getGMNName() const		{ return "\\DrRenz"; };

		int getInverse() const;
};

#endif
