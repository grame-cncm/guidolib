#ifndef ARInstrument_H
#define ARInstrument_H

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

class TagParameterString;

/** \brief not yet documented
*/
class ARInstrument : public ARMTParameter
{
	public:

						ARInstrument();	
		virtual 		~ARInstrument();	

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual void print() const;

		const char* getName() const;
		const char* getSecondName() const;
		const TagParameterString * getPar1() const	 	{ return s1; }
		const TagParameterString *getPar2() const	 	{ return s2; }
		const TagParameterString *getTransp() const	 	{ return transp; }
		virtual bool IsStateTag() const 				{ return true; }
	
		virtual std::ostream &operator<<(std::ostream &os) const;

	protected:

		TagParameterString *s1;
		TagParameterString *s2;
		TagParameterString *transp;

		static ListOfTPLs ltpls;
};

#endif
