#ifndef GRRest_H
#define GRRest_H

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


