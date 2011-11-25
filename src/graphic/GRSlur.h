#ifndef GRSlur_H
#define GRSlur_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame.

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

#include "ARSlur.h"
#include "GRBowing.h"

/** \brief Graphical representation of a slur.
*/
class GRSlur : public GRBowing
{
	public:
	 		GRSlur(GRStaff * grstaff); // constructor without REAL abstract element
	 		GRSlur(GRStaff * grstaff, ARSlur * inAR );
	 
	 virtual ~GRSlur();

	protected:

		virtual void updateBow( GRStaff * grstaff );

		virtual void automaticCurveDirection( GRBowingContext * bowContext, 
											  ARBowing * arBow, 
											  GRSystemStartEndStruct * sse );

		virtual void automaticAnchorPoints( GRBowingContext * bowContext, 
										ARBowing * arBow, 
										GRSystemStartEndStruct * sse );

		virtual void automaticControlPoints( GRBowingContext * bowContext, 
										ARBowing * arBow, 
										GRSystemStartEndStruct * sse );
	
};

#endif
