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

// #include <cmath>

#include <stdio.h>

#include "ARRest.h"
#include "GRRest.h"
#include "GRDefine.h"


GRRest::GRRest(GRStaff * grstf,const TYPE_DURATION & theDuration)
  		: GREvent(grstf, new ARRest(theDuration), true) // ownsAR
{
	mWholeMeasure = 0;
}

GRRest::GRRest(GRStaff * grstf,ARRest * arrest, bool p_ownsAR)
  : GREvent(grstf,arrest,p_ownsAR)
{
	mWholeMeasure = 0;
}

GRRest::GRRest(GRStaff *grstf,ARRest* arrest, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration)
  : GREvent(grstf, arrest, date, duration)
{
	mWholeMeasure = 0;
}


GRRest::~GRRest()
{
}


ARRest * GRRest::getARRest() const
{
	return /*dynamic*/static_cast<ARRest*>(getAbstractRepresentation());
}

