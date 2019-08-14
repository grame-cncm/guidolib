#ifndef ARNewSystem_H
#define ARNewSystem_H

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

@tagname:\newLine
@tagalias:\newSystem
@tagtype:P
@tagnotation:force a line break
@tagdesc
See the [notes](@EXAMPLES/notes/) example. <br />
See the [Bartok](@EXAMPLES/bartok/) example.
@tagend

*/

/** \brief not yet documented
*/

class ARNewSystem : public ARMTParameter
{
	public:
				 ARNewSystem() {}
		virtual ~ARNewSystem() {}

		virtual const char*	getTagName() const		{ return "ARNewSystem"; };
		virtual std::string getGMNName() const		{ return "\\newSystem"; };

		ARMusicalObject *isARNewSystem()			{ return this; }
};


#endif

