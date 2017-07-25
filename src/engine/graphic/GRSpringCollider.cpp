/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRSpringCollider.h"
#include "GRDefine.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRVoice.h"

// #include "kf_vect.h"
// #include "kf_ivect.h"
 
GRSpringCollider::GRSpringCollider() : mPosListVector( 1 )
{
	// mPosListVector = 0; // now a embeded object (was a pointer)
}

GRSpringCollider::~GRSpringCollider()
{
	// delete mPosListVector;
}

void GRSpringCollider::AddElement(GRNotationElement * el, GuidoPos posel)
{
	// we have to find out the y-extent of the el ....
	// if( mPosListVector == 0 )
	//	mPosListVector = new poslistvector(1);// owns elements !

	assert(el);

	GRStaff * grstaff = el->getGRStaff();
	int staffnum = 0;
	if (grstaff)
	{
		GRSystemSlice * sysslc = grstaff->getGRSystemSlice();
		if (sysslc)
			staffnum = sysslc->getStaffNumber(grstaff);
		else
		{
			GRSystem * grsys = grstaff->getGRSystem();
			if (grsys)
				staffnum = grsys->getStaffNumber(grstaff);
		}
	}

	const NVPoint & pt = el->getPosition();
	const NVRect & rt = el->getBoundingBox();
	float fminy = pt.y + rt.top;
	float fmaxy = pt.y + rt.bottom;
	
	const float invSpace = (float(2) / LSPACE);
	const float offset = float(staffnum * 20);
	
	const int miny = (int)(fminy * invSpace + offset); 
	const int maxy = (int)(fmaxy * invSpace + offset);  

	for( int i = miny; i <= maxy; ++i )
	{
		// check, wether the position is already used .

		poslist * ppl = mPosListVector.Get(i);
		if (ppl)
		{
		}
		else
		{
			ppl = new poslist;
		}
		ppl->AddTail(posel);
		mPosListVector.Set(i, ppl);
	}
}

void GRSpringCollider::RemoveElement(GRNotationElement * el, GuidoPos posel)
{
	int i;
//	if (mPosListVector)
//	{
		i = mPosListVector.GetMinimum();
		for (; i <= mPosListVector.GetMaximum(); ++i)
		{
			poslist * pl = mPosListVector.Get(i);
			if (pl)
			{
				GuidoPos pos = pl->GetHeadPosition();
				while (pos)
				{

					GuidoPos oldpos = pos;
					GuidoPos lpos = pl->GetNext(pos);
					if (lpos == posel)
					{
						pl->RemoveElementAt(oldpos);
					}
				}
				if (pl->GetCount() == 0)
				{
					// then we need to remove the element
					// from the mPosListVector ...
					mPosListVector.Set(i,NULL);
					delete pl;
				}
			}
		}
//	}
}

/** \brief Called by a spring which owns this instance of GRSpringCollider and knows the 
	 SpringCollider of the next spring. 
	 
	 On the first run, count equals -1, after that, count is the index
	into the poslistvector of this instance.
*/
int GRSpringCollider::GetMatchingPoslists(int & count, const GRSpringCollider * inCollider,
								poslist ** pl1, poslist ** pl2)
{
	if (count == KF_VECTOR_MININDEX)
	{
	//	if (mPosListVector)
			count = mPosListVector.GetMinimum();
	//	else
	//	{
	//		*pl1 = NULL;
	//		*pl2 = NULL;
	//		return -1;
	//	}

	}
	if (count == KF_VECTOR_MININDEX)
	{
		count = -1;
		*pl1 = NULL;
		*pl2 = NULL;
		return -1;
	}

	for( ; ; )
	{
		*pl1 = mPosListVector.Get(count);

		// now we try to find the same pl in the other SpringCollider.
		if (inCollider ) // && sprcol.mPosListVector)
		{
			*pl2 = inCollider->mPosListVector.Get(count);
		}
		else
			*pl2 = 0;

		count = mPosListVector.GetNextIndex(count);


		if (*pl1 && *pl2)
			break;

		if (count == KF_VECTOR_MININDEX)
		{
			*pl1 = NULL;
			*pl2 = NULL;
			break;
		}

		*pl1 = NULL;
		*pl2 = NULL;
	}

	return 0;
}
