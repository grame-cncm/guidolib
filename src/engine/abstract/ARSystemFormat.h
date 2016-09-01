#ifndef ARSystemFormat_H
#define ARSystemFormat_H

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

/** \brief not yet documented
*/
class ARSystemFormat : public ARMTParameter
{
public:
					ARSystemFormat();
	virtual 		~ARSystemFormat();

	virtual bool IsStateTag() const { return true; }
	
	virtual void setTagParameterList( TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

protected:
	
	static ListOfTPLs ltpls;

};

#endif 
