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
/*
	Revision 1.1  2002-08-29 1 renz
	Added RCS Headers
	The Space-Force-Function now has the optimum-force-average and the minimum
	and maximum spring-constant for interpolation (also xmin, minimum extent)
	GRSpaceForceFunction2 is the home of the optimum force which is used
	for finding optimum line and page- breaks
*/ 

#include <fstream>
#include <cfloat>
#include <cmath>
#include <iostream>

#include "GRSpringForceIndex.h"
#include "GRVoice.h"
#include "GuidoDefs.h"

using namespace std;

// The code for the GRSpaceForceFunction2

GRSpaceForceFunction2::GRSpaceForceFunction2()	: sortedlist(1)
{
	xminopt = 0;
	copt = -1;

	xmin = 0;
}

GRSpaceForceFunction2::~GRSpaceForceFunction2()
{
	sortedlist.RemoveAll();	
}


/** \brief addSpring adds a Spring to the SFF.
*/
void GRSpaceForceFunction2::addSpring(GRSpring *spr)
{
	float force  = spr->force;
	float sconst = spr->sconst;

	// this increases the minimum extent of the SFF
	xmin += spr->x;

	// the number of elements/springs in the SFF
	int count = sortedlist.GetCount();
	if (count == 0)
		cmax = sconst;
	else {
		// the last one ....
		float calt = cmax;
		float cneu = CalcSpringConstant( calt, sconst );
		cmax = cneu;
	}

	if (force <= optforce) {
		if (copt == -1)
			copt = sconst;
		else
		{
			float calt = copt;
			float cneu = CalcSpringConstant( calt, sconst );
			copt = cneu;
		}
	}
	else {
		// force is bigger ....
		xminopt += spr->x;
	}

	// now we have to sort the force into the list of stored forces ...
	GRSpringForceIndex *sfi = new GRSpringForceIndex(spr, spr->force, count+1);
	GuidoPos pos = sortedlist.GetHeadPosition();
	while (pos)
	{
		GuidoPos savepos = pos;
		GRSpringForceIndex *tmpsfi = sortedlist.GetNext(pos);
		if (tmpsfi->force >  force)
		{
			sortedlist.AddElementAt(savepos,sfi);
			sfi = NULL;
			break;
		}
	}
	if (sfi)
		sortedlist.AddTail(sfi);
}

/** \brief This procedure removes a spring from  the SFF
*/
void GRSpaceForceFunction2::deleteSpring(GRSpring *spr)
{

	// now we find the position of the spring in our
	// internal springlist. The position in the internal
	// list govern the (pre)-calculation of
	// the partial spring-constants
	int npos = sortedlist.GetCount()+1; // for security .. see below
	GuidoPos tmppos = sortedlist.GetHeadPosition();
	while (tmppos)
	{
		GuidoPos savepos = tmppos;
		GRSpringForceIndex *tmpsfi = sortedlist.GetNext(tmppos);
		if (tmpsfi->spr == spr)
		{
			npos = tmpsfi->index;
			sortedlist.RemoveElementAt(savepos);
			break;
		}
	}

	float force = spr->force;
	float sconst = spr->sconst;

	int count = sortedlist.GetCount();
	if (npos <= ( count + 1 ))
	{
		xmin -= spr->x;
	}

	if (sortedlist.GetCount() == 0)
	{
		cmax = 0;
	}
	else
	{
		float calt = cmax;
		// float cneu = 1.0 / ( (1.0/calt) - (1.0/sconst) );
		float cneu = CalcSpringConstant( -calt, sconst );
		cmax = cneu;
	}

	if (force <= optforce)
	{
		if (copt == sconst)
		{
			copt = -1;
		}
		else
		{
			float calt = copt;
			//float cneu = 1.0 / ( (1.0/calt) - (1.0/sconst) );
			float cneu = CalcSpringConstant( -calt, sconst );
			
			copt = cneu;
		}
	}
	else
	{
		xminopt -= spr->x;
	}
	
	// I have to go through the springlist and adjust the indeces:

	tmppos = sortedlist.GetHeadPosition();
	while (tmppos)
	{
		GRSpringForceIndex *sfi = sortedlist.GetNext(tmppos);
		if (sfi->index > npos )
		{
			sfi->index--;
		}
	}
}


