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

GRRest::GRRest(GRStaff * grstf,ARRest * abstractRepresentationOfRest, bool p_ownsAR)
  : GREvent(grstf,abstractRepresentationOfRest,p_ownsAR)
{
	mWholeMeasure = 0;
}

GRRest::GRRest(GRStaff *grstf,ARRest* abstractRepresentationOfRest,
	const TYPE_TIMEPOSITION & relativeTimePositionOfGRRest,
	const TYPE_DURATION & durationOfGRRest)
  : GREvent(grstf,abstractRepresentationOfRest,relativeTimePositionOfGRRest,durationOfGRRest)
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

void GRRest::print(int &indent) const
{
	fprintf(stderr,"(R%.2f,%.2f)",(float) getRelativeTimePosition(),
		(float) getDuration());
}

