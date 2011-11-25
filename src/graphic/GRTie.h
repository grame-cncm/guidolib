#ifndef GRTie_H
#define GRTie_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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


#include "GRBowing.h"

class ARTie;
class GRStaff;

/** \brief Graphical representation of a tie.
*/
class GRTie : public GRBowing
{
	public:
	 
	 			GRTie(GRStaff * grstaff, GRNotationElement * start, 
	 										GRNotationElement * end = 0);
	 										
				GRTie(GRStaff * grstaff);
				GRTie(GRStaff * grstaff, ARTie * abstractRepresentationOfBowing);
	
		virtual	~GRTie();

	  	virtual bool IsFull();

		virtual void addAssociation(GRNotationElement *el);

	protected:

		virtual void updateBow( GRStaff * grstaff );

		virtual void automaticAnchorPoints( GRBowingContext * bowContext, ARBowing * arBow, 
														GRSystemStartEndStruct * sse );
														
		virtual void automaticControlPoints( GRBowingContext * bowContext, 
										ARBowing * arBow, 
										GRSystemStartEndStruct * sse );

};

#endif
