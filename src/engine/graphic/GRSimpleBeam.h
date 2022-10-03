#ifndef GRSimpleBeam_H
#define GRSimpleBeam_H

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

#include "GRNotationElement.h"
#include "GRBeam.h"

/** \brief Graphical representation af a Beam
*/
class GRSimpleBeam : public GRNotationElement
{
	public:
				GRSimpleBeam(GRBeam * p_parent, const BeamRect&);
		
		virtual const unsigned char * getColRef() const;

		virtual void OnDraw( VGDevice & hdc ) const;
				void setPoints( const BeamRect& );

		BeamRect	fRect;
		GRBeam *	fParent;
};

#endif
