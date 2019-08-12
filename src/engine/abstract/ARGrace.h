#ifndef ARGrace_H
#define ARGrace_H

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
#include "ARPositionTag.h"

class TagParameterInt;

/*@mkdoc

@group:Notes

@tagname:\grace
@tagalias:
@tagtype:R
@tagnotation:display grace notes
@tagdesc
See the [notes](@EXAMPLES/notes/) example.
@tagend

*/

/** \brief not yet documented
*/
class ARGrace : public ARMTParameter, public ARPositionTag
{
	public:
					 ARGrace();
		virtual 	~ARGrace() {}

		virtual const char*	getParamsStr() const	{ return kARGraceParams; };
		virtual const char*	getTagName() const		{ return "ARGrace"; };
		virtual std::string getGMNName() const		{ return "\\grace"; };
		virtual ARGrace *isARGrace()				{ return this; }

		const TagParameterInt * getNum() const;
};

#endif
