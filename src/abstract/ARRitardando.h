#ifndef ARRitardando_H
#define ARRitardando_H

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

class TagParameterString;

/** \brief Ritardando tag
*/

class ARRitardando : public ARMTParameter,  public ARPositionTag
{		
public:			

	virtual bool MatchEndTag(const char * s);

					ARRitardando();
	virtual 		~ARRitardando();

	virtual void setTagParameterList(TagParameterList & tpl);

	// routines to access TagParameters ..
	virtual void print() const;
	
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	

protected:

	// TagParameters ..
	TagParameterString * s1;
	TagParameterString * s2;

	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;

};

#endif
