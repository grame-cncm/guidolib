#ifndef ARArpeggio_H
#define ARArpeggio_H

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
#include "ARPositionTag.h"

/*@mkdoc

@group:Ornaments

@tagname:\arpeggio
@tagalias:
@tagtype:R
@tagnotation:an arpeggio sign
@tagdesc
The **\arpeggio** tag displays arpeggio signs and should be applied to chords only.
It has no effect on single notes.
@tagend

@params:
@param:direction:string:up or down:*none*:true
@paramdesc
- up: to display an up arrow
- down: to display a down arrow

See the [Articulations](@EXAMPLES/articulations/) example.
@paramend

*/

/** \brief Abstract representation of aa arpeggio.
*/
class ARArpeggio :  public ARMTParameter, public ARPositionTag
{
	public:
				 ARArpeggio();
		virtual ~ARArpeggio() {}

		enum dir { kUnknown, kUp, kDown };

		virtual bool MatchEndTag(const char * s);

		virtual const char*	getParamsStr() const	{ return kARArpeggioParams; };
		virtual const char*	getTagName() const		{ return "ARArpeggio"; };
		virtual std::string getGMNName() const		{ return "\\arpeggio"; };

		virtual void setTagParameters (const TagParameterMap& params);
	
		dir getDirection() const 		{ return fDirection; }

	protected:
		dir fDirection;
};

#endif

