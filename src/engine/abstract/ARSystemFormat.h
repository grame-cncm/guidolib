#ifndef ARSystemFormat_H
#define ARSystemFormat_H

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

/*@mkdoc

@group:Layout

@tagname:\systemFormat
@tagalias:
@tagtype:P
@tagnotation:sets the system format
@tagdesc
@tagend

The **\systemFormat** tag is mainly used for indentation of the first system.

@params:
@param:dx:unit:set an horizontal offset to the first system:0hs:true
@paramdesc
Note: the **\systemFormat** tag supports common parameters but only dx has an effect.

See the [Mozart](@EXAMPLES/mozart581/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARSystemFormat : public ARMTParameter
{
	public:
						 ARSystemFormat() {}
		virtual 		~ARSystemFormat() {}

		virtual int	 getOrder() const		{ return kSystemFormatOrder; }
		virtual bool IsStateTag() const 	{ return true; }
	
		virtual const char*	getTagName() const		{ return "ARSystemFormat"; };
		virtual std::string getGMNName() const		{ return "\\systemFormat"; };
};

#endif 
