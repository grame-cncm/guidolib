#ifndef GRIntens_H
#define GRIntens_H

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

#include "GRTagARNotationElement.h"

class ARIntens;
class GRStaff;
class VGDevice;


/** \brief not yet documented
*/
class GRIntens : public GRTagARNotationElement
{
	float fSize = 1;
	float fDx 	= 0;
	float fDy 	= 0;
	const GRSingleNote* fNote = 0;

	public:
				 GRIntens( GRStaff * inStaff, const ARIntens * abstractRepresentationOfIntens );
		virtual ~GRIntens() {}
	
		virtual void OnDraw(VGDevice & hdc ) const;
		virtual void accept (GRVisitor& visitor);

		const ARIntens* getARIntens() const;
		void setNote (const GRSingleNote* note) 	{ fNote = note; }
		const GRSingleNote* getNote() const 		{ return fNote; }
};

#endif


