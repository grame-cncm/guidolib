#ifndef GREmpty_H
#define GREmpty_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003	Grame

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

#include "GREvent.h"

/** \brief An empty (invisible) event.
*/
class GREmpty : public GREvent  
{
	public:

				GREmpty( GRStaff * inStaff,
							ARMusicalEvent * abstractRepresentationOfEmpty,
							bool p_ownsAR = false );

				GREmpty( GRStaff * inStaff,
							ARMusicalEvent * abstractRepresentationOfEmpty,
							const TYPE_TIMEPOSITION theRelativeTimePositionOfGR,
							const TYPE_DURATION theDurationOfGR);
		
		virtual ~GREmpty();

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void tellPosition( GObject * caller, const NVPoint & newPosition );

		virtual void print() const	{ }
		virtual void updateBoundingBox();
		
	protected: 
};

#endif

