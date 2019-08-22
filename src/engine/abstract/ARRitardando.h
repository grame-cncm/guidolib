#ifndef ARRitardando_H
#define ARRitardando_H

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

//#include "ARFontAble.h"
//#include "ARPositionTag.h"
#include "TempoChange.h"

//class TagParameterString;

/*@mkdoc

@group:Tempo

@tagname:\ritardando
@tagalias:\rit <br /> \ritBegin \ritEnd
@tagtype:R
@tagnotation:ritardando marks
@tagdesc
Works similarly to [\accelerando](#accelerando)
@tagend

@params:
@param:before:string:a string placed before 'accel.':*none*:true
@param:after:string:a string placed after the accel range:*none*:true
@fontparams:
@paramdesc
- **before** and **after** ara arbitrary strings that may contain a marker for note duration in the form "[n/d]" where 'n' and 'd' are integers.
The corresponding mark is decoded as a note duration and replaced with the corresponding note symbol. <br/>Example: *before="[1/4] = 80"*

See the [Tempo](@EXAMPLES/tempo/) example. <br />
See the [Lutkin](@EXAMPLES/lutkin/) example.
@paramend

*/

/** \brief Ritardando tag
*/

//class ARRitardando : public ARFontAble,  public ARPositionTag
class ARRitardando : public TempoChange
{
	public:			

		virtual bool MatchEndTag(const char * s);

						 ARRitardando();
		virtual 		~ARRitardando() {}

//		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARRitardandoParams; };
		virtual const char*	getTagName() const		{ return "ARRitardando"; };
		virtual std::string getGMNName() const		{ return "\\ritardando"; };

//		const TagParameterString * getTempo() const      { return getParameter<TagParameterString>(kTempoStr, true); }
//		const TagParameterString * getAbsTempo() const   { return getParameter<TagParameterString>(kAbsTempoStr, true);}
};

#endif
