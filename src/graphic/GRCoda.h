#ifndef GRCoda_H
#define GRCoda_H

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRJump.h"

class ARCoda;

/** \brief A Coda graphical element.
*/
class GRCoda : public GRJump
{
	public:
						GRCoda( ARCoda * ar );
		virtual			~GRCoda();
	protected:
		virtual float	getSymbolSize () const;
};

#endif
