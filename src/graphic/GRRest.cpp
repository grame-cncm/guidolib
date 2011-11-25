/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

void GRRest::print() const
{
	fprintf(stderr,"(R%.2f,%.2f)",(float) getRelativeTimePosition(),
		(float) getDuration());
}

