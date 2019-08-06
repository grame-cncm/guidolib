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
@fontparams:
@paramdes:
**Note**: default settings (size, position) are still to be defined for the **\harmony** tag.
The current settings are therefore subject to change.
@paramend

*/

/** \brief The text tag
 */
class ARTextHarmony : public ARFontAble, public ARPositionTag
{
	public:
					 ARTextHarmony();
		virtual		~ARTextHarmony() {}

		virtual void		setTagParameters (const TagParameterMap& params);

		virtual const char* getText() const		{ return text ? text->getValue() : 0; }

		virtual const char*	getParamsStr() const	{ return kARTextHarmonyParams; };
		virtual const char*	getTagName() const		{ return "ARTextHarmony"; };
		virtual std::string getGMNName() const		{ return "\\harmony"; };

	protected:
		const TagParameterString * text; // der entsprechende Text ...
};


#endif
