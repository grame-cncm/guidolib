#ifndef ARCue_H
#define ARCue_H

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
#include "ARFontAble.h"

class TagParameterString;

/*@mkdoc

@group:Notes

@tagname:\cue
@tagalias:
@tagtype:R
@tagnotation:cue notes
@tagdesc
Regarding the notes, the **\cue** tag is equivalent to **\noteFormat<color="blue", size=0.6>** tag.
Cue notes are colored in blue by default. You can override that with  [**\noteFormat**](#noteFormat)
@tagend

@params:
@param:name:string:the cue description:*none*:true
@fontparams:
@paramdesc
@paramend

*/

/** \brief not yet documented
*/
class ARCue : public ARFontAble, public ARPositionTag
{
	public:
				 ARCue();
		virtual ~ARCue() {}

		virtual const char*	getParamsStr() const	{ return kARCueParams; };
		virtual const char*	getTagName() const		{ return "ARCue"; };
		virtual std::string getGMNName() const		{ return "\\cue"; };

		const TagParameterString * getName() const;
};

#endif
