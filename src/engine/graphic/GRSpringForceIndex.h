#ifndef GRSpaceForceFunction2_H
#define GRSpaceForceFunction2_H

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

/** \brief Handles the calculation of Forces/Extent given the springs and 
	 their initial (saved) forces.

	The purpose of this class is as follows: While
	adding graphical elements to a system/staff, 
	the current extent needs to be calculated. As
	the extent depends on an exerted Force, a Space-
	Force-Function (SFF) has to be calculated. This
	function depends on springs, their initial extent
	(that is, saved forces which indicate minimum 
	 stretching for a spring) and the spring-constants.
	 There are two operation to retrieve either a 
	needed force for a given extent or to
	retrieve the extent for a given force.

	The operations and function describe their actions
	in detail below. 

	There are N operations:

	addSpring(S) : adds a Spring to the current
			SFF. Adjusts the initial extent and
	      the extent at the other force-points

	changeSpringForce(S,newForce): changes a 
			force and adjusts the extents at all
			force-points. Actually realised by a call
			to deleteSpring(S) and then addSpring(S) with
			the new force.

	deleteSpring(S): removes a Spring from the function.
			the extents are adjusted accordingly.

	getExtent(Force): gives the extent when exerting
			a given force.

	 getForce(Extent): retrieves the force that is
		needed to get the given extent.
*/

#include "kf_ilist.h"
#include "kf_vect.h"
#include "GRSpring.h"

const float optimumforce = 800;

class GRSpaceForceFunction2;

/** \brief not yet documented
*/
class GRSpringForceIndex
{
		friend class GRSpaceForceFunction2;

	public:
		GRSpringForceIndex(GRSpring * p_spr,float p_force,int p_index)
		{
			spr = p_spr;
			force = p_force;
			index = p_index;
		}
		~GRSpringForceIndex() {}

	protected:

		GRSpring * spr;
		float	  force;
		int       index;
};



// typedef KF_IPointerList<GRSpring> SpringList;
typedef KF_IPointerList<GRSpringForceIndex> SortedForceList;
//typedef KF_Vector<float> FloatArray;

/** \brief This class realizes the second implementation of the space-force-function. 

	Instead of handling a map of values, a simple ordered list of springs is maintained. 
	To calculate the force, we just have to follow the sorted list to find the line segment 
*/
class GRSpaceForceFunction2
{
	public:

						 GRSpaceForceFunction2(float force);
		virtual 		~GRSpaceForceFunction2();
						// Copy ctor, do I copy the springs? not really? do i?
						 GRSpaceForceFunction2(const GRSpaceForceFunction2 &sff);

		void 	setOptForce(float newoptforce);

		void writeAllExtents(std::ostream& os) const;

		float EvaluateBreak(float extent);
		void UnfreezeSpring(GRSpring *spr);
		void FreezeSpring(GRSpring *spr);
		void ResetSprings();
		void addSFF(const GRSpaceForceFunction2 &sff);

		float getOptForce();


		void addSpring(GRSpring *spr);
		void deleteSpring(GRSpring *spr);
		float getExtent(float force) const;
		float getForce(float extent);

		float getOptConstant() const	{ return copt; }
		float getXminOpt() const	{ return xminopt; }

	protected:
		static float CalcSpringConstant( float c1, float c2 )	{ return (c1 * c2) / (c1 + c2); }

		SortedForceList sortedlist;

		float xmin; // the sum of all lengths of all prestretched springs
		float cmax; // the maximum spring-constant
					// if all springs need to be stretched.
		float xminopt; // This is the xmin-value for the optimum force
		float copt;  // this is the spring-constant for the optimum force 

		float optforce;
};


#endif
