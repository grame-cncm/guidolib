#ifndef ARGlissando_H
#define ARGlissando_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/*@mkdoc

@group:Articulations

@tagname:\glissando
@tagalias:glissandoBegin glissandoEnd
@tagtype:R
@tagnotation:a glissando line between notes
@tagdesc
The **\glissando** tag displays lines between notes but can be applied to chords as well.
In the latter case, a line is drawn between the corresponding notes of each chord.
It is the position of the notes in the GMN code that determines the correspondence
@tagend

@params:
@param:dx1:unit:displacement of the line left anchor point:0:true
@param:dy1:unit:displacement of the line left anchor point:0:true
@param:dx2:unit:displacement of the line right anchor point:0:true
@param:dy2:unit:displacement of the line right anchor point:0:true
@param:fill:boolean@param:fill space between lines when applied to a chord:false:true
@param:thickness:unit:line thickness:0.3:true
@paramdesc
See a [glissando](/examples/articulations/) example.
@paramend

*/

class ARGlissando : public ARMTParameter, public ARPositionTag
{
	public:
					ARGlissando();
					ARGlissando(const ARGlissando * glissando);
		virtual	   ~ARGlissando() {};
	
		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARGlissandoParams; };
		virtual const char*	getTagName() const		{ return "ARGlissando"; };
		virtual std::string getGMNName() const		{ return "\\glissando"; };

		virtual void browse(TimeUnwrap& mapper) const;
		virtual bool MatchEndTag(const char * s);

		const float getDx1()       const { return fDx1; }
		const float getDy1()       const { return fDy1; }
		const float getDx2()       const { return fDx2; }
		const float getDy2()       const { return fDy2; }
		const float getThickness() const { return fThickness; }
		const bool  isFill()	   const { return fFill; }
		//const bool  isWavy()             { return fWavy; }
	
	protected:
		float fDx1;
		float fDy1;
		float fDx2;
		float fDy2;
		float fThickness;
		bool  fFill;
		//bool  fWavy;
};

#endif
