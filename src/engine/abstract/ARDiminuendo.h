#ifndef ARDiminuendo_H
#define ARDiminuendo_H

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

#include "ARDynamic.h"

/*@mkdoc

@group:Dynamics

@tagname:\decrescendo
@tagalias:\decresc<br />\dimnuendo <br />\dim<br />\crescBegin \crescEnd<br />\diminuendoBegin \diminuendoEnd <br />\dimBegin \dimEnd
@tagtype:R
@tagnotation:crescendo sign
@tagdesc
@tagend


@params:

@paramdesc
Parameters for decrescendo are the same as for crescendo
@paramend

*/


/** \brief not yet documented
*/
class ARDiminuendo : public ARDynamic
{
	public:

					 ARDiminuendo()	{}
					 ARDiminuendo(const ARDiminuendo* dim) : ARDynamic(dim) {}
		virtual		~ARDiminuendo() {}

		virtual bool MatchEndTag(const char * s);

		virtual const char*	getTagName() const		{ return "ARDiminuendo"; };
		virtual std::string getGMNName() const		{ return "\\diminuendo"; };
};

#endif
