#pragma once

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


#include "TempoChange.h"

/*@mkdoc

@group:Tempo

@tagname:\accelerando
@tagalias:\accel <br /> \accelBegin \accelEnd
@tagtype:R
@tagnotation:accelerando marks
@tagdesc
@tagend

@params:
@param:before:string:a string placed before 'accel.':*none*:true
@param:after:string:a string placed after the accel range:*none*:true
@param:dx2:unit:displacement of the right anchor point:0:true
@fontparams:
@paramdesc
- **before** and **after** ara arbitrary strings that may contain a marker for note duration in the form "[n/d]" where 'n' and 'd' are integers.
The corresponding mark is decoded as a note duration and replaced with the corresponding note symbol. <br/>Example: *before="[1/4] = 80"*

See the [Tempo](@EXAMPLES/tempo/) example.
@paramend

*/

/** \brief The Accelerando position tag
*/
//class ARAccelerando :  public ARFontAble,  public ARPositionTag
class ARAccelerando :  public TempoChange
{
	public:			

		virtual bool MatchEndTag(const char * s);

				 ARAccelerando();
		virtual ~ARAccelerando() {}

		virtual const char*	getParamsStr() const	{ return kARAccelerandoParams; };
		virtual const char*	getTagName () const		{ return "ARAccelerando"; };
		virtual std::string getGMNName () const		{ return "\\accelerando"; };

};
