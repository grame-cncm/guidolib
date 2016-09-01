#ifndef ARRepeatEnd_H
#define ARRepeatEnd_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

// #include <iosfwd>

#include "ARPositionTag.h"
#include "ARBar.h"
//#include "ARMTParameter.h"
// #include "ARMusicalObject.h"

class ARFactory;
class ARRepeatBegin;

/** \brief not yet documented
*/
//class ARRepeatEnd : public ARMTParameter, public ARPositionTag
//class ARRepeatEnd : public ARBar, public ARPositionTag
class ARRepeatEnd : public ARBar
{
		friend class ARFactory;
		friend class ARRepeatBegin;

	public:
					ARRepeatEnd(ARRepeatBegin * curbeg = 0);
		virtual		~ARRepeatEnd() { }

		virtual void setTagParameterList( TagParameterList & tpl);

	    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void browse(TimeUnwrap& mapper) const;

        /**** Function to avoid dynamic_cast ****/
        ARMusicalObject *isARRepeatEnd() { return this; }
        /****************************************/

	protected:
		static ListOfTPLs ltpls;
		ARRepeatBegin * repbeg;
};

#endif
