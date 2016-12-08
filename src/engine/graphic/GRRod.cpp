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

#include "GRRod.h"
#include "GRSpringForceIndex.h"
#include "GRVoice.h"
#include "kf_ivect.h"

GRRod::GRRod(const GRNotationElement * gr1, const GRNotationElement * gr2,
			 int lastsprid, float spacedistance, float aOptForce) : optForce(aOptForce)
{
	// this calculates the space needed for the right part of gr1 and for the left part of gr2
	mLength = spacedistance;

	// - Get springs ID
	int spr1 = gr1->getSpringID();
	int spr2 = gr2->getSpringID();
	if( spr2 < 0 ) spr2 = lastsprid;

	setSpringIDs( spr1, spr2 );
	mIsSpaceRod = false;
	mForce = -1;
}

GRRod::GRRod( const GRNotationElement * gr1, const GRNotationElement * gr2, int lastspringid, float aOptForce)
	: optForce(aOptForce)
{
	const float r1 = gr1->getRightSpace();
	const float r2 = gr2->getLeftSpace();

	// this calculates the space needed for the right part of gr1 and for the left part of gr2
	mLength = r1 + r2;
	// - Get springs ID
	int spr1 = gr1->getSpringID();
	int spr2 = gr2->getSpringID();
	if( spr2 < 0) spr2 = lastspringid;

	setSpringIDs( spr1, spr2 );
	mIsSpaceRod = false;
	mForce = -1;
}

GRRod::GRRod( const GRRod & rod )
{
	mGrStaff = rod.mGrStaff;
	mLength  = rod.mLength;
	setSpringIDs( rod.mSpr1,  rod.mSpr2 );
	mIsSpaceRod = rod.mIsSpaceRod;
	mForce = -1;
	optForce = rod.optForce;
}

GRRod::GRRod( float inLength, int inSpring1, int inSpring2, float aOptForce)
	: optForce(aOptForce)
{
	setSpringIDs( inSpring1, inSpring2 );
	mLength = inLength;
	mIsSpaceRod = false;
	mForce = -1;
}

/** \brief Calculates the force neccessary to stretch all springs that are
spanned by this rod to the length of the rod.

Only those strings can be stretched further, whose
stored force is smaller than the newly applied force.
the formula works like this
f(F) := F/D_1 + F/D_2 + ... + F/D_n = length

	# Idea: 
	# 1: get the different F's that are already applied to the springs. 
	# 2: sort them
	# 3. calculate the length at the different F's
	# 4. find the segment, where the solution must be
	# 5. solve the equation analytically.

save the forces ..
just change the force, if any of the spring-lengths were changed ....
(how do I know, wich springs are in which rods? -> rods are ordered by spring-position
think about it 
juergens example with wrong meter-params
invent15,gmn
*/
float GRRod::calcforce(SpringVector * sprvect)
{
	if( mForce >= 0 ) return mForce;

	// now we work with a GRSpaceForceFunction  (partial)
	GRSpaceForceFunction2 sff(optForce);
	for (int i = mSpr1; i < mSpr2; ++i ) {
		GRSpring * spr = sprvect->Get(i);
		if (spr) {
			sff.addSpring(spr);
			if (spr->fIsfrozen)
				sff.FreezeSpring(spr);
		}
	}

	if (sff.getExtent(0) >= mLength) {
		// no stretching necessary ... 
		mForce = 0;
		return 0;
	}

	mForce = sff.getForce(mLength);
	if (mForce < 0 ) {
		// then, probably all springs are frozen .. , then unfreeze the LAST spring
		sff.UnfreezeSpring(sprvect->Get(mSpr2-1));
		mForce = sff.getForce(mLength);
	}
	assert( mForce >= 0 );	
	return mForce;
}

/** \brief Calculates the length that the given force will generate.
*/
float GRRod::calclength( float inForce, SpringVector * sprvect ) // doh! force is also a class variable
{
	assert( inForce >= 0 );
	float result = 0;
	
	const int s1 = getSpr1();
	const int s2 = getSpr2();
	
	for (int i = s1; i < s2; ++i )
	{
		GRSpring * spr = sprvect->Get(i);
		if (!spr->fIsfrozen && inForce >= spr->getForce())
			result += spr->apply_force(inForce);
		else
			result += spr->fX;
	}
	return result;

}

/** \brief Stretches the spanned springs with the given force.
*/
float GRRod::stretchsprings(float inForce, SpringVector * sprvect) // doh! force is also a class variable
{
	assert( inForce >= 0 );
	float nlength = 0;
	int allfrozen = 1;
	const int s1 = getSpr1();
	const int s2 = getSpr2();
	for (int i = s1; i < s2; ++i )
	{
		GRSpring * spr = sprvect->Get(i);
		if (!spr->fIsfrozen)
			allfrozen = 0;

		if (!spr->fIsfrozen && inForce > spr->getForce())
			nlength += spr->change_force(inForce);

		else if (spr->fIsfrozen) {
			if (i == (mSpr2 - 1) && allfrozen) {
				// temporarily unfreeze the LAST spring, if all are frozen
				assert(spr->fIsfrozen);
				spr->fIsfrozen = 0;
				nlength += spr->change_force(inForce);
				spr->fIsfrozen = 1;
			}
			else nlength += spr->fX;
		}
	}
	return nlength;
}

/** \brief reset the force, in case the start and end
	spring of the rod rd do interfere
*/
int GRRod::resetForce(const GRRod & rd)
{
	if (mForce < 0) return 0;
	if ( (rd.mSpr2 > mSpr1 && rd.mSpr2 <= mSpr2)
		|| (rd.mSpr1 >= mSpr1 && rd.mSpr1 < mSpr2)
		|| (rd.mSpr2 >= mSpr2 && rd.mSpr1 <= mSpr1) )
	{
		  mForce = -1;
		  return 1;
	}
	return 0;
}

int GRRod::operator<(const GRRod & r2) const
{
	const int s1 = spansOne();
	const int s2 = r2.spansOne();
	if (s1 && !s2) return -1;
	if (s2 && !s1) return 1;

	if (mSpr1 < r2.mSpr1)		return -1;
	else if (mSpr1 == r2.mSpr1)	return 0;
	else						return 1;
	return 0;
}

