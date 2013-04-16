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

#ifndef __GRSPF_H
#define __GRSPF_H

#ifdef OLDSPFACTIVE

/** The purpose of this class is as follows: While
adding graphical elements to a system/staff, 
the current extent needs to be calculated. As
the extent depends on an exerted Force, a Space-
Force-Function (SPF) has to be calculated. This
function depends on springs, their initial extent
(that is, saved forces which indicate minimum 
 stretching for a spring) and the spring-constants.
There are two operation to retrieve either a 
needed force for a given extent or to
retrieve the extent for a given force.

Technical: Internally, we have a hash with 
Entries of the form: (Force, Extent, SpringList). 
The hash is "sorted" by the Force-Key.  For each
distinct Force, an extent is saved. When looking
for an extent at a given force, the hash retrieves
the matching force-intervall and then returns
the (linearly calculated) extent. (Note, this
is a linear function).
The operations and function describe their actions
in detail below. 

There are N operations:

addSpring(S) : adds a Spring to the current
//		SPF. Adjusts the initial extent and
     the extent at the other force-points

changeSpringForce(S,newForce): changes a 
//		force and adjusts the extents at all
//		force-points. Actually realised by a call
//		to deleteSpring(S) and then addSpring(S) with
//		the new force.

deleteSpring(S): removes a Spring from the function.
//		the extents are adjusted accordingly.

getExtent(Force): gives the extent when exerting
//		a given force.

getForce(Extent): retrieves the force that is
//		needed to get the given extent.
*/

// for the hash-table.
#include "KR_Hash.h"
#include "KF_IList.h"
#include "GRSpring.h"


// already defined in GRSpringForceIndex.h 
// const float optimumforce = 600;

typedef KF_IPointerList<GRSpring> SpringList;
class GRConstraint;

typedef KF_IPointerList<GRConstraint>
	ConstraintList;

class ofstream;

class GRConstraint
{
public:
		int start;
		int end;
		double value;
		GRConstraint(int pstart,int pend,double pvalue)
		{
			start = pstart;
			end  = pend;
			value = pvalue;
		}
		GRConstraint(const GRConstraint &ct)
		{
			start = ct.start;
			end = ct.end;
			value = ct.value;
		}
		int operator==(const GRConstraint &ct)
		{
			if (start == ct.start
				&& end == ct.end
				&& value == ct.value)
				return 1;
			return 0;
		}
};

/** \brief This class handles the calculation of Forces/Extent given the springs 
	and their initial (saved) forces.
*/

class GRSpaceForceFunction
{
public:

	class GRHashEntry	
	{
	
		friend class GRSpaceForceFunction;
		float extent;
		SpringList *sprlst;

	public:
		GRHashEntry()
		{
			sprlst = new SpringList(0);
		}
		virtual ~GRHashEntry()
		{
			delete sprlst;
		}

		GRHashEntry & operator=(const GRHashEntry &he)
		{
			delete sprlst;
			extent = he.extent;
			sprlst = new SpringList(*he.sprlst,0);

			return *this;
		}


		// the copy-constructor
		GRHashEntry(const GRHashEntry &he)
			
		{
			extent = he.extent;
			sprlst = new SpringList(*he.sprlst,0);
		}
	};

public:
	GRSpring *getGRSpring(int id);
	int addConstraint(int start,int end,double value);
	int getMatrixRealSize();
	int getMatrixMemSize();
	double * getMatrix();
	double * resizeMatrix(int newrealsize);

	int getMSCMatrixRealSize();
	int getMSCMatrixMemSize();
	double * getMSCMatrix();

	double getMSCMatrix(int i,int j) const;
	void setMSCMatrix(int i,int j,double value);

	double * resizeMSCMatrix(int newrealsize);

	float zconstdenom;
	GRSpaceForceFunction(const GRSpaceForceFunction &spf);
	float EvaluateBreak(float extent);
	float getOptimumForce(float sprconst);
	void UnfreezeSpring(GRSpring *spr);
	void FreezeSpring(GRSpring *spr);
	void ResetSprings();
	void CopySPFWithoutSprings(const GRSpaceForceFunction &spf);
	void addSPF(const GRSpaceForceFunction &spf);

	GRSpaceForceFunction();
	virtual ~GRSpaceForceFunction();

	void addSpring(GRSpring *spr);
	void deleteSpring(GRSpring *spr);
	void changeSpringForce(GRSpring *spr,float newforce);
	float getExtent(float force) const;
	float getForce(float extent,
		ofstream *springlog = NULL);

private:

	KR_HashTable<float,GRHashEntry> *hash;

	

protected:
	float forceret;
	float forceextent;
	float evalextent;
	float evalret;
	float zconstnum;
	// the freezeoffset is a number
	// that must be used to calculate
	// the extent/force, when
	// some springs in the Space-Force-Function
	// are frozen.
	// it offsets the base-coordinate-point
	// by the amount of the spring-extent.
	float freezeoffset;

	int matmemsize;
	int matrealsize;
	double *mat;

	int mscmatmemsize;
	int mscmatrealsize;
	double *mscmat;


	// this is for QP-Programming (solving the
	// spring-equation as energy-minimizing)
	ConstraintList cl;

};

#endif // ifdef OLDSPFACTIVE

#endif

