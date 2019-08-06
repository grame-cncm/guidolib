#ifndef ARMark_H
#define ARMark_H

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

#include <map>
#include <string>

#include "ARText.h"

/*@mkdoc

@group:Text

@tagname:\mark
@tagalias:
@tagtype:P
@tagnotation:marks
@tagdesc
The **\text** tag is both a range and a position tag. When a range is specified, the text doesn't take place: it is placed over the range.
With no range, the text is inserted at its position.
@tagend

@params:
@param:text:string:the mark:*none*:false
@param:enclosure:string:none, square, rectangle, oval, circle, bracket, triangle, diamond:none:true
@fontparams:
@paramdes:
@paramend

*/

/** \brief not yet documented
*/
class ARMark : public ARText // public ARMusicalObject,
{
	int fEnclosure;
	
	public:
		enum { kNoEnclosure, kSquare, kRectangle, kOval, kCircle, kBracket, kTriangle, kDiamond };
	
				 ARMark();
		virtual ~ARMark() {}

		int  getEnclosure() const					{ return fEnclosure; }

		virtual const char*	getParamsStr() const	{ return kARMarkParams; };
		virtual const char*	getTagName() const		{ return "ARMark"; };
		virtual std::string getGMNName() const		{ return "\\mark"; };

		virtual void setTagParameters (const TagParameterMap& params);

	protected:
		static std::map<std::string, int>	fEnclosureShapes;
};

#endif
