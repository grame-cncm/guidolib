/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cmath>
#include <iostream>

#include "GuidoDefs.h"
#include "NVPoint.h"

#include "ARBowing.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARBowing::ltpls(1);

using namespace std;

ARBowing::ARBowing()
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	dx1 = dy1 = dx2 = dy2 = r3 = h = 0;
	mCurve = 0;

	mParSet = false;
}

ARBowing::ARBowing(const ARBowing * bowing)
:			 ARMTParameter(-1,bowing)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	dx1 = dy1 = dx2 = dy2 = r3 = h = 0;
	mCurve = 0;

	mParSet = 0;

	if (bowing->mCurve)	// "down" or "up"
	{
		mCurve = TagParameterString::cast( bowing->mCurve->getCopy());
	}
	if (bowing->dx1)
	{
		dx1 = TagParameterFloat::cast( bowing->dx1->getCopy());
	}
	if (bowing->dy1)
	{
		dy1 = TagParameterFloat::cast( bowing->dy1->getCopy());
	}
	if (bowing->dx2)
	{
		dx2 = TagParameterFloat::cast( bowing->dx2->getCopy());
	}
	if (bowing->dy2)
	{
		dy2 = TagParameterFloat::cast( bowing->dy2->getCopy());
	}
	if (bowing->r3)
	{
		r3 = TagParameterFloat::cast( bowing->r3->getCopy());
	}
	if (bowing->h)
	{
		h = TagParameterFloat::cast( bowing->h->getCopy());
	}
}

ARBowing::~ARBowing()
{
	delete dx1;
	delete dy1;
	delete dx2;
	delete dy2;
	delete r3;
	delete h;
	delete mCurve;
}

void ARBowing::print() const
{
}

void ARBowing::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		
		// \slur<curve>  // TODO: add dx's and dy's ?
		lstrs.AddTail(("S,curve,down,o"));

		// \slur<dx1,dy1, dx2,dy2, r3,h> 
		lstrs.AddTail( "U,dx1,2hs,o;U,dy1,1hs,o;"
			             "U,dx2,-2hs,o;U,dy2,1hs,o;"
			             "F,r3,0.5,o;U,h,2hs,o" );
			             
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 1)
		{
			dx1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx1);
			if (dx1->TagIsSet())
				mParSet = true;

			dy1 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy1);
			if (dy1->TagIsSet())
				mParSet = true;
	
			dx2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx2);
			if (dx2->TagIsSet())
				mParSet = true;

			dy2 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy2);
			if (dy2->TagIsSet())
				mParSet = true;

			r3 = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(r3);
			if (r3->TagIsSet())
				mParSet = true;

			h = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(h);
			if (h->TagIsSet())
				mParSet = true;
		}
		else if (ret==0)
		{
			// then, we have the "curve"-parameter ...
			// depending on upward or downward curve, we 
			// set the dx-parameters.
			mCurve = TagParameterString::cast(rtpl->RemoveHead());
			assert(mCurve);
			if (mCurve->TagIsSet()) mParSet = true;

			std::string down ("down");
			if (down == mCurve->getValue())
			{
				// we have a downward-curve
				dx1= new TagParameterFloat( 0 ); // 1.2f); // (JB) to be removed ? (now automatic)...
				dx1->setUnit("hs");
				
				dy1= new TagParameterFloat( 0 ); // -1.1f);
				dy1->setUnit("hs");

				dx2= new TagParameterFloat( 0 ); // -1.2f);
				dx2->setUnit("hs");

				dy2 = new TagParameterFloat( 0 ); // -1.1f);
				dy2->setUnit("hs");

				r3 = new TagParameterFloat( 0.5f);

				h  = new TagParameterFloat( -2.0f);
				h->setUnit("hs");
			}
			else
			{
				// we assume an upward curve ...
				dx1 = new TagParameterFloat( 0 ); // 1.2f);
				dx1->setUnit("hs");
				
				dy1 = new TagParameterFloat( 0 ); // 1.1f);
				dy1->setUnit("hs");

				dx2 = new TagParameterFloat( 0 ); // -1.2f);
				dx2->setUnit("hs");

				dy2 = new TagParameterFloat( 0 ); // 1.1f);
				dy2->setUnit("hs");

				r3 = new TagParameterFloat( 0.5f);

				h = new TagParameterFloat( 2.0f);
				h->setUnit("hs");
			}
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
	return;
}

//void ARBowing::Reset()
//{
//	dy1 = 0.0;
//	dy2 = 0.0;
//	dx3 = 0.0;
//	dy3 = 0.0;
//	parcount = 0;
//	hascontrol = 0;
//}

