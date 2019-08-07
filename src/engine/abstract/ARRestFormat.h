#ifndef ARRestFormat_H
#define ARRestFormat_H

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

@group:Notes

@tagname:\restFormat
@tagalias:
@tagtype:RP
@tagnotation:control the rest format
@tagdesc
The **\restFormat** tag supports both a range and a position form.
For the latter, the format is applied up to the next format specification or to the end of the voice.

**\restFormat** is a way to introduce [common parameters](../../tagsparams#common-parameters) to rest.
@tagend

*/


/** \brief not yet documented
*/
class ARRestFormat : public ARMTParameter, public ARPositionTag
{
	public:
				 ARRestFormat(const ARRestFormat *p_saverf = NULL, const ARRestFormat *copyrf = NULL)
					: fSaveRF(p_saverf)
				 { rangesetting = ARMusicalTag::RANGEDC; }


		virtual ~ARRestFormat() {}

		virtual bool IsStateTag() const				{ return true; }
	
		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARRestFormat"; };
		virtual std::string getGMNName() const		{ return "\\restFormat"; };

		virtual ARRestFormat *getEndTag() const { return new ARRestFormat(NULL,fSaveRF); }

	protected:
		const ARRestFormat * fSaveRF;
};

#endif 