/** \brief This routine gets the extent when a given force is exerted ...
	First, the interval is determined, then the
	extent is calulated by linear interpolition.
*/
float GRSpaceForceFunction2::getExtent(float force) const
{
//	float count = sortedlist.GetCount();
	GRSpringForceIndex * tmpsfi = sortedlist.GetTail();
	
	if( tmpsfi == 0 )
	{
#if(0)
		std::cout << "null spring force index !" << std::endl;
#endif		
		return 0;
	}
	
	if (tmpsfi->force<= force && cmax > 0)
	{
		// then, we can just stretch using all springs ....
	
		float extent = force / cmax;
		return extent;
	}

	// if the force is smaller then the first one,
	// we just return xmin ....

	tmpsfi = sortedlist.GetHead();
	if (tmpsfi->spr->force > force)
	{
		return xmin;
	}
	
	// otherwise we have to go through the sortedlist and
	// then determine, where the force gets going ....
	// this is then an index into the scarr ...

	GuidoPos tmppos = sortedlist.GetHeadPosition();
	float myc = 0;  // my spring-constant ...
	float mymin = xmin;
	bool firstFlag = true;
	while(tmppos)
	{
		GRSpringForceIndex * tmpsfi = sortedlist.GetNext(tmppos);
		if (tmpsfi->force >= force)
		{
			// this case cannot happen, if it is the first
			// one ....
			// assert(first == 0);
			// assert(myc > 0.0);
			if (firstFlag)
				// then, myc is not yet defined ...
				return mymin;

			const float extent = mymin + force / myc;

			return extent;
		}
		if (firstFlag)
		{
			// the first spring-constant
			myc = tmpsfi->spr->sconst;
			firstFlag = false;
		}
		else
		{
			// the spring-constant is adjusted accordingly.
			//myc = 1.0 / ( (1.0/myc) + (1.0/tmpsfi->spr->sconst) );	// <- was
			myc = CalcSpringConstant( myc, tmpsfi->spr->sconst );
		}
		// we remove the prestretch from the springs that
		// will now be stretched aswell.
		mymin -= tmpsfi->spr->x;
	}
	assert(false);
	return 0;
}

/** \brief This routine gets the extent when a given
	force is exerted ...
	First, the interval is determined, then the
	extent is calulated by linear interpolition.
*/
void GRSpaceForceFunction2::writeAllExtents(ostream &os) const
{

	GuidoPos tmppos = sortedlist.GetHeadPosition();
	float myc = 0;  // my spring-constant ...
	float mymin = xmin;
	int first = 1;
	os << "xmin: " << xmin << endl;

	while(tmppos)
	{
		GRSpringForceIndex * tmpsfi = sortedlist.GetNext(tmppos);

		if (first)
		{
			// the first spring-constant
			myc = tmpsfi->spr->sconst;
			first = 0;
		}
		else
		{
			// the spring-constant is adjusted accordingly.
			//myc = 1.0 / ( (1.0/myc) + (1.0/tmpsfi->spr->sconst) );
			myc = CalcSpringConstant( myc, tmpsfi->spr->sconst );
		}

		// we remove the prestretch from the springs that
		// will now be stretched aswell.
		mymin -= tmpsfi->spr->x;

		float extent = mymin + tmpsfi->force / myc;
		os << tmpsfi->force << "\t" << extent << "\t";
		os << tmpsfi->spr->id << "\t" << tmpsfi->spr->sconst <<
			"\t" << tmpsfi->spr->x << "\t" << tmpsfi->spr->force << 
			"\t" << myc << "\t" << mymin <<  endl;
	}
}

/** \brief This calculated the force with a given extent.
 This is analogous to the getExtent-Function.
*/
float GRSpaceForceFunction2::getForce(float extent)
{
	// then we check, wether the extent is
	// smaller then xmin ...
	if (extent <= xmin) return 0; // we don't need a force!
	
	float myc = 0;
	int first = 1;
	float mymin = xmin;

	GuidoPos tmppos = sortedlist.GetHeadPosition();
	while (tmppos)
	{
		GRSpringForceIndex *tmpsfi = sortedlist.GetNext(tmppos);

		if (first)
		{
			myc = tmpsfi->spr->sconst;
			first = 0;
		}

		// we take the prestretch away from mymin....
		mymin -= tmpsfi->spr->x;

		float fx = (extent - mymin) * myc;

		if (tmppos)
		{
			// then, we have more springs in our list ....
			GRSpringForceIndex * next = sortedlist.GetAt(tmppos);
			if (fx <= next->force)
				return fx;

			// then we look further. in the next round,
			// the next spring will be stretched as well ....
			// myc = 1.0 / ( (1.0/myc)+(1.0/next->spr->sconst)); 
			
			myc = CalcSpringConstant( myc, next->spr->sconst );

		}
	}

	// if we are still around, then we have done it all ....
	// myc contains all the constants. mymin should be 0

	float fx = extent * myc;
	return fx;
}


