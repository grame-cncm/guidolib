#ifndef ARSlur_H
#define ARSlur_H

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

#include "ARBowing.h"

/*@mkdoc

@group:Articulations

@tagname:\slur
@tagalias:\sl <br />slurBegin \slurEnd
@tagtype:R
@tagnotation:slur
@tagdesc
@tagend

@params:
@param:curve:string:up or down:*none*:true
@param:dx1:unit:displacement of the left anchor point:2hs:true
@param:dy1:unit:displacement of the left anchor point:1hs:true
@param:dx2:unit:displacement of the right anchor point:-2hs:true
@param:dy2:unit:displacement of the right anchor point:1hs:true
@param:r3:float:displacement of the inflection point:0.5:true
@param:h:unit:height of the curve:2hs:true
@paramdesc
The **r3** parameter should be between 0 (leftmost) and 1 (rightmost).

Note that using **h** parameter while **curve** direction is also specified may be conflicting.

See the [Slurs](@EXAMPLES/slurs/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARSlur : public ARBowing
{
	public:
				 ARSlur() {}
				 ARSlur(const ARSlur * slr) : ARBowing(slr) {}
		virtual ~ARSlur() {}

		virtual ARMusicalObject * Copy() const		{ return new ARSlur(this); }
		virtual bool MatchEndTag(const char *s);

		virtual const char*	getTagName() const		{ return "ARSlur"; };
		virtual std::string getGMNName() const		{ return "\\slur"; };

    	virtual void browse(TimeUnwrap& mapper) const;
};


#endif


