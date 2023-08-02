#ifndef GRGrace_H
#define GRGrace_H

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

#include "GRARCompositeNotationElement.h"
#include "GRPositionTag.h"

class ARGrace;
class GRStaff;

/** \brief not yet documented
*/
class GRGrace : public GRARCompositeNotationElement,
				public GRPositionTag
{
	public:
					GRGrace( GRStaff * grstf, const ARGrace * argrc );
		virtual 	~GRGrace() {}

		virtual void tellPosition( GObject * caller,const NVPoint & inPos );
		// DF-02-08-2023 don't add associations to grace (useless)
		// solves the crash bug in issue #173
		virtual void addAssociation( GRNotationElement * p ) {}

		virtual GuidoPos AddTail( GRNotationElement * el );
};

#endif
