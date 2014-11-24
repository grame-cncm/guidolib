#ifndef GRTFermata_H
#define GRTFermata_H

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

#include "GRPositionTag.h"
#include "GRARNotationElement.h"

class ARTFermata;


/** \brief not yet documented
*/
class GRTFermata : public GRARNotationElement, public GRPositionTag
{

public:
		virtual void OnDraw( VGDevice & hdc ) const;
	const ARTFermata * getARTFermata() const;
	virtual void addAssociation(GRNotationElement * el);
	virtual void removeAssociation(GRNotationElement * el);
	GRTFermata(GRStaff * pstaff,ARTFermata * art);
	virtual ~GRTFermata();

};

#endif
