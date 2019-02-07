#ifndef GRFingering_H
#define GRFingering_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRText.h"

class ARFingering;

/** \brief A fingering string.
*/
class GRFingering : public GRText
{	
	public:
						 GRFingering( GRStaff *, const ARText * artext );
		virtual 		~GRFingering() {}

		virtual void	accept (GRVisitor& visitor);
		virtual void 	tellPosition( GObject * caller, const NVPoint & inPosition );

		const ARFingering * getARFingering() const;
};

#endif

