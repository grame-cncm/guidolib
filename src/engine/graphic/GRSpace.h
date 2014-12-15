#ifndef GRSpace_H
#define GRSpace_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
class ARSpace;
class VGDevice;

#include "GRARNotationElement.h"


class GRStaff;

/** \brief not yet documented
*/
class GRSpace : public GRARNotationElement
{
	public:
						GRSpace( ARSpace * arspace, bool p_ownsar = false );

		virtual 		~GRSpace();
		virtual void 	OnDraw( VGDevice & hdc ) const;

		const ARSpace * getARSpace() const;
		virtual int 	getStandardSpacing() const;
};

#endif

