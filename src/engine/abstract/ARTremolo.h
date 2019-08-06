#ifndef ARTremolo_H
#define ARTremolo_H

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

class TagParameterInt;
class TagParameterFloat;
class TagParameterString;

/*@mkdoc

@group:Repeat Signs

@tagname:\tremolo
@tagalias:\trem <br />tremBegin tremEnd <br />tremoloBegin tremoloEnd
@tagtype:R
@tagnotation:displays a tremolo
@tagdesc
@tagend

@params:
@param:style:string:a strokes string:///:true
@param:speed:integer:the tremolo speed:32:true
@param:pitch:string:the pitch of the other note:*none*:true
@param:thickness:unit:the stroke thickness:0.75:true
@param:text:string:a string displayed over the strokes:*none*:true
@paramdesc
@paramend

*/

/** \brief not yet documented
*/

class ARTremolo : public ARMTParameter, public ARPositionTag
{		
	public:
				 ARTremolo();
				 ARTremolo(const ARTremolo * tremolo);
		virtual ~ARTremolo() {}

		virtual bool 	MatchEndTag(const char * s);
		bool	isSecondPitchCorrect() const;
		int		getNumberOfStrokes() const;

		const TagParameterString *	getStyle() const;
		const TagParameterInt *		getSpeed() const;
		const TagParameterString *	getPitch() const;
		const TagParameterFloat *	getThickness() const;
		const TagParameterString *	getText() const;
		
	 
		virtual const char*	getParamsStr() const	{ return kARTremoloParams; };
		virtual const char*	getTagName() const		{ return "ARTremolo"; };
		virtual std::string getGMNName() const		{ return "\\tremolo"; };

	private:
		void init();
};

#endif
