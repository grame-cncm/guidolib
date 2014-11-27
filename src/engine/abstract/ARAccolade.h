#ifndef ARAccol_H
#define ARAccol_H

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

class TagParameterInt;

/** \brief Accolade tag to group staves into accolades.
*/
class ARAccolade : public ARMTParameter
{
public:
	ARAccolade();
	virtual ~ARAccolade();

	const TagParameterString * getIDString() const   { return sid; }
	const TagParameterInt * getIDInt() const         { return nid; }
	const TagParameterString * getAccolRange() const { return range; }
	const TagParameterString * getType() const       { return type; }
	virtual bool IsStateTag() const                  { return true; }
	
	virtual void setTagParameterList( TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

protected:
	TagParameterInt * 		nid;
	TagParameterString *	sid;

	TagParameterString *	range;
	TagParameterString *	type;

	static ListOfTPLs 		ltpls;
};

#endif 
