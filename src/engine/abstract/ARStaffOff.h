#ifndef ARStaffOff_H
#define ARStaffOff_H

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


/*@mkdoc

@group:Layout

@tagname:\staffOff
@tagalias:
@tagtype:P
@tagnotation:hides the staff
@tagdesc
The **\staffOff** tag effect is to hide the staff and all the staff elements up to the next **\staffOn** tag.

The **\staffOn** and **\staffOff** tags are still experimental and may not provide the expected result depending on the context.
@tagend

*/

/** \brief not yet documented
*/
class ARStaffOff : public ARMTParameter
{
	public:
//		friend class ARFactory;

					 ARStaffOff() { }
		virtual	    ~ARStaffOff() { }

		virtual int	 getOrder() const		{ return kDontMove; }

		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARStaffOff"; };
		virtual std::string getGMNName() const		{ return "\\staffOff"; };
};

#endif
