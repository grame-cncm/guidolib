#ifndef ARColor_H
#define ARColor_H

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

/** \brief The color tag. Ready to support the Alpha componenent for transparency (RGBA).
*/

class ARColor : public ARMTParameter 
{
	public:
				 ARColor();
		virtual ~ARColor() {}

		virtual void setTagParameters (const TagParameterMap& params);

		virtual bool IsStateTag() const				{ return true; }
		virtual const char*	getParamsStr() const	{ return kARColorParams; };
		virtual const char*	getTagName() const		{ return "ARColor"; };
		virtual std::string getGMNName() const		{ return "\\color"; };

		float getColorR() const { return fR; }
		float getColorG() const { return fG; }
		float getColorB() const { return fB; }
		float getColorA() const { return fA; }

	protected:
		float fR;			// (JB) should be unsigned char components ?
		float fG;
		float fB;
		float fA;
};

#endif
