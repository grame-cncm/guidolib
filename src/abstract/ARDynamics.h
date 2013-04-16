#ifndef ARDynamics_H
#define ARDynamics_H

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
// #include "ARNote.h"
#include "ARMTParameter.h"
#include "ARPositionTag.h"


/** \brief not yet documented
*/
class ARDynamics : 
	public ARMTParameter,
		//  public ARMusicalObject,
	public ARPositionTag
{
	public:

					ARDynamics();
		virtual 	~ARDynamics();

		virtual void print() const;
		virtual void setTagParameterList(TagParameterList& tpl);

		virtual const NVstring & getText() const { return text; }

		virtual float getValue() const { return (float) value; }

		int location; // 0 for upper, 1 fuer lower ... should be bool?

protected:

	float value;
	NVstring text;

	static ListOfTPLs ltpls;
};

#endif
