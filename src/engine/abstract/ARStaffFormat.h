#ifndef ARStaffFormat_H
#define ARStaffFormat_H

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
#include "TagParameterFloat.h"

class TagParameterString;

/** \brief not yet documented
*/
class ARStaffFormat : public ARMTParameter
{
	public:
				 ARStaffFormat();
				 ARStaffFormat(const ARStaffFormat &);
		virtual ~ARStaffFormat() {}

		virtual bool IsStateTag() const		{ return true; }

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARStaffFormatParams; };
		virtual const char*	getTagName() const		{ return "ARStaffFormat"; };
		virtual std::string getGMNName() const		{ return "\\staffFormat"; };
		virtual const TagParameterFloat *	getSize() const;

				const TagParameterString *	getStyle() const;
				const TagParameterFloat *	getStaffDistance() const;
				float getLineThickness() const		{ return fLineThickness; }

	protected:
		TagParameterFloat	fSize;
		float				fLineThickness;
};

#endif 
