/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ARFontAble_H
#define ARFontAble_H

#include <string>
#include "ARMTParameter.h"


class ARFontAble : public ARMTParameter
{
	public:
				 ARFontAble();
		virtual ~ARFontAble() {}
	
		virtual void setTagParameters (const TagParameterMap& params);
		virtual const char*	getParamsStr() const { return kARFontAbleParams; };

		const char* getFont() const				{ return fFontName.c_str(); }
		const char* getTextFormat() const		{ return fTextFormat.c_str(); }
		const char* getTextAttributes() const	{ return fTextAttributes.c_str(); }
		float		getFSize() const			{ return fFontSize; }

	protected:
		std::string fTextFormat;
		std::string fFontName;
		float		fFontSize = 0.f;
		std::string fTextAttributes;
};

#endif
