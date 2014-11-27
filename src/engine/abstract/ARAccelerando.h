#ifndef ARAccelerando_H
#define ARAccelerando_H

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
#include "TagParameterString.h"
#include "TagParameterFloat.h"

class TagParameterString;

/** \brief The Accelerando position tag
*/
class ARAccelerando :  public ARMTParameter,  public ARPositionTag
{		
public:			

	virtual bool MatchEndTag(const char * s);

	ARAccelerando();

	virtual ~ARAccelerando();

	virtual void setTagParameterList(TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	TagParameterString * getTempo()             { return tempo;}
	TagParameterString * getAbsTempo()          { return abstempo;}	
	const char* getFont() const	                { return font ? font->getValue() : 0; }
	const char*	getFAttrib() const              { return fattrib ? fattrib->getValue() : 0; }
	int			getFSize(float curLSPACE) const { return fsize ? (int) fsize->getValue(curLSPACE) : 0;}

protected:
	// TagParameters ..
	TagParameterString *tempo;
	TagParameterString *abstempo;
	TagParameterString *font;
	TagParameterString *fattrib;
	TagParameterFloat  *fsize;

	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;
};

#endif
