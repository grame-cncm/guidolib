#ifndef ARAccol_H
#define ARAccol_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"

/** \brief Accolade tag to group staves into accolades.
*/
class ARAccolade : public ARMTParameter
{
	public:
				 ARAccolade();
		virtual ~ARAccolade() {}

		const TagParameterString * getIDString() const   { return getParameter<TagParameterString>(kIDStr); }
		const TagParameterInt *	   getIDInt() const      { return getParameter<TagParameterInt>(kIDStr); }
		const TagParameterString * getAccolRange() const { return getParameter<TagParameterString>(kRangeStr); }
		const TagParameterString * getType() const       { return getParameter<TagParameterString>(kTypeStr); }

		virtual bool IsStateTag() const                  { return true; }

		virtual int	 getOrder() const				{ return kAccoladeOrder; }
		virtual const char*	getParamsStr() const	{ return kARAccoladeParams; };
		virtual const char*	getTagName () const		{ return "ARAccolade"; };
		virtual std::string getGMNName () const		{ return "\\accolade"; };
};

#endif 
