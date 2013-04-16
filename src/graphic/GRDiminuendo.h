#ifndef GRDiminuendo_H
#define GRDiminuendo_H

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

class ARDiminuendo;
#include "GRDynamics.h"

class GRStaff;


/** \brief The Diminuendo dynamics. Draws a decrescendo arrow.
*/
class GRDiminuendo : public GRDynamics
{
  public:
	 
	  // constructor without REAL abstract element
	 GRDiminuendo(GRStaff *); 
	 
	 GRDiminuendo(GRStaff *,ARDiminuendo * abstractRepresentationOfDecresc);
	 // GRDiminuendo(GRStaff *,ARDimBegin *arb);
	 virtual ~GRDiminuendo();

	 virtual void OnDraw( VGDevice & hdc ) const;
	 virtual void print() const;
  };

#endif
