#ifndef GRSpecial_H
#define GRSpecial_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

class ARSpecial;

#include "GRTag.h"
#include "GRARNotationElement.h"

/** \brief Handles the list of all fonts used by the Guido Library.
*/
class GRSpecial : public GRARNotationElement, public GRTag
{
	public:
						 GRSpecial( ARSpecial * arspec, bool p_ownsar = false );
		virtual 		~GRSpecial() {}

		virtual void 	OnDraw( VGDevice & hdc ) const;

		virtual void 	setPosition(const NVPoint & point)		{ GRARNotationElement::setPosition(point); } 
};

#endif