void ARBowing::PrintParameters(std::ostream & os) const
{
	int previous = 0;
	if (mParSet)
	{
		os << "<";
	}
	if (mCurve && mCurve->TagIsSet())
	{
		os << "curve=\"" << mCurve->getValue() << "\"";
	}
	else
	{
		if (dx1 && dx1->TagIsSet())
		{
			os << "dx1=" << dx1->getUnitValue() << dx1->getUnit();
			previous = 1;
		}
		if (dy1 && dy1->TagIsSet())
		{
			if (previous)
				os << ",";
			os << "dy1=" << dy1->getUnitValue() << dy1->getUnit();
			previous = 1;
		}
		if (dx2 && dx2->TagIsSet())
		{
			if (previous)
				os << ",";
			os << "dx2=" << dx2->getUnitValue() << dx2->getUnit();
			previous = 1;
		}
		if (dy2 && dy2->TagIsSet())
		{
			if (previous)
				os << ",";
			os << "dy2=" << dy2->getUnitValue() << dy2->getUnit();
			previous = 1;
		}
		if (r3 && r3->TagIsSet())
		{
			if (previous)
				os << ",";
			os << "r3=" << r3->getUnitValue();
			previous = 1;
		}
		if (h && h->TagIsSet())
		{
			if (previous)
				os << ",";
			os << "h=" << h->getUnitValue() << h->getUnit();
			previous = 1;
		}
	}
	
	if (mParSet)
		os << ">";
}

TagParameterList * ARBowing::getTagParameterList() const
{
	TagParameterList * tpl = new TagParameterList(1);

	// now we see, which parameters are set ....
	if (mCurve && mCurve->TagIsSet())
	{
		tpl->AddTail(mCurve->getCopy());
		return tpl;
	}

	if (dx1 && dx1->TagIsSet())
	{
		tpl->AddTail(dx1->getCopy());
	}
	if (dy1 && dy1->TagIsSet())
	{
		tpl->AddTail(dy1->getCopy());
	}
	if (dx2 && dx2->TagIsSet())
	{
		tpl->AddTail(dx2->getCopy());
	}
	if (dy2 && dy2->TagIsSet())
	{
		tpl->AddTail(dy2->getCopy());
	}
	if (r3 && r3->TagIsSet())
	{
		tpl->AddTail(r3->getCopy());
	}
	if (h && h->TagIsSet())
	{
		tpl->AddTail(h->getCopy());
	}
	if (color && color->TagIsSet())
	{
		tpl->AddTail(color->getCopy());
	}

	return tpl;
}

/** \brief Called by Graphics to automatically set the curvature of slurs/ties.
	we should probably also use the begin- and end-point
	of the bow to determine optimum curvature ...
*/
void ARBowing::setCurve(int ,const NVPoint & p1, const NVPoint & p2)
{
	// this only works, if the parameters have NOT been set
	// by the user before-hand ...

	if (mParSet) return;

	// we take into account the distance for the curvature ...
	const float distx = float(p2.x - p1.x);
	const float disty = float(p2.y - p1.y);
	float alpha = float(atan2(disty,distx));
	const float myr3 = (float(0.5) * distx) / cos(alpha) / distx;
	float myh = float(1.5); // 2hs are standard (for ties?)

	alpha = abs( alpha );
	if( alpha > (float)0.02)
	{
		myh = float(distx * float(0.5) * tan(alpha) / LSPACE * 2);

		if (myh < float(1.5))
			myh += float(1.5);
	}

	if (distx < 151) // ? hardcoded
	{
		if (myh == float(1.5))
			myh = float(0.75);
	}

	if (dx1) dx1->reset(0,"hs");
	if (dy1) dy1->reset(0,"hs");
	if (dx2) dx2->reset(0,"hs");
	if (dy2) dy2->reset(0,"hs");
	if (r3) r3->reset(myr3,"\0");
	if (h)	h->reset(myh,"hs");


//	if (curve == 1)
//	{
		// upward

		// WARNING: DON'T DELETE MEMBERS HERE OTHERWISE EXTERNAL POINTERS
		// MAY POINT TO INVALID MEMORY ZONE !

/*		delete dx1;
		dx1= new TagParameterFloat( 0 ); // (JB) was: 1.2f);
		dx1->setUnit("hs"); */
//		dx1->reset(0,"hs");
		
/*		delete dy1;
		dy1= new TagParameterFloat( 0 ); // (JB) was: 1.1f);
		dy1->setUnit("hs"); */
//		dy1->reset(0,"hs");
		
/*		delete dx2;
		dx2= new TagParameterFloat( 0 ); // (JB) was: -1.2f);
		dx2->setUnit("hs"); */
//		dx2->reset(0,"hs");
		
/*		delete dy2;
		dy2 = new TagParameterFloat( 0 ); // (JB) was: 1.1f);
		dy2->setUnit("hs"); */
//		dx2->reset(0,"hs");
		
/*		delete r3;
		r3 = new TagParameterFloat( myr3 );*/ 
//		r3->reset(myr3,"\0");
		
/*		delete h;
		h  = new TagParameterFloat( myh );
		h->setUnit("hs");*/
//		h->reset(myh,"hs");

/*	}
	else if (curve == -1)
	{
		// downward
		// we have a downward-curve
		delete dx1;
		dx1= new TagParameterFloat( 0 ); // (JB) was: 1.2f);
		dx1->setUnit("hs");
		
		delete dy1;
		dy1= new TagParameterFloat( 0 ); // (JB) was: -1.1f);
		dy1->setUnit("hs");
		
		delete dx2;
		dx2= new TagParameterFloat( 0 ); // (JB) was: -1.2f);
		dx2->setUnit("hs");
		
		delete dy2;
		dy2 = new TagParameterFloat( 0 ); // (JB) was: -1.1f);
		dy2->setUnit("hs");
		
		delete r3;
		r3 = new TagParameterFloat(myr3);
		
		delete h;
		h  = new TagParameterFloat(-myh);
		h->setUnit("hs");
	}
*/
}
