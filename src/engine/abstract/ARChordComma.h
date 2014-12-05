#ifndef ARChordComma_H
#define ARChordComma_H

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
class ARChordComma : // public ARMusicalObject, 
	public ARMTParameter
{
public:
	
	ARChordComma(const ARChordComma *cc);

	virtual ARMusicalObject *Copy() const;

		     ARChordComma();
	virtual ~ARChordComma();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const;

	virtual void setTagParameterList(TagParameterList & theTagParameterList);
	virtual void browse(TimeUnwrap& mapper) const;
        
    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARChordComma() { return this; }
    /****************************************/

protected:
	static ListOfTPLs ltpls;
};

#endif
