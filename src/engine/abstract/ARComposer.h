#ifndef ARComposer_H
#define ARComposer_H

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

@group:Header

@tagname:\composer
@tagalias:
@tagtype:P
@tagnotation:the composer name
@tagdesc
@tagend

The composer name is displayed on top of the first page, right aligned to the score first system.

@params:
@param:name:string:the composer name:*none*:false
# @param:pageformat:string:a page format:c2:true   ????
@fontparams:
@paramdesc
See the [Composer](/examples/mozart581/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARComposer : public ARText
{
	public:
					 ARComposer();
		virtual     ~ARComposer() {}

		const char*  getName() const;
		const char*  getPageFormat() const;

		virtual int	 getOrder() const				{ return kComposerOrder; }
		virtual const char*	getParamsStr() const	{ return kARComposerParams; };
		virtual const char*	getTagName() const		{ return "ARComposer"; };
		virtual std::string getGMNName() const		{ return "\\composer"; };

		virtual void setTagParameters (const TagParameterMap& params);

	private:
		const TagParameterString * name;
		const TagParameterString * pageformat;
};


#endif
