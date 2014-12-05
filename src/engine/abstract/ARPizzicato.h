#ifndef ARPizz_H
#define ARPizz_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/*brief	The pizzicato articulation tag */

class ARPizzicato : public ARMTParameter, public ARPositionTag
{
public :
	enum			tTypePizz {SNAP, BUZZ, FINGERNAIL, LEFTHAND};

	ARPizzicato() : ARMTParameter() {
		rangesetting = ONLY;
		type = LEFTHAND;
	}

	virtual		~ARPizzicato() { };
	
	virtual void setTagParameterList(TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;
	
	tTypePizz	 getType() const {return type;}
	
protected:

	static ListOfTPLs ltpls;
	tTypePizz type;
	
};

#endif


