#ifndef GRChordTag_H
#define GRChordTag_H

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
#include "GRSystemTagInterface.h"

class GRStaff;
class GRSystem;
class ARUserChordTag;

/** \brief The Chord Tag
*/

class GRChordTag : 	public GRPTagARNotationElement,
					public GRSystemTagInterface
{
	public:
		GRChordTag(GRStaff * pstaff, ARUserChordTag * pchordtag);
		virtual ~GRChordTag();

		virtual void removeAssociation(GRNotationElement * grnot);

		virtual void checkPosition(const GRSystem * grsys);
	    virtual void addAssociation(GRNotationElement * grnot);
		

		const char* getLabel() const;


	virtual void RangeEnd(GRStaff * grstaff);

	virtual void tellPosition(GObject *, const NVPoint & );


};

#endif
