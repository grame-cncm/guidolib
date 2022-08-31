#ifndef GREmpty_H
#define GREmpty_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GREvent.h"
//#include "GRVisitor.h"

/** \brief An empty (invisible) event.
*/
class GREmpty : public GREvent  
{
	public:

				GREmpty( GRStaff * inStaff, const ARMusicalEvent * ar, bool p_ownsAR = false );
				GREmpty( GRStaff * inStaff, const ARMusicalEvent * ar,
							const TYPE_TIMEPOSITION theRelativeTimePositionOfGR,
							const TYPE_DURATION theDurationOfGR);
		
		virtual ~GREmpty();

		virtual void accept (GRVisitor& visitor);
		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void tellPosition( GObject * caller, const NVPoint & newPosition );
		virtual bool isEmpty() const			{ return true; }

		virtual void updateBoundingBox();
		
		void setAuto (bool val)		{ fIsAuto = val; }
		bool isAuto () const		{ return fIsAuto; }
		bool isInChord () const		{ return fIsAuto; }
	
	protected:
		bool fIsAuto = false;
	
};

#endif

