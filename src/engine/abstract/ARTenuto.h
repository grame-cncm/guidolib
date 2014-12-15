#ifndef ARTenuto_H
#define ARTenuto_H

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

// for TagParameters
//#include "TagParameterString.h"
//#include "TagParameterFloat.h"
//#include "TagParameterInt.h"


/** \brief the Tenuto articulation tag
*/
class ARTenuto : public ARMTParameter, public ARPositionTag
{		
public:		
	ARTenuto() : ARMTParameter() { rangesetting = ONLY; }

    virtual ~ARTenuto() {};

	virtual void setTagParameterList(TagParameterList& tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void browse(TimeUnwrap& mapper) const;

protected:
	// TagParameters ..
	// TagParameterString *textformat;
	// TagParameterFloat  *fsize;
	// TagParameterInt	  *fnumber;


	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;
};

#endif
