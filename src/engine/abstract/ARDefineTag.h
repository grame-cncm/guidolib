#ifndef ARDefineTag_H
#define ARDefineTag_H

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
class ARDefineTag : public ARMTParameter  
{
public:
	ARDefineTag();
	virtual void print(int &indent) const {};

	virtual ~ARDefineTag();
	virtual void setTagParameterList(TagParameterList & tpl);
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;

protected:
	TagParameterString * tagname;
	TagParameterString * tagparameters;
	TagParameterString * ggstext;
	static ListOfTPLs ltpls;
};

#endif
