#ifndef GRDynamics_H
#define GRDynamics_H

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
