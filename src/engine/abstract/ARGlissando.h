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
