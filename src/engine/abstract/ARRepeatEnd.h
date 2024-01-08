#ifndef ARRepeatEnd_H
#define ARRepeatEnd_H

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

#include "ARBar.h"
#include "TimeUnwrap.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

class ARRepeatBegin;

/*@mkdoc

@group:Repeat Signs

@tagname:\repeatEnd
@tagalias:
@tagtype:P
@tagnotation:An end repeat bar
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

*/

/** \brief not yet documented
*/
class ARRepeatEnd : public ARBar
{
	public:
					 ARRepeatEnd(ARRepeatBegin * curbeg = 0) { rangesetting = RANGEDC; setupTagParameters (gMaps->sARRepeatMap); }
		virtual		~ARRepeatEnd() { }

		virtual const char*	getTagName() const		{ return "ARRepeatEnd"; };
		virtual std::string getGMNName() const		{ return "\\repeatEnd"; };

        ARMusicalObject *isARRepeatEnd() 			{ return this; }
		bool			 hidden () const			{ return getParameter<TagParameterString>(kHiddenStr, true)->getBool(); }

		virtual void browse(TimeUnwrap& mapper) const
				{ mapper.AtPos (this, TimeUnwrap::kRepeatEnd); }
};

#endif
