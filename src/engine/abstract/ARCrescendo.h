#ifndef ARCrescendo_H
#define ARCrescendo_H

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


#include "ARDynamic.h"

/*@mkdoc

@group:Dynamics

@tagname:\crescendo
@tagalias:\cresc<br />\crescBegin \crescEnd
@tagtype:R
@tagnotation:crescendo sign
@tagdesc
@tagend


@params:
@param:dx1:unit:start position displacement:0:true
@param:dx2:unit:end position displacement:0:true
@param:deltaY:unit:control the open end of the hairpins:3:true
@param:thickness:unit:the line thickness:0.16:true
@param:autopos:string:when 'on', try to automatically avoid collisions:off:true
@paramdesc
See the [Dynamics](/examples/dynamics/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARCrescendo : public ARDynamic
{
	public:
					 ARCrescendo()	{}
					 ARCrescendo(const ARCrescendo* crescendo) : ARDynamic (crescendo) {}
		virtual		~ARCrescendo() {}

	    virtual bool MatchEndTag(const char * s);

		virtual const char*	getTagName() const		{ return "ARCrescendo"; };
		virtual std::string getGMNName() const		{ return "\\crescendo"; };
};

#endif
