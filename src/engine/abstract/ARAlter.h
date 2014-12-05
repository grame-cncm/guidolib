#ifndef ARAlter_H
#define ARAlter_H

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

#include <string>

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief The Alter tag
*/
class ARAlter : public ARMTParameter, public ARPositionTag
{
	public:
				     ARAlter(ARAlter * p_savealter = NULL, ARAlter * copyalter = NULL); 
		virtual     ~ARAlter();

		virtual bool IsStateTag() const									{ return true; }
		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual const TagParameterString *getTPAlterString() const		{ return alterstring; }
		virtual const TagParameterFloat *getTPAlterValue() const		{ return altervalue; }

		virtual std::string getAlterText() const;
		virtual float getDetune() const;

	    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual ARAlter * getEndTag() const								{ return new ARAlter(NULL, savealter); }

	protected:
		ARAlter * savealter;

		TagParameterString * alterstring;
		TagParameterFloat * altervalue;
		static ListOfTPLs ltpls;
};

#endif
