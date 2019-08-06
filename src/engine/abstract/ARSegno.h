#ifndef __ARSegno__
#define __ARSegno__

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARJump.h"
#include "TimeUnwrap.h"

/*@mkdoc

@group:Repeat Signs

@tagname:\segno
@tagalias:
@tagtype:P
@tagnotation:Segno
@tagdesc
@tagend

*/

/** \brief Signa
*/
class ARSegno : public ARJump
{
	public:
					 ARSegno() : ARJump ("[segno]") {}
		virtual 	~ARSegno() {}

		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARSegno"; };
		virtual std::string getGMNName() const		{ return "\\segno"; };
		ARMusicalObject *isARSegno()				{ return this; }

		virtual void browse(TimeUnwrap& mapper) const
			{ mapper.AtPos (this, TimeUnwrap::kSegno); }
};

#endif
