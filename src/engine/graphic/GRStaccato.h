#ifndef GRStaccato_H
#define GRStaccato_H

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

#include "ARStaccato.h"
#include "GRARNotationElement.h"
#include "GRDefine.h"

class GRStaff;


/** \brief Graphical representation af a Staccato
*/
class GRStaccato : public GRARNotationElement
{

  public:
		GRStaccato( ARStaccato * arstacc, bool ownsar = false);

		virtual ~GRStaccato();

		// ignoriere Positionssetzungen ...
		virtual void setPosition(const NVPoint &point);

		// this will be done by the event to which
		// the staccato belongs ....
		virtual void tellPosition(GObject * caller,const NVPoint & pos);

		virtual void OnDraw( VGDevice & hdc ) const;

		




};

#endif


