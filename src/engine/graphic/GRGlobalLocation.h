#ifndef GRGlobalLocation_H
#define GRGlobalLocation_H

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

#include "GRPTagARNotationElement.h"

class ARShareLocation;
class GRStaff;
class GRFlag;
class ARDisplayDuration;
class GRStdNoteHead;
class GRStem;
class GRPositionTag;

/** \brief not yet documented
*/
class GRGlobalLocation : public GRPTagARNotationElement
{
	public:
					GRGlobalLocation(GRStaff *gstaff, ARShareLocation *pshare);
		virtual 	~GRGlobalLocation();


		int getHighestAndLowestNoteHead(GRStdNoteHead **highest,
					GRStdNoteHead **lowest) const;
		virtual void removeAssociation(GRNotationElement * el);
	    virtual void addAssociation(GRNotationElement *grnot);
		virtual void setHPosition( GCoord nx );
		virtual void OnDraw(VGDevice & hdc ) const;


	virtual void RangeEnd(GRStaff *grstaff);

	virtual GRNotationElement *  getFirstEl()
	{
		return mFirstEl;
	}
	virtual void tellPosition(GObject *, const NVPoint &);


	protected:

		// firstEl is the first element in 
		// the \shareStem-Range. Only the
		// first event in the range gets a spring
		// on its own ....
		GRNotationElement * mFirstEl;

};

#endif

