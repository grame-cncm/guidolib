#ifndef ARTremolo_H
#define ARTremolo_H

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

/** \brief not yet documented
*/

class ARTremolo : public ARMTParameter, public ARPositionTag
{		
public:			


					ARTremolo() : ARMTParameter(), mDur(0) 
						{ rangesetting = ONLY; }

	virtual 		~ARTremolo();

	virtual void setTagParameterList(TagParameterList & tpl);

	// routines to access TagParameters ..
	virtual void print() const;
	
	virtual void PrintName(std::ostream &os) const;
	virtual void PrintParameters(std::ostream &os) const;	

protected:

	// TagParameters ..
	// TagParameterString *textformat;
	// TagParameterFloat  *fsize;
	TagParameterInt	  * mDur;


	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;
};

#endif
