#ifndef ARPizz_H
#define ARPizz_H

/*
 *  ARPizz.h
 *  GuidoSceneComposer
 *
 *  Created by Samuel Brochot on 26/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
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
	virtual void	print() const;
	virtual void	PrintName(std::ostream & os) const;
	virtual void	PrintParameters(std::ostream & os) const;
	
	tTypePizz		getType() const {return type;}
	
protected:

	static ListOfTPLs ltpls;
	tTypePizz type;
	
};

#endif


