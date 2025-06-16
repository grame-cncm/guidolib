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
#include "VGColor.h"

/*@mkdoc

@group:Layout

@tagname:\color
@tagalias: \colour
@tagtype:P
@tagnotation:sets the voice color
@tagdesc
The **\color** tag sets the voice color.
Note that cross staff (like \accol) or global tags (like \title) are ignored. To set the whole score color, use the \pageFormat color parameter.
Note also that the color applies to the voices that follow.
@tagend

@params:
@param:c:string:a color string:*none*:false
@paramdesc
The color string must be an html color name or a rgb(a) string in the form `0xrrggbb` or `0xrrggbbaa`
@paramend

*/

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

		const VGColor& getColor() const { return fColor; } //return VGColor (int(fR), int(fG), int(fB), int(fA)); }

	protected:
		VGColor fColor;
};

#endif
