#ifndef GRRest_H
#define GRRest_H

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

#include "GREvent.h"

class ARRest;
class ARRestFormat;


/** \brief The Rest event.
*/
class GRRest : public GREvent
{ 
	public:
					GRRest( GRStaff * grstf, const TYPE_DURATION & theDuration );
					GRRest( GRStaff * grstf, ARRest * abstractRepresentationOfRest,
															 	bool p_ownsAR = false );

					GRRest( GRStaff * grstf,ARRest * abstractRepresentationOfRest,
							const TYPE_TIMEPOSITION & relativeTimePositionOfGRRest,
							const TYPE_DURATION & durationOfGRRest );

		virtual 	~GRRest();

		virtual void setRestFormat(ARRestFormat *restfrmt);
	  	
		virtual int getWholeMeasure() const;
	  	virtual void setWholeMeasure(int p);	// ! unused !

		virtual void OnDraw( VGDevice & hdc ) const = 0;

		ARRest * getARRest() const;

		virtual void print() const;

	private:

		int mWholeMeasure;
};


inline void GRRest::setRestFormat(ARRestFormat *)
{
}

inline void GRRest::setWholeMeasure(int p)
{
	mWholeMeasure = p;
}

inline int GRRest::getWholeMeasure() const
{
	return mWholeMeasure;
}


#endif


