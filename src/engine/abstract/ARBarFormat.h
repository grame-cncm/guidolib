#ifndef ARBarFormat_H
#define ARBarFormat_H

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
class TagParameterString;

/** \brief the bar format tag parameter.
*/
class ARBarFormat : public ARMTParameter
{
public:
	ARBarFormat();
	ARBarFormat(const ARBarFormat&);
	virtual ~ARBarFormat();

	virtual bool IsStateTag() const { return true; }
	
	virtual void setTagParameterList( TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	const TagParameterString *getStyle() const { return style; }

protected:
	TagParameterString *	style;
	static ListOfTPLs 		ltpls;

};

#endif 
