#ifndef ARMarcato_H
#define ARMarcato_H

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

#include "ARArticulation.h"

/*@mkdoc

@group:Articulations

@tagname:\marcato
@tagalias:
@tagtype:RP
@tagnotation:marcato sign
@tagdesc
@tagend

@params:
@param:position:string:above or below:above:true
@paramdesc
Note that depending on the position, the glyph for the marcato sign is not the same.

See the [Articulations](@EXAMPLES/articulations/) example.
@paramend

*/

/** \brief The marcato articulation tag
*/
class ARMarcato : public ARArticulation
{		
	public:			
				 ARMarcato() {}
		virtual ~ARMarcato() {}

		virtual const char*	getTagName() const		{ return "ARMarcato"; };
		virtual std::string getGMNName() const		{ return "\\marcato"; };
};

#endif
