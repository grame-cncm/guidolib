#ifndef ARTextHarmony_H
#define ARTextHarmony_H

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

#include "ARFontAble.h"
#include "ARPositionTag.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

class TagParameterString;

/*@mkdoc

@group:Text

@tagname:\harmony
@tagalias:
@tagtype:P
@tagnotation:harmony marking
@tagdesc
@tagend

@params:
@param:text:string:the harmony string (e.g. C7):*none*:false
@param:position:string:above or below:below:false
@fontparams:
@paramdesc:
The harmony string may contain # and & characters. They are converted to sharp and flat symbols.

See the [jazz](@EXAMPLES/allthethings/) example.
@paramend

*/

/** \brief The text tag
 */
class ARTextHarmony : public ARFontAble, public ARPositionTag
{
	public:
		enum { kUndefined, kAbove, kBelow  };
					 ARTextHarmony( int position = kUndefined);
		virtual		~ARTextHarmony() {}

		virtual void		setTagParameters (const TagParameterMap& params);

		virtual const char* getText() const			{ return fText ? fText->getValue() : 0; }
		int 				position() const		{ return fPosition; }

		virtual const char*	getParamsStr() const	{ return kARTextHarmonyParams; };
		virtual const char*	getTagName() const		{ return "ARTextHarmony"; };
		virtual std::string getGMNName() const		{ return "\\harmony"; };

	private:
		const TagParameterString * fText = 0;
		int fPosition = kUndefined;
};


#endif
