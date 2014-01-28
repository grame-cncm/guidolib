#ifndef GRCompositeRest_H
#define GRCompositeRest_H

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

#include "GRDefine.h"
#include "GRRest.h"

class ARRest;
class GRSingleRest;


/** \brief An abstract rest which consists of several graphical 
	rests which are linked by ties.
*/

class GRCompositeRest : public GRRest
{

  public:
//		GRCompositeRest(GRStaff *,
//			ARNote* abstractRepresentationOfNote,
//			const TYPE_TIMEPOSITION relativeTimePositionOfGRNote,
//			const TYPE_DURATION durationOfGRNote);

		GRCompositeRest(GRStaff * grstf, ARRest * abstractRepresentationOfRest);

		virtual ~GRCompositeRest();
		virtual void OnDraw( VGDevice & hdc ) const;

		virtual void updateBoundingBox();
		virtual void setPosition(const NVPoint &newPosition);


		virtual TYPE_TIMEPOSITION getNextTimePosition(const TYPE_TIMEPOSITION tp) const;
		virtual GRNotationElement * getGRNotationElementAtTimePosition(
			const TYPE_TIMEPOSITION tp);

		virtual void addRest(GRRest * rest);
		virtual void finish();

		virtual void tellPosition(GObject * caller,const NVPoint & point);


};



#endif

