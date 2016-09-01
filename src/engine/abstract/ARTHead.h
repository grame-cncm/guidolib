#ifndef ARTHead_H
#define ARTHead_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"
// #include "TagParameterFloat.h"

/** \brief not yet documented
*/
class ARTHead : public ARMTParameter, public ARPositionTag
{
	public:
		enum HEADSTATE { NOTSET, NORMAL, REVERSE, CENTER,  RIGHT, LEFT };

				 ARTHead(int st = ARTHead::NOTSET, ARTHead * p_savehead = NULL, ARTHead * p_copyhead = NULL);

				 ARTHead(const ARTHead & arthead);
		virtual ~ARTHead() { }

		ARMusicalObject * Copy() const; 

		virtual bool IsStateTag() const { return true; }

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

        virtual void setTagParameterList(TagParameterList& tpl) {};

		virtual const HEADSTATE getHeadState() const { return  headstate; }

		virtual ARMusicalObject * getEndTag() const	{ return new ARTHead(NOTSET,NULL,savehead);	}

	protected:
		ARTHead * savehead;
		HEADSTATE headstate;
		static ListOfTPLs ltpls;
};

#endif
