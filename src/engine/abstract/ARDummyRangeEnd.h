#ifndef ARDummyRangeEnd_H
#define ARDummyRangeEnd_H

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

#include "ARTagEnd.h"

/** \brief not yet documented
*/
class ARDummyRangeEnd : public ARTagEnd
{
	public:
				 ARDummyRangeEnd(const ARDummyRangeEnd * copy)
					: ARTagEnd(-1,copy)					{ if (copy) endstr = copy->endstr; }
				 ARDummyRangeEnd(std::string txt = "")	{ endstr = txt.size() ? txt : ")"; }

		virtual ~ARDummyRangeEnd() {}

		virtual ARMusicalObject * Copy() const		{ return new ARDummyRangeEnd(this); }
		virtual const char*	getTagName() const		{ return "ARDummyRangeEnd"; };
		virtual std::string getGMNName() const		{ return endstr; };

	    const ARMusicalObject *isARDummyRangeEnd() const { return this; }
	  	std::string endstr;
};

#endif
