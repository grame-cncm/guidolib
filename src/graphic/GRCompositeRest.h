#ifndef GRCompositeRest_H
#define GRCompositeRest_H

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

