#ifndef GRNotations_H
#define GRNotations_H

/*
  GUIDO Library
  Copyright (C) 2002-2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRTagARNotationElement.h"

class ARNotations;
class GRStaff;
class VGDevice;


/** \brief not yet documented
*/
class GRNotations : public GRTagARNotationElement
{
	public:
				 GRNotations( GRStaff * inStaff, const ARNotations * ar);
		virtual ~GRNotations() {}
	
		virtual void OnDraw(VGDevice & hdc ) const;
		virtual void accept (GRVisitor& visitor);
	
		const ARNotations* getARNotations() const;
};

#endif


