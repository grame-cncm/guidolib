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

/*@mkdoc

@group:Layout

@tagname:\staffFormat
@tagalias:
@tagtype:P
@tagnotation:sets the staff format
@tagdesc
@tagend

@params:
@param:style:string:a style string in the form 'n-lines' or 'TAB':5-lines:true
@param:lineThickness:float:the staff lines thickness:0.08:true
@param:distance:unit:sets a fixed distance to the preceding staff:0hs:true
@paramdesc
The "TAB" style is intended to display tablatures, it is similar to
`style="6-lines", size=1.35`

Note: the **\staffFormat** tag supports common parameters but dx has no effect.

See the [Layout](@EXAMPLES/layout/) example.
@paramend

*/

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

//				const TagParameterString *	getStyle() const;
				const TagParameterFloat *	getStaffDistance() const;
				float getLineThickness() const	{ return fLineThickness; }
				int   getLinesCount() const		{ return fLinesCount; }
				bool  isTAB () const 			{ return fTAB; }

	protected:
		TagParameterFloat	fSize;
		float				fLineThickness;
		int					fLinesCount = 5;
		bool				fTAB = false;
};

#endif 
