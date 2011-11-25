#ifndef GRGlobalLocation_H
#define GRGlobalLocation_H

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

