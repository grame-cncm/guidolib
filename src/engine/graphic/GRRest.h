#ifndef GRRest_H
#define GRRest_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

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
					 GRRest( GRStaff * grstf, const TYPE_DURATION & duration );
					 GRRest( GRStaff * grstf, const ARRest * ar, bool p_ownsAR = false );
					 GRRest( GRStaff * grstf, const ARRest * ar, const TYPE_TIMEPOSITION & timepos, const TYPE_DURATION & duration );

		virtual 	~GRRest() {}

		virtual void	setRestFormat( const  ARRestFormat *restfrmt)	{}
		virtual int		getWholeMeasure() const					{ return mWholeMeasure; }
		virtual int		getMeasuresCount() const				{ return fMeasuresCount; }
	  	virtual void	setWholeMeasure(int p)					{ mWholeMeasure = p; }
	  	virtual void	setMeasuresCount(int count)				{ fMeasuresCount = count; }
		virtual void	OnDraw( VGDevice & hdc ) const = 0;

		virtual const GRRest *	isRest() const					{ return this; }

		const ARRest * getARRest() const;
	
	private:
		int		 mWholeMeasure;
		int		 fMeasuresCount = 0;
};

#endif


