#ifndef AROctava_H
#define AROctava_H

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

class TagParameterInt;

/** \brief The Octava tag
*/
class AROctava : public ARMTParameter, public ARPositionTag
{
	public:
						AROctava(AROctava * p_saveoct = NULL, AROctava * copyoct = NULL);
		virtual			~AROctava();

		virtual bool IsStateTag() const { return true; }

		virtual ARMusicalObject * getEndTag() const { return new AROctava(NULL, saveoct); }

		virtual const TagParameterInt * getTPOctava() const { return noct; }

		virtual int getOctava() const;

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList& tpl);

	protected:
		AROctava * saveoct; // this is required to save
				// the current octava state ...
		TagParameterInt  * noct;
		static ListOfTPLs ltpls;
};

#endif
 
