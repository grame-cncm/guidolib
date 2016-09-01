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
				 GRBeginSpaceForceFunction2(float force) : GRSpaceForceFunction2(force), vecttop(0), vectbottom(0) {}
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
