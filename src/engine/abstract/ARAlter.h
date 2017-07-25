#ifndef ARAlter_H
#define ARAlter_H

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

#include <string>

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief The Alter tag
*/
class ARAlter : public ARMTParameter, public ARPositionTag
{
	public:
				     ARAlter(const ARAlter * p_savealter = NULL, const ARAlter * copyalter = NULL);
		virtual     ~ARAlter() {}

		virtual bool IsStateTag() const				{ return true; }

		virtual std::string getAlterText() const;
		virtual float		getDetune() const;

		virtual const char*	getParamsStr() const	{ return kARAlterParams; };
		virtual const char*	getTagName () const		{ return "ARAlter"; };
		virtual std::string getGMNName () const		{ return "\\alter"; };

		virtual ARAlter * getEndTag() const			{ return new ARAlter(NULL, fSaveAlter); }

	private:
		const ARAlter * fSaveAlter;
};

#endif
