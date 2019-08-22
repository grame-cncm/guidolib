#ifndef ARTitle_H
#define ARTitle_H

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

#include "ARText.h"

/*@mkdoc

@group:Text

@tagname:\title
@tagalias:
@tagtype:P
@tagnotation:the score title
@tagdesc
@tagend

The piece title is displayed centered on top of the first page.

@params:
@param:name:string:the title of the piece:*none*:false
@param:pageformat:string:a page location:c2:true
@fontparams:
@paramdesc
**pageformat** is a 2 chars string that describes the location of the title on the page: the first char gives the x position, and the secod the y position.
This location is relative to the page size and to the page margins (see [\pageFormat](../Layout/#pageformat)).

Values for the first char must be in:

- **1** or **l**: the x position is set to the left side of the page
- **2**: the x position is set to the middle of left margin.
- **3**: the x position is set to the left margin.
- **4** or **c**: the x position is centered on the page
- **5**: the x position is set to the right margin.
- **6**: the x position is set to the middle of right margin.
- **7** or **r**: the x position is set to the right side of the page

Values for the second char must be in:

- **1** or **t**: the y position is set to the top side of the page
- **2**: the y position is set to the middle of top margin.
- **3**: the y position is set to the top margin.
- **4** or **c**: the y position is centered on the page
- **5**: the y position is set to the bottom margin.
- **6**: the y position is set to the middle of bottom margin.
- **7** or **b**: the y position is set to the bottom side of the page


See the [Mozart](@EXAMPLES/mozart581/) example.<br />
See the [Fauré](@EXAMPLES/faure/) example.<br />
See the [Jazz](@EXAMPLES/allthethings/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARTitle : public ARText
{
	public:
					 ARTitle();
		virtual     ~ARTitle() {}

		virtual int			getOrder() const		{ return kTitleOrder; }
		virtual const char*	getParamsStr() const	{ return kARTitleParams; };
		virtual const char*	getTagName() const		{ return "ARTitle"; };
		virtual std::string getGMNName() const		{ return "\\title"; };

		const char*  getName() const;
		const char*  getPageFormat() const;
};

#endif