/** \brief Adds another space-force-function.

	This is done to alculate the actual SFF for 
	a StaffManager
	(multiple SFFs are maintained, one for each breakpoint)
	this can be done intelligently ...
	we go through both original sortedlists
	and then interweave them according to the
	saved forces ...
*/
void GRSpaceForceFunction2::addSFF( const GRSpaceForceFunction2 & sff)
{
	int count1 = sortedlist.GetCount();
	int count2 = sff.sortedlist.GetCount();

	GuidoPos tmppos1 = sortedlist.GetHeadPosition();
	GuidoPos tmppos2 = sff.sortedlist.GetHeadPosition();

	GRSpringForceIndex * tmpsfi1 = 0;
	GRSpringForceIndex * tmpsfi2 = 0;

	do
	{
		if (tmppos1) tmpsfi1 = sortedlist.GetAt(tmppos1);
		else tmpsfi1 = 0;

		if (tmppos2) tmpsfi2 = sff.sortedlist.GetAt(tmppos2);
		else  tmpsfi2 = 0;

		if (tmpsfi1 && tmpsfi2)
		{
			if (tmpsfi1->force <= tmpsfi2->force)
			{
				sortedlist.GetNext(tmppos1);
			}
			else
			{
				// then, the second list has a "smaller" force entry
		
				GRSpringForceIndex * newsfi = new GRSpringForceIndex( tmpsfi2->spr, 
													 tmpsfi2->force, tmpsfi2->index + count1 );

				sortedlist.AddElementAt( tmppos1, newsfi );

				sff.sortedlist.GetNext( tmppos2 );
				
			}
		}
		else if (tmpsfi1)
		{
			// then, we do not have anything else in the second sff
			// we can just stop here ....
			break;
		}
		else if (tmpsfi2)
		{
			// then the original list is at its end ....
			// we just put everything in ...
			while (tmppos2)
			{
				GRSpringForceIndex *tmpsfi2 = sff.sortedlist.GetNext(tmppos2);
				GRSpringForceIndex * newsfi = new GRSpringForceIndex( tmpsfi2->spr, 
													tmpsfi2->force,tmpsfi2->index + count1 );
				sortedlist.AddTail(newsfi);
			}
			break;
		}
			
	}
	while (tmppos1 || tmppos2);

	// now we have to calculate the new scarr and the
	// new xmin ....

	// this is easy.
	xmin = xmin + sff.xmin;

	xminopt = xminopt + sff.xminopt;

	// we just add the new spring-constant ....

	// only the maximum ....
			// the last one ....
	if (count1 == 0)
	{
		cmax = sff.cmax;
		copt = sff.copt;
	}
	if (count2 == 0)
	{
		// cmax does not change 
		// maxbe is 0 already?
	}
	if (count1 > 0 && count2 > 0 )
	{
		float calt = cmax;
		//float cneu = 1.0/( (1.0 / calt) + (1.0/sff.cmax));
		float cneu = CalcSpringConstant( calt, sff.cmax );
		
		cmax = cneu;

		calt = copt;
		//cneu = 1.0/( (1.0 / calt) + (1.0/sff.copt));
		cneu = CalcSpringConstant( calt, sff.copt );
		copt = cneu;
	}
}


/** \brief Rresets the force of all springs within its 
		range so that it matches the key.
*/
void GRSpaceForceFunction2::ResetSprings()
{

	// this really works on the springs.
	// I am not sure, why and when this gets called?

	GuidoPos tmppos = sortedlist.GetHeadPosition();
	while (tmppos)
	{
		GRSpringForceIndex * tmpsfi = sortedlist.GetNext(tmppos);
		// only, if the spring is not frozen ....
		if (!tmpsfi->spr->isfrozen)
			tmpsfi->spr->change_force(tmpsfi->force);
	}

}

