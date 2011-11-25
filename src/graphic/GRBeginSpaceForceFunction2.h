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
#ifndef GRBeginSpaceForceFunction2_H
#define GRBeginSpaceForceFunction2_H

#include "GRSpringForceIndex.h"
#include "kf_vect.h"

typedef KF_Vector<float> FloatVect;

/** \brief GRBeginSpaceForceFunction2 is an derived version of
	GRSpaceForceFunction2. 
	
	Additionally to the orginal SFF2, this version includes a float-vector which 
	captures the height of each staff for the begin-slice that would be added if
	the line of music would really be created.
*/

class GRBeginSpaceForceFunction2 : public GRSpaceForceFunction2
{
	public:	
				 GRBeginSpaceForceFunction2() : GRSpaceForceFunction2(), vecttop(0), vectbottom(0) {}
		virtual ~GRBeginSpaceForceFunction2()  {}
		
		float getTop(int i) const
		{
			return vecttop.Get(i);
		}
		
		float getBottom(int i) const
		{
			return vectbottom.Get(i);
		}

		void setHeight(int i,float top,float bottom)
		{
			vecttop.Set(i,top);
			vectbottom.Set(i,bottom);
		}

	protected:
		FloatVect vecttop;
		FloatVect vectbottom;
};

#endif
