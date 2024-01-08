#ifndef ARRepeatBegin_H
#define ARRepeatBegin_H

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

class ARRepeatEnd;

/*@mkdoc

@group:Repeat Signs

@tagname:\repeatBegin
@tagalias:
@tagtype:P
@tagnotation:A begin repeat bar
@tagdesc
See the [Repeat Signs](@EXAMPLES/repeats/) example.
@tagend

*/

/** \brief The right facing repeat sign.
*/
class ARRepeatBegin : public ARMTParameter
{
	public:
					 ARRepeatBegin();
		virtual		~ARRepeatBegin() {}

		virtual const char*	getTagName() const		{ return "ARRepeatBegin"; };
		virtual std::string getGMNName() const		{ return "\\repeatBegin"; };
		virtual void browse(TimeUnwrap& mapper) const;

		virtual void setRepeatEnd( const ARRepeatEnd * );

        ARMusicalObject *isARRepeatBegin() 							{ return this; }
		const ARBar::TRanges&	getRanges () const					{ return fRanges; }
		void					setRanges (const ARBar::TRanges& r)	{ fRanges = r; }
		bool					hidden () const;
	
	private:
		ARBar::TRanges	fRanges;
		TYPE_DURATION dur;
};

#endif
