#ifndef ARIntens_H
#define ARIntens_H

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

#include <string>

#include "ARFontAble.h"


/*@mkdoc

@group:Dynamics

@tagname:\intensity
@tagalias:\intens \i
@tagtype:P
@tagnotation:dynamic marks
@tagdesc
@tagend


@params:
@param:type:string:a dynamic string:*none*:false
@param:before:string:a string displayed before the dynamic mark:*none*:true
@param:after:string:a string displayed after the dynamic mark:*none*:true
@param:autopos:boolean:when 'on', try to automatically avoid collisions:off:true
#fontparams:
@paramdesc
- supported dynamic strings are "p", "pp", "ppp", "pppp", "f", "ff", "fff", "ffff", "mf", "mp", "sf", "sfz", "rfz" and "fz"

See the [Dynamics](@EXAMPLES/dynamics/) example.
@paramend

*/

/** \brief The intensity tag.
*/
class ARIntens :  public ARFontAble
{
	public:
				 ARIntens(const char * txt);
				 ARIntens(bool autopos);
		virtual ~ARIntens() {}

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const std::string& getText() const 			{ return fIntens; }
		virtual const std::string& getTextBefore() const 	{ return fBefore; }
		virtual const std::string& getTextAfter() const 	{ return fAfter; }

		virtual const char*	getParamsStr() const	{ return kARIntensParams; };
		virtual const char*	getTagName() const		{ return "ARIntens"; };
		virtual std::string getGMNName() const		{ return "\\intens"; };

		virtual void browse(TimeUnwrap& mapper) const;

		bool	  autoPos() const 					{ return fAutoPos; }

	protected:
		std::string fIntens;
		std::string fBefore;
		std::string fAfter;
		bool 		fAutoPos;
};

#endif
