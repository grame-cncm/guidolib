#ifndef ARHarmonic_H
#define ARHarmonic_H

/*
 *  ARHarmonic.h
 *  GuidoSceneComposer
 *
 *  Created by Samuel Brochot on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/*brief		The harmonic tag */

class ARHarmonic : public ARMTParameter, public ARPositionTag
{

public :
			ARHarmonic() : ARMTParameter()
			{
				rangesetting = ONLY;
			}
			virtual ~ARHarmonic()
			{
			};
			
			virtual void setTagParameterList(TagParameterList & tpl);
			
			virtual void print() const;
			virtual void PrintName(std::ostream & os) const;
			virtual void PrintParameters(std::ostream & os) const;
			
protected :
			static ListOfTPLs ltpls;
			
};

#endif
