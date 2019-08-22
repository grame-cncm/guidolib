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

#pragma once

#include "ARText.h"

/*@mkdoc

@group:Text

@tagname:\footer
@tagalias:
@tagtype:P
@tagnotation:the score footer
@tagdesc
@tagend

The footer is displayed centered on the bottom of the first page.

@params:
@param:text:string:the footer text:*none*:false
@param:pageformat:string:a page location:c6:true
@fontparams:
@paramdesc
**pageformat** works similarly to [\title](#title).

See the [Jazz](@EXAMPLES/allthethings/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARFooter : public ARText
{
	public:
					 ARFooter();
		virtual     ~ARFooter() {}

		virtual int			getOrder() const		{ return kFooterOrder; }
		virtual const char*	getParamsStr() const	{ return kARFooterParams; };
		virtual const char*	getTagName() const		{ return "ARFooter"; };
		virtual std::string getGMNName() const		{ return "\\footer"; };

		const char*  getName() const;
		const char*  getPageFormat() const;
};
