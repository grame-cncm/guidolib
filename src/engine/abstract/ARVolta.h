#ifndef __ARVolta__
#define __ARVolta__

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/*@mkdoc

@group:Repeat Signs

@tagname:\volta
@tagalias:\voltaBegin \voltaEnd
@tagtype:R
@tagnotation:support for multiple endings in repreated sections
@tagdesc
@tagend

@params:
@param:mark:string:the section mark, typically the ending number:*none*:false
@param:format:string:the volta format:*none*:true
@paramdesc
- the **format** is a string in the form "|-|", "|-", "-|" or "-", that describes the shape of the line over the repeated section.
@paramend

*/

/** \brief for different repeat endings
*/
class ARVolta : public ARMTParameter, public ARPositionTag
{
	public:	
					 ARVolta();
		virtual 	~ARVolta() {}

		virtual const char*	getParamsStr() const	{ return kARVoltaParams; };
		virtual const char*	getTagName() const		{ return "ARVolta"; };
		virtual std::string getGMNName() const		{ return "\\volta"; };

		virtual void browse (TimeUnwrap& mapper) const;

		virtual bool MatchEndTag(const char * s);
  		const char* getMark() const;
		const char* getFormat() const;
};

#endif
