#ifndef GRPositionTag_H
#define GRPositionTag_H

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

#include "GRTag.h"
#include "kf_ilist.h"

class GRSystem;
// class GRSystemSlice;
class GRNotationElement;

/** \brief This class is used to determine when a position tag ends. 

The endposition is the position within the voice. When this position has been parsed 
in the GRVoiceManager, 
then we know, that the positiontag-range has ended.

*/

class GRSystemStartEndStruct
{
public:

	enum setflag { LEFTMOST, RIGHTMOST,	OPENLEFT, OPENRIGHT ,NOTKNOWN };
		
	GRSystemStartEndStruct()
	{
		grsystem = 0;
		startElement = 0;
		endElement = 0;
		startflag = NOTKNOWN;
		endflag = NOTKNOWN;
		startpos = 0;
		endpos = 0;
		p = 0;
	}
	virtual ~GRSystemStartEndStruct();

	GRSystem * grsystem;

	GRNotationElement * startElement;
	GRNotationElement * endElement;

	setflag startflag;
	setflag endflag;

	// these give the position in the associated lists ...
	// so that the tag later nows, where it's sub-parts start and end.
	GuidoPos startpos;
	GuidoPos endpos;

	void * p;
};

typedef KF_IPointerList<GRSystemStartEndStruct> SSEList;

// the position tag owns this list.

/** \brief The GRPositionTag is the base class for representing range tags. 

The GRPositionTag also has a list of staffs, start and end elements as well as a 
void-pointer that is used to handle position tags that reach over multiple systems, 
that is, that they have been broken either by newSystem/newPage or by automatic Break-routines.

*/
class GRPositionTag : public GRTag
{
public:
	class GRSaveStruct 
	{
		public:
				GRSaveStruct() {};
		virtual ~GRSaveStruct() {};
	};

	void DeleteAllSSEs();
	virtual void ResumeTag(GRStaff * grstaff, GuidoPos assocpos);
	virtual void BreakTag(GRStaff * grstaff, GuidoPos & assocpos);
	virtual void StaffBegin(GRStaff  * grstaff = 0);
	virtual void StaffFinished(GRStaff * grstaff = 0);
	virtual void FinishPTag (GRStaff *)     {}
	virtual void RangeEnd(GRStaff * grstaff = 0);

	void changeCurrentSystem(GRSystem * psystem);
	virtual bool DeleteStaff(GRStaff * grstaff);
	GRPositionTag(ARMusicalTag * armt = 0);
	GRPositionTag(GuidoPos p_ep, ARMusicalTag * armt = 0);

	GuidoPos getEndPos() const { return ep; }

	GRSystemStartEndStruct * getSystemStartEndStruct(const GRSystem * grsystem) const;

protected:

	virtual void setStartElement(const GRStaff * grstaff, 	GRNotationElement * n_element);
	virtual void setEndElement(const GRStaff * grstaff, GRNotationElement * n_element);

	virtual void addAssociation(GRNotationElement * grnot);
	virtual void removeAssociation(GRNotationElement * grnot);

	virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct() 
			{ return new GRSaveStruct; }

	GuidoPos ep;

	SSEList mStartEndList;	// the Range tag Start-End List

	GRSystemStartEndStruct::setflag lastendflag;
	GRNotationElement * lastendElement;
	GuidoPos lastendpos;

};


#endif
