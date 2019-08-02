#ifndef ARAlter_H
#define ARAlter_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/*@mkdoc

@group:Accidentals

@tagname:\alter
@tagalias:
@tagtype:RP
@tagnotation:micro-tonal accidentals
@tagdesc
The **\alter** tag may be used for micro-tonal accidentals. It supports common tag parameters.
It can be used with or without a range: in the latter, it takes effect until the next **\alter** tag; if a ranged alter is encountered in between, it is applied locally and the non-ranged alter continues to apply after that.
 
**Note**: the **\alter** tag has a cumulative effect: it is added to existing accidentals.
@tagend

@params:
@param:detune:float:a signed floating point value representing semi-tones:0:false
@paramdesc
See the [Accidentals](/examples/accidentals/) example.
@paramend

*/

/** \brief The Alter tag
*/
class ARAlter : public ARMTParameter, public ARPositionTag
{
	public:
				     ARAlter(const ARAlter * p_savealter = NULL, const ARAlter * copyalter = NULL);
		virtual     ~ARAlter() {}

		virtual bool IsStateTag() const				{ return true; }

		virtual std::string getAlterText() const;
		virtual float		getDetune() const;

		virtual const char*	getParamsStr() const	{ return kARAlterParams; };
		virtual const char*	getTagName () const		{ return "ARAlter"; };
		virtual std::string getGMNName () const		{ return "\\alter"; };

		virtual ARAlter * getEndTag() const			{ return new ARAlter(NULL, fSaveAlter); }

	private:
		const ARAlter * fSaveAlter;
};

#endif