/** \brief Freezes a spring in the space-force-function.

	Freezing a spring is actually the process
	of setting the force to infinity ... then
	the spring can not be stretched by any external
	force. Its extension is already part of xmin.
*/
void GRSpaceForceFunction2::FreezeSpring(GRSpring *spr)
{
	assert(spr);

	// we search for the spring in our list.
	GuidoPos tmppos = sortedlist.GetHeadPosition();
	GRSpringForceIndex *sfi = NULL;
	while (tmppos)
	{

		GuidoPos savepos = tmppos;
		sfi = sortedlist.GetNext(tmppos);
		if (sfi->spr == spr)
		{
			// this should be sufficient.
			sfi->force = float(1e+9);
			sortedlist.setOwnership(0);
			sortedlist.RemoveElementAt(savepos);
			sortedlist.setOwnership(1);
			sortedlist.AddTail(sfi);

			break;
		}
	}

	if (!sfi) return;


	if (spr->force <= optforce)
	{
		// then we have to remove the spring-constant
		// form the average constant ....
		// and add the xmin-value 
		if (spr->sconst == copt)
		{
			copt = -1;
		}
		else
		{
			float calt = copt;
			//float cneu = 1.0 / ( (1.0/calt) - (1.0/spr->sconst) );
			float cneu = CalcSpringConstant( -calt, spr->sconst );
			copt = cneu;
		}

		xminopt += spr->x;
	}
	else
	{
		// then we have to do nothing. 
	}

	spr->isfrozen = 1;
}

/** \brief Called to unfreeze a spring in the spaceforcefunction.
*/
void GRSpaceForceFunction2::UnfreezeSpring(GRSpring * spr)
{
	assert(spr);

	// first, we search for the spring in the the sortedlist

	GRSpringForceIndex * sfi = NULL;
	GuidoPos tmppos = sortedlist.GetTailPosition();
	while (tmppos)
	{
		GuidoPos savepos = tmppos;
		sfi = sortedlist.GetPrev(tmppos);
		if (sfi->spr == spr)
		{
			// this puts the force back.
			sfi->force = spr->force;

			sortedlist.setOwnership(0);
			sortedlist.RemoveElementAt(savepos);
			sortedlist.setOwnership(1);

			break;
		}
	}

	if (!sfi) return;

	// now, we have to put the sfi in the correct position
	// of the list ....
	tmppos = sortedlist.GetHeadPosition();
	while (tmppos)
	{
		GuidoPos savepos = tmppos;
		GRSpringForceIndex * tmpsfi = sortedlist.GetNext(tmppos);
		if (tmpsfi->force > sfi->force)
		{
			// we put the sfi at the correct position ....
			sortedlist.AddElementAt(savepos,sfi);
			sfi = NULL;
			break;
		}
	}

	// the force of the sfi is so big that it must be placed at the end.
	if (sfi)
		sortedlist.AddTail(sfi);

	if (spr->force <= optforce)
	{
		// then we have to add the spring-constant
		// form the average constant ....
		// and remove the xmin-value 
		if (copt == -1)
		{
			copt = spr->sconst;
		}
		else
		{
			float calt = copt;
			//float cneu = 1.0 / ( (1.0/calt) + (1.0/spr->sconst) );
			float cneu = CalcSpringConstant( calt, spr->sconst );
			copt = cneu;
		}

		xminopt -= spr->x;
	}
	else
	{
		// then we have to do nothing. 
	}

	spr->isfrozen = 0;

}

/** \brief Evaluates a break-position. It gets the wanted extent and 
	then determines an evaluation-value. 
	
	A value of 1 is most desirable ...
	Everything bigger or smaller is	worse. It works as follows: first, the
	extent with zero force (ex0) is determined. Then, the force for reaching 
	the wanted extent is calculated. 
*/
float GRSpaceForceFunction2::EvaluateBreak(float extent)
{

//	float ex0 = getExtent(0.0);
	float neededforce = getForce(extent);
	float optimum = 600;

	float evalret; 

	if (neededforce <= 0)
	{
		evalret = 0;
		return evalret;
	}
	if (neededforce <= optimum)
	{
		evalret = optimum / neededforce;
		return evalret;
	}
	else
	{
		if (neededforce >= 2* optimum)
		{
			evalret = 0;
			return evalret;
		}
		evalret = 1.0f - (neededforce-optimum) / optimum;
		return evalret;
	}

	assert(false);
	return 0;
}

// this is another constructor for copying 
// the complete sff (including the springs)
GRSpaceForceFunction2::GRSpaceForceFunction2( const GRSpaceForceFunction2 & sff )	
																	: sortedlist(1)
{
	xmin = 0;
	xminopt = 0;
	copt = -1;

	addSFF(sff);
}

float GRSpaceForceFunction2::optforce = kSettingDefaultForce;

float GRSpaceForceFunction2::getOptForce()
{
	return optforce; 
}

void GRSpaceForceFunction2::setOptForce(float newoptforce)
{
	optforce = newoptforce;
}
