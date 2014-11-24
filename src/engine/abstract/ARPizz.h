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

class ARPizz : public ARMTParameter, public ARPositionTag
{
public :
	enum			tTypePizz {SNAP, BUZZ, FINGERNAIL, LEFTHAND};

	ARPizz() : ARMTParameter()
	{
		rangesetting = ONLY;
		type = LEFTHAND;
	}
	virtual			~ARPizz() { };
	
	virtual void	setTagParameterList(TagParameterList & tpl);
	
	// Routines to access TagpParameters
	virtual void	print(int &indent) const;
	virtual void	PrintName(std::ostream & os) const;
	virtual void	PrintParameters(std::ostream & os) const;
	
	tTypePizz		getType() const {return type;}
	
protected:

	static ListOfTPLs ltpls;
	tTypePizz type;
	
};

#endif


