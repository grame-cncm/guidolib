#ifndef GRDynamics_H
#define GRDynamics_H

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

class GRStaff;
class ARDynamics;


/** \brief Graphical representation of dynamics. Draws crescendo and decrescendo arrows.
*/
class GRDynamics : public GRPTagARNotationElement
{
	public:

		class GRDynamicsSaveStruct : public GRPositionTag::GRSaveStruct
		{
			public:
				virtual ~GRDynamicsSaveStruct() {}
				enum d_pos_type {D_BOTTOM, D_TOP, D_FREE} d_pos;
				NVPoint position;
				NVRect  boundingBox;
		};

		virtual void removeAssociation(GRNotationElement * el);
		virtual void addAssociation(GRNotationElement *);
		GRDynamics(GRStaff * pstaff); // constructor without REAL abstract element
		GRDynamics(GRStaff * pstaff, ARDynamics * abstractRepresentationOfDynamic);
		virtual ~GRDynamics();
		virtual void OnDraw(VGDevice & dev ) const;
		
		
		virtual void tellPosition(GObject * caller, const NVPoint & newPosition);
		virtual void print() const;
		
		virtual int updateDynamic(const GRStaff * pstaff);
		
	protected:
		
		virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()
		{
			return new GRDynamicsSaveStruct;
		}
		
};

#endif
