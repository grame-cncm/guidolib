/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

// if this is defined, then the GRSystem writes the
// used spaceforcefunction into a file
// #define  _DEBUGSFF

// C++
#include <fstream>
#include <sstream>
#include <climits>
#include <typeinfo>
#include <iostream>

// Guido Misc
#include "secureio.h"
#include "kf_ivect.h"
#include "GRDefine.h"
#include "VGDevice.h"
#include "NVRect.h"

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"


// Guido AR
#include "ARSystemFormat.h"
#include "ARMusicalVoice.h"	// for template instanciation
#include "ARMusic.h"
#include "ARAccol.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "ARStaff.h"

// Guido GR
#include "GRSystem.h"
#include "GRSystemTag.h"
#include "GRBeam.h"
#include "GRBar.h"
#include "GRPage.h"
#include "GRMusic.h"
#include "GRStaffManager.h"
#include "GRText.h"
#include "GRAccolade.h"

#include "GRBar.h"
#include "GRGlue.h"
#include "GRSpecial.h"
#include "GRKey.h"
#include "GRSystemTag.h"
#include "GRSliceHeight.h"
#include "GRPossibleBreakState.h"

using namespace std;

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		cout << (void*)this << " GRSystem::" << method << endl
#else
#define traceMethod(method)	
#endif
#define trace1Method(method)		cout << (void*)this << " GRSystem::" << method << endl


GRStaff * gCurStaff;

int GRSystem::sSystemID = 0;

/** \brief Used by GRStaffManager to create Systems on the fly as needed.
*/
GRSystem::GRSystem( GRPage * inPage, const TYPE_TIMEPOSITION & relativeTimePositionOfSystem )
  :  GREvent(NULL, inPage->getARMusic(), relativeTimePositionOfSystem, DURATION_0),
	 mStaffs( new StaffVector(true)) // (1) == staff owns elements!
	 // spacelist and zerospacelist are handled automatically (do not own elements!)
{
	assert(false);
	mMarginLeft = 0;
	mDistanceSet = false;
	mDistance = 0;
	mNewLinePage = 0;
	mPage = inPage;
	mNextStaffPosition.x = 0; // 5*LSPACE;
	mNextStaffPosition.y = 0; // 5*LSPACE;

	mSpringVector = NULL;
	simplerods = NULL;
	complexrods = NULL;
#ifdef OLDSPFACTIVE
	spf = NULL;
#endif
	mSpaceForceFunc = NULL;
	mIsLastLine = false;
	float tmpf = -1.0f;
	inPage->addSystem( this, &tmpf );
}

//--------------------------------------------------------------------------------------------------------
/*
	DF - sept. 10 2009 - new internal function from GRSystem constructor structuration
*/
void GRSystem::InitForceFunction (GRStaffManager * staffmgr, SSliceList ** psystemslices, int count)
{
	int icount = 0;
	int endSpringID = -1, startSpringID = -1;
	GRSystemSlice * previousSlice = 0;
	GuidoPos pos = (*psystemslices)->GetHeadPosition();
	while (pos)
	{
		icount++;
		GuidoPos prevpos = pos;
		GRSystemSlice * slice = (*psystemslices)->GetNext(pos);

		endSpringID = slice->mStartSpringID;
		if ((startSpringID >= 0) && (startSpringID < endSpringID))
		{
			// I have to adjust the SFF's
			GRSpring * spr = staffmgr->mSpringVector->Get(startSpringID);

			if (previousSlice->mForceFunction)
				previousSlice->mForceFunction->deleteSpring(spr);
			else if (previousSlice->mPossibleBreakState && previousSlice->mPossibleBreakState->sff)
				previousSlice->mPossibleBreakState->sff->deleteSpring(spr);

			staffmgr->InitialSpringStretch (startSpringID, endSpringID, staffmgr->mSimpleRods,
						staffmgr->mComplexRods, staffmgr->mSpringVector);

			if (previousSlice->mForceFunction)
				previousSlice->mForceFunction->addSpring(spr);
			else if (previousSlice->mPossibleBreakState && previousSlice->mPossibleBreakState->sff)
				previousSlice->mPossibleBreakState->sff->addSpring(spr);
		}

		startSpringID = slice->mEndSpringID;
		AddSystemSlice( slice );
		GetCompositeElements().DumpListAtTail( &slice->GetCompositeElements());

		slice->mGrSystem = this;
		if (count == icount)
		{
			SSliceList * pneu;
			(*psystemslices)->Cut(prevpos, &pneu);
			// this just deletes the old list, without actually deleting
			// the content (the original list does not own the slices)
			delete (*psystemslices);
			*psystemslices = pneu;
			break;
		}
		previousSlice = slice;
	}
}

//--------------------------------------------------------------------------------------------------------
/*
	DF - sept. 10 2009 - new internal function from GRSystem constructor structuration
*/
void GRSystem::AdjustForceFunction (GRSliceHeight &sliceheight, int &startspring, int &endspring)
{
	// then we need to Merge the mForceFunction's of the slices and stretch the springs accordingly ....
	// ATTENTION, we have to take care of the boundary-rods! Maybe we need to restretch some of those springs !!!
	mSpaceForceFunc = new GRSpaceForceFunction2();

	// We also have to take care of the distance between staff-elements
	// and set all staff-distances equally to get the correct continuation.
	startspring = INT_MAX;
	endspring = -1;
	GuidoPos pos = mSystemSlices.GetHeadPosition();
	while (pos)
	{
		// GRSpaceForceFunction2 *tmpsff;
		GRSystemSlice * slice = mSystemSlices.GetNext(pos);
		if (slice)
		{
			sliceheight.AddSystemSlice(slice);
			if (slice->mForceFunction)
				mSpaceForceFunc->addSFF(*slice->mForceFunction);
			else if (slice->mPossibleBreakState && slice->mPossibleBreakState->sff)
				mSpaceForceFunc->addSFF(*slice->mPossibleBreakState->sff);

			// the slice really starts at the startSpringID
			GRSpring * spr = mSpringVector->Get(slice->mStartSpringID); // firstrod->spr1-1);
			if (spr)
			{
				if (!slice->mStartGlue)
				{
					slice->mStartGlue = new GRGlue(NULL,0);
					slice->mStartGlue->addAssociation(slice);
					spr->addElement(slice->mStartGlue,NULL);
				}
			}
			if (slice->mStartSpringID < startspring)
			{
				startspring = slice->mStartSpringID; // firstrod->spr1;
				if (endspring == -1)
					endspring = startspring;
			}
			// it ends at the endSpringID , BUT if the association is set, 
			// the extent of the spring is taken into account when drawing the staff ....
			spr = mSpringVector->Get(slice->mEndSpringID);
			if (spr) {
				if (!slice->mEndGlue) {
					slice->mEndGlue = new GRGlue(NULL,0);
					slice->mEndGlue->addAssociation(slice);
					spr->addElement(slice->mEndGlue, NULL);
					slice->mEndGlue->tellSpringID(slice->mEndSpringID);
				}
			}

			if (slice->mEndSpringID > endspring)
				endspring = slice->mEndSpringID;

			if (slice != mSystemSlices.GetTail()) {
				// these possible break states are no longer needed.
				// we might as well delete them here ....
				delete (slice->mPossibleBreakState);
				slice->mPossibleBreakState = NULL;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------
/*
	DF - sept. 10 2009 - new internal function from GRSystem constructor structuration
*/
GRStaff * GRSystem::ComputeBoundingBox (GRSliceHeight &sliceheight)
{
	GRStaff * lastStaff = 0;
	GuidoPos pos = mSystemSlices.GetHeadPosition();
	while (pos)
	{
		GRSystemSlice * slice = mSystemSlices.GetNext(pos);
		if (slice)
		{
			NVPoint curp;
			for( int j = slice->mStaffs->GetMinimum() ; j<=slice->mStaffs->GetMaximum(); j++)
			{
				GRStaff * tmpstaff = slice->mStaffs->Get(j);
				if (tmpstaff)
				{
					const NVRect & br = tmpstaff->getBoundingBox();
					curp.y = (GCoord)sliceheight.getPosition(j);
					tmpstaff->setPosition(curp);
					if (curp.y + br.top < mBoundingBox.top)
						mBoundingBox.top = curp.y + br.top;
					if (curp.y + br.bottom > mBoundingBox.bottom)
						mBoundingBox.bottom = curp.y + br.bottom;
					lastStaff = tmpstaff;
				}
			}
			GuidoPos epos = First();
			while (epos)
			{
				GRNotationElement * el = GetNext(epos);
				GRBar * tmpbar = dynamic_cast<GRBar *>( el );
				if (tmpbar && tmpbar->getTagType() == GRTag::SYSTEMTAG)
				{
					NVRect & br = tmpbar->getReferenceBoundingBox();
					br.bottom = (GCoord)( lastStaff->getPosition().y + lastStaff->getDredgeSize());
				}
			}
		}
	}
	return lastStaff;
}

//--------------------------------------------------------------------------------------------------------
/** \brief this constructor gets a list of systemslices together with a count and takes this from the list.

	Ownership is transfered to the system: it actually takes possession of the slices in the list. 
	The caller looses the slices.

   Then the global space force function (ssf) is build, so that the whole system can actually be stretched.
*/
GRSystem::GRSystem(	GRStaffManager * staffmgr, GRPage * inPage,
					const TYPE_TIMEPOSITION & relativeTimePositionOfSystem,
					SSliceList ** psystemslices, int count,
					GRSystemSlice * beginslice, ISpringVector ** pvect, ARSystemFormat * sysform,
					bool islastsystem )

			: GREvent( 0, inPage->getARMusic(), relativeTimePositionOfSystem, DURATION_0 ),
				mSystemSlices(1), mStaffs( 0 )
{
	traceMethod("GRSystem");
	// also, we need to take the springs from the spring-vector ....
	mMarginLeft = 0;
	mDistanceSet = 0;
	mDistance = 0;
	mNewLinePage = 0;
	mPage = inPage;
	mNextStaffPosition.x = mNextStaffPosition.y = 0;

	mSpringVector = NULL;
	simplerods = NULL;
	complexrods = NULL;
	mSpaceForceFunc = NULL;

	mIsLastLine = false;

	// This sets the left indent, if one is present ....
	setSystemFormat(sysform);
	GRSliceHeight sliceheight;

	if( beginslice )
	{
		sliceheight.AddSystemSlice(beginslice,1);
		AddSystemSlice( beginslice );
		GetCompositeElements().DumpListAtTail( &beginslice->GetCompositeElements());
		beginslice->mGrSystem = this;
	}

	InitForceFunction (staffmgr, psystemslices, count);
	// what about ivector ... otherwise I do not have any springs !?!?!
	// maybe we just borrow the springs from the staff-manager ?
	// I just do not know ....
	mSpringVector = *pvect;
	int startspring, endspring;
	AdjustForceFunction (sliceheight, startspring, endspring);

	// now we freeze the endspring ....
	GRSpring * spr = mSpringVector->Get(endspring);
	if (spr && spr->containsBar())			// actually we freeze it only if it's a barline ....
		mSpaceForceFunc->FreezeSpring(spr);

	ISpringVector * newvect;

	// this cuts the mSpringVector so that the newvect is
	// the vector beginning at the minimum going to (and including) endspring.
	// mSpringVector is just the rest .... (minimum is adjusted, count as well ...)
	mSpringVector->CutBegin( endspring, &newvect );

	ISpringVector * mytmpvector = mSpringVector;
	mSpringVector = newvect;		// this is the vector from the beginning
	*pvect = mytmpvector;			// This is the returned sprvector (the rest of the springs)
	GRSystemSlice * endslice = (mSystemSlices.empty() ? 0 : mSystemSlices.back()); // GetTail();
    assert (endslice);

	// now we add the endglue to the system .....
	TYPE_TIMEPOSITION mytp (endslice->mPossibleBreakState->tp);
	spr = new GRSpring(mytp, DURATION_0);
	spr->setID(++endspring);
	spr->isfrozen = 1;
	mSpringVector->Set(endspring,spr);

	int i;
	for (i=endslice->mStaffs->GetMinimum();
	         i<=endslice->mStaffs->GetMaximum(); i++)
	{
		GRStaff * staff = endslice->mStaffs->Get(i);
		if (staff)
		{
			// this call adds an endglue to this staff (attaching it to the last spring)
			staff->EndStaff(mytp,spr);
		}
	}
	
	GRStaff * lastStaff = ComputeBoundingBox(sliceheight);

	// then we have the mSpaceForceFunc ....
	// we have to decide, wether to stretch the system to the desired with or whether to
	// use the optimum force (for a single line ...)
	float extent = getSystemWidthCm() * kCmToVirtual;
	mSystemforce = mSpaceForceFunc->getForce(extent);

	// if we are at the very beginning and there is no begin-slice and
	// there are no more systemslices left, then this is the very first line ...
	// we have to find out, if the required force is greater than the optimum force, in which case the force
	// is set to the optimum, the
	if (mRelativeTimePositionOfGR == DURATION_0
		&& !beginslice
		&& (*psystemslices)->empty()
		&& islastsystem)
	{
			float alternate_force = GRSpaceForceFunction2::getOptForce();
			if (mSystemforce > alternate_force)
				mSystemforce = alternate_force;
	}

#ifdef _DEBUGSFF
	char mybuffer[200];
	ostrstream myfname(mybuffer,200);
	int i1;
	if (!beginslice)
		i1 = mSystemSlices.GetHead()->number;
	else
	{
		GuidoPos pos = mSystemSlices.GetHeadPosition();
		mSystemSlices.GetNext(pos);
		i1 = mSystemSlices.GetAt(pos)->number;
	}
	int i2 = mSystemSlices.GetTail()->number;
	myfname << "system" << i1-1 << "_" << i2 << ".txt";
	char *cname = myfname.str();
	cname[myfname.pcount()] = 0;
	ofstream myout(cname);
	myout << "force : " << force << " for extent " <<  extent << endl;
	mSpaceForceFunc->writeAllExtents(myout);
#endif

	// we call the StaffManager to tell him to take care of the still open tags of the lastslice, 
	// which staffmagr can now easily retrieve.
	// we do this here, so that the elements do not yet tell any position tags of there new position 
	// (which they will receive shortly). The problem is, that we have to have a valid
	// pointer to grsystem so the the sse of the positiontags can be set accordingly .....
	staffmgr->TakeCareOfBreakAt(this);

	// then we need to stretch the springs accordingly ....
	// how do I know, which springs are concerned?
	float curx = 0;
	if (startspring >= 0 && endspring < INT_MAX )
	{
		for( i = startspring; i <= endspring; ++i)
		{
			GRSpring * spr = mSpringVector->Get(i);
			if (spr)
			{
				spr->stretchWithForce (mSystemforce);
				spr->setGRPositionX (curx);
				curx += spr->x;
			}
		}
	}
	mBoundingBox.right = curx;				// this is the amount we have to go to the right ....

	// now I cut the spring-vector and give back the stuff that I do not longer need ....

	// now we have to deal with SYSTEM-Elements that are associated with staff-positions ...
	GuidoPos pos = First();
	while (pos)
	{
		GRNotationElement * el = GetNext(pos);
		GRBar * bar = dynamic_cast<GRBar *>(el);
		GRSystemTag * systag;
		if (bar)
		{
			int linesOffset = 0;
			bar->setPosFrom(0);
			if (lastStaff) {
				linesOffset += LSPACE / 2 * (lastStaff->getNumlines() - 5);
				bar->setPosTo( lastStaff->getPosition().y + lastStaff->getDredgeSize() - linesOffset);
			}
		}
		else if ((systag = dynamic_cast<GRSystemTag *>(el)) != 0 )
			systag->checkPosition(this);
	}
}

GRSystem::~GRSystem()
{
	mSystemSlices.RemoveAll();
	delete mStaffs;			mStaffs = 0;
	delete mSpringVector;	mSpringVector = 0;
	delete simplerods;
	delete complexrods;

#ifdef OLDSPFACTIVE
	delete spf;
#endif
	delete mSpaceForceFunc;
}

void GRSystem::GGSOutput() const
{
	ggsoffsetx += (long)mPosition.x;
	ggsoffsety += (long)mPosition.y;

	GRStaff * s;
	for (int i=mStaffs->GetMinimum();i<=mStaffs->GetMaximum();i++)
	{
		if ((s = mStaffs->Get(i)) == NULL)
			continue;

		gCurStaff = s;
		s->GGSOutput();
	}

	// now draw the (System)-elements
	GuidoPos elpos = First();
	while (elpos)
	{
		GetNext(elpos)->GGSOutput();
	}

	ggsoffsetx -= (long)mPosition.x;
	ggsoffsety -= (long)mPosition.y;
}

// --------------------------------------------------------------------------
void GRSystem::AddSystemSlice( GRSystemSlice * inSlice )
{
	traceMethod("AddSystemSlice");
	const TYPE_TIMEPOSITION & date = inSlice->getRelativeTimePosition();
	// const TYPE_DURATION & dur = inSlice->getDuration(); // durations are wrong

	// - (JB) the time position of the system is calculated here.
	// It takes the date of the first slice added to it.
	if( mSystemSlices.empty())
	{
		mDebugSystemDate = date;
	}
	mSystemSlices.push_back( inSlice );
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRSystem::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoSystem) {
		SendMap (f, getRelativeTimePosition(), getDuration(), kSystem, infos);
		return;		// done !
	}

	infos.fPos.x += mPosition.x;
	infos.fPos.y += mPosition.y;

	int staffCount = 0;
	if (mStaffs) {
		for( int i = mStaffs->GetMinimum(); i <= mStaffs->GetMaximum(); i++ )
		{
			GRStaff * staff = mStaffs->Get(i);
			if (staff) {
				staff->GetMap (sel, f, infos);
				staffCount++;
			}
		}
	}
	else if (mSystemSlices.size() > 0 )
	{
		GuidoPos pos = mSystemSlices.GetHeadPosition();
		while (pos) {
			GRSystemSlice * slice = mSystemSlices.GetNext(pos);
			slice->GetMap (sel, f, infos);
		}
	}
	if (( staffCount > 1 ) && ! mAccolade.empty())
	{
		for(std::vector<GRAccolade *>::const_iterator it = mAccolade.begin(); it < mAccolade.end(); it++)
		{
			(*it)->GetMap (sel, f, infos);
		}
	}

    GetSubElementsMap( sel, f, infos );
	infos.fPos.x -= mPosition.x;
	infos.fPos.y -= mPosition.y;
}


// --------------------------------------------------------------------------
/** \brief Draws system-slices, accolade, borders, and springs.
*/
void GRSystem::OnDraw( VGDevice & hdc ) const
{
	traceMethod("OnDraw");
	hdc.OffsetOrigin( mPosition.x, mPosition.y );  // (JB) sign change

	// - Find the positions of top and bottom systems.
	// Todo: the complete list of positions, to allow many accolades.
	GRStaff * theStaff = 0;
	NVPoint firstStaffPos ( -1, 0 );
	NVPoint lastStaffPos;
	int staffCount = 0;
	if (mStaffs)
	{
		for( int i = mStaffs->GetMinimum(); i <= mStaffs->GetMaximum(); i++ )
		{
			if ((theStaff = mStaffs->Get(i)) == NULL)
				continue;

			if (firstStaffPos.x == -1)
				firstStaffPos = theStaff->getPosition();

			lastStaffPos = theStaff->getPosition();
			gCurStaff = theStaff;

			theStaff->OnDraw(hdc);
			++ staffCount;
		}

	}		

	else if (mSystemSlices.size() > 0 )
	{
		// then we have to draw the systemslices ....
		GuidoPos pos = mSystemSlices.GetHeadPosition();
		bool firstFlag = true;
		
		while (pos)
		{
			GRSystemSlice * slice = mSystemSlices.GetNext(pos);
		//	if (mycount < 3 )
			if (firstFlag && slice)
			{
				firstFlag = false;
				firstStaffPos = slice->getPosition();
				int tmpmaxind = slice->mStaffs->GetMaximum();
				theStaff = slice->mStaffs->Get(tmpmaxind);
				if (theStaff)
				{
					lastStaffPos = theStaff->getPosition();
					staffCount = tmpmaxind;
				}
			}
			
			
			if( ! mAccolade.empty() )
			{
				for(std::vector<GRAccolade *>::const_iterator it = mAccolade.begin(); it < mAccolade.end(); it++)
				{
					int begin = (*it)->getBeginRange();
					int end = (*it)->getEndRange();
					if(begin == 0 || end < begin)
					{
						NVPoint top = slice->mStaffs->Get(slice->mStaffs->GetMinimum())->getPosition();
						NVPoint bottom = slice->mStaffs->Get(slice->mStaffs->GetMaximum())->getPosition();
						float staffHeight = (theStaff->getNumlines() - 1) * theStaff->getStaffLSPACE();
						bottom.y += staffHeight;
						(*it)->draw(hdc, top, bottom);
					}
					for(int i = slice->mStaffs->GetMinimum(); i <= slice->mStaffs->GetMaximum(); i++)
					{
						if (i == begin && slice->mStaffs->Get(end))
						{
							NVPoint endpos = NVPoint(slice->mStaffs->Get(end)->getPosition().x, slice->mStaffs->Get(end)->getPosition().y);
							float staffHeight = (theStaff->getNumlines() - 1) * theStaff->getStaffLSPACE();
							endpos.y += staffHeight;
							//no use of DrawAccolade anymore ?
							(*it)->draw(hdc, slice->mStaffs->Get(begin)->getPosition(), endpos);
						}
					}
				}
			}
			else if (staffCount > 1)
			{
				const float staffHeight = (theStaff->getNumlines() - 1) * theStaff->getStaffLSPACE();
				NVPoint endstaffPos = lastStaffPos;
				endstaffPos.y += staffHeight; // Set to the bottom of last staff
				GRAccolade * onlyAccol = new GRAccolade();
				onlyAccol->draw(hdc, firstStaffPos, endstaffPos);
			}
			
			std::map<int, bool> StavesOn;

			if(pos)
			{
				GRSystemSlice * nextSlice = mSystemSlices.GetAt(pos);
				if(nextSlice)
				{
					for(int i = nextSlice->mStaffs->GetMinimum(); i <= nextSlice->mStaffs->GetMaximum(); i++)
					{
						GRStaff * st = nextSlice->mStaffs->Get(i);
						StavesOn[i]=st->isStaffBeginOn();
					}
				}
				for(int i = slice->mStaffs->GetMinimum(); i <= slice->mStaffs->GetMaximum(); i++)
				{
					if(nextSlice->mStaffs->Get(i))
						slice->mStaffs->Get(i)->setNextOnOff(StavesOn[i]);
					else
					{
						slice->mStaffs->Get(i)->setNextOnOff(true);
					}
				}
			}
			else
			{
				for(int i = slice->mStaffs->GetMinimum(); i <= slice->mStaffs->GetMaximum(); i++)
				{
					slice->mStaffs->Get(i)->setNextOnOff(slice->mStaffs->Get(i)->isStaffEndOn());
				}
			}

			slice->OnDraw(hdc);
		}

	}

	// - Draws the vertical left border line.

	const float staffHeight = (theStaff->getNumlines() - 1) * theStaff->getStaffLSPACE();
	lastStaffPos.y += staffHeight; // Set to the bottom of last staff
	hdc.PushPenWidth( kLineThick );
	hdc.Line( firstStaffPos.x, firstStaffPos.y, lastStaffPos.x, lastStaffPos.y );
	hdc.PopPenWidth();

	// - Now draws the (System)-elements
    DrawSubElements( hdc );

	if( gGlobalSettings.gDisplaySprings == 1 )	DrawSystemSprings( hdc );
	if( gGlobalSettings.gDisplayForce == 1 )	DrawSystemForce( hdc );

	hdc.OffsetOrigin( -mPosition.x, -mPosition.y ); // Restore previous origin. (JB) sign change

	if (gBoundingBoxesMap & kSystemsBB)
		DrawBoundingBox( hdc, kSystemBBColor);
}

// --------------------------------------------------------------------------
/** \brief Scales and draws the accolade. Also draws the left staff borderline
*/
/*

// (CL) -> useless if everything is done in OnDraw ?

void GRSystem::DrawAccolade( VGDevice & hdc, const NVPoint & leftTop, const NVPoint & leftBottom, int id ) const
{
	// TODO: draw accolades
	// GRAccoladeList::const_iterator ptr;
	// for( ptr = mAccolades.begin(); ptr != mAccolades.end(); ++ptr )
	if( ! mAccolade.empty() )
	{
		for(std::vector<GRAccolade *>::const_iterator it = mAccolade.begin(); it < mAccolade.end(); it++)
		{
			if((*it)->getAccoladeID() == id)
				(*it)->draw(hdc, leftTop, leftBottom);
		}
	}
}
*/

// --------------------------------------------------------------------------
/** \brief Draws the springs used for the spacing.
*/
void GRSystem::DrawSystemSprings( VGDevice & hdc ) const
{
	const VGColor springColor = VGColor( 0, 0, 200, 50 );	// semi-transparent Blue
	const float springThickness = 5;
	hdc.PushPen ( springColor, springThickness );

	const int minspr = mSpringVector->GetMinimum();
	const int maxspr = mSpringVector->GetMaximum();
	for (int i = minspr; i <= maxspr; ++i )
	{
		GRSpring * spr = (GRSpring *)mSpringVector->Get(i);
		if (spr)
			spr->OnDraw(hdc);
	}
	hdc.PopPen();
}

// --------------------------------------------------------------------------
/** \brief Draws the force used for the spacing.
*/
void
GRSystem::DrawSystemForce( VGDevice & hdc ) const
{
	char tmpbuf[32];
	snprintf( tmpbuf,32,"%.2f", mSystemforce );

	const float xPos = (float)(mBoundingBox.right + 20);	// hardcoded
	const float yPos = 0;

	hdc.DrawString( xPos, yPos, tmpbuf, (int)strlen(tmpbuf) );
}

// --------------------------------------------------------------------------
void GRSystem::addStaff( GRStaff * newStaff, int num )
{
	traceMethod("addStaff");
	assert(newStaff);
	mStaffs->Set(num,newStaff);
}

// ----------------------------------------------------------------------------
void GRSystem::print() const
{
}

// ----------------------------------------------------------------------------
ARMusic * GRSystem::getARMusic() const
{
	return /*dynamic*/static_cast<ARMusic *>(getAbstractRepresentation());
}

// ----------------------------------------------------------------------------
void GRSystem::updateBoundingBox()
{
	traceMethod("updateBoundingBox");
/*
	actually, updateBoundingBox is never called,
	the bounding box is updated by GRSystem constructor
	Note also that the mapping is finalized by the setPosition and the FinishSystem methods
	
	
	// place for accolade
	GREvent::updateBoundingBox();

	GRNotationElement * e;
	int i;
	if (mStaffs)
	{
		for (i = mStaffs->GetMinimum(); i <= mStaffs->GetMaximum(); ++i)
		{
			e = mStaffs->Get(i);
			if (!e) continue;

			NVRect eltBox ( e->getBoundingBox());
			eltBox += e->getPosition();
			mBoundingBox.Merge( eltBox );
		}

		// here the staff udpate is done
		for (i = mStaffs->GetMinimum(); i <= mStaffs->GetMaximum(); ++i)
		{
			e = mStaffs->Get(i);
			if (!e) continue;
			e->getReferenceBoundingBox().right = - e->getPosition().x +	mBoundingBox.right;
		}
	}
	else
	{
	}
*/
}

// ----------------------------------------------------------------------------
void GRSystem::setMeterOfStaffs( int num, int denom )
{
}

// ----------------------------------------------------------------------------
/** \brief return the width of the system, in cm
*/
float GRSystem::getSystemWidthCm()
{
	float sizex = 0;
	if (mPage)
		sizex = mPage->getInnerWidthCm(); // to check.

	sizex -= mMarginLeft * kVirtualToCm;

	if (sizex < 0)
		sizex = 0;

	return sizex;
}

// ----------------------------------------------------------------------------
/** \brief Does the spacing.
*/
void GRSystem::dospacing()
{
	traceMethod("dospacing");
	if (mSpringVector)
	{
		// we can call Haken-Blostein now ...

		// now we need to tell new positions to
		// the graphical elements ...
		float posx = 0;
		int i;
		int pos1 = mSpringVector->GetMinimum();
		int pos2 = mSpringVector->GetMaximum();

		for ( i = pos1; i <= pos2; ++i )
		{
			GRSpring * spr = mSpringVector->Get(i);
			spr->setGRPositionX( posx );
			posx += spr->x;
		}

		// don't think we need this: the last element is just a spring where the
		// endglue is attached.
		// should have been told by the stretching already ..
		// but we need to update the bounding-rectangle!

		// we probably need to tell the last element
		// its position (for each staff ...)

		GRStaff * grstaff;

		for ( i = mStaffs->GetMinimum(); i <= mStaffs->GetMaximum(); i++ )
		{
			grstaff = mStaffs->Get(i);
			if (grstaff == 0) continue;

			// old:
			// grstaff->setLastElementPositionX(posx);
			// new:
			grstaff->updateBoundingBox();
		}
	}
}

// ----------------------------------------------------------------------------
/** \brief Works as follows: Text-Elements are added to the spring-list, so that they are
	told, when there position changes (after spring-strechting has been done)
*/
void GRSystem::addToSpring( GRNotationElement * el, int id)
{
	traceMethod("addToSpring");
	if (mSpringVector)
	{
		GRSpring * spr = mSpringVector->Get(id);
		if (spr)
		{
			spr->addElement(el,NULL);
		}
	}
}

// ----------------------------------------------------------------------------
/** \brief this sets the spring constants a new and redoes the spacing of the system
*/
void GRSystem::setSpringParameter( float nconst )
{
	traceMethod("setSpringParameter");
}

// ----------------------------------------------------------------------------
/** \brief Called by the GRPage finishPage.

	Propagates the finish procedure to slices and staves
*/
void GRSystem::FinishSystem()
{
	traceMethod("FinishSystem()");
	GuidoPos pos = mSystemSlices.GetHeadPosition();
    NVRect r;
	TYPE_DURATION duration;
	bool first = true;
	while (pos) {
        GRSystemSlice * slice = mSystemSlices.GetNext (pos);
		if (first) {
			setRelativeTimePosition (slice->getRelativeTimePosition());
			first = false;
		}
        slice->FinishSlice();
		r.Merge(slice->getBoundingBox());
		duration += slice->getDuration();
    }
	setDuration (duration);
	mBoundingBox.Merge (r);
	// bounding box and position should now be ready
	mMapping = mBoundingBox;				// set the mapping box
	mMapping += mPosition + getOffset()	;	// and adjust position	
}

// ----------------------------------------------------------------------------
/** \brief Called by the StaffManager when the System is finished.

	In this case, we have to do the final stretching and also set the positions ...
	updateBoundingBox should also be called ...
*/
void GRSystem::FinishSystem( ISpringVector * pvect, IRodList * prods1,  IRodList * prods2,
#ifdef OLDSPFACTIVE
		GRSpaceForceFunction * pspf,
#endif
		GRSpaceForceFunction2 * psff, const TYPE_TIMEPOSITION &tp, int lastline)
{
	traceMethod("FinishSystem");
}

// ----------------------------------------------------------------------------
/** \brief Returns the number corresponding to input staff.
*/
int GRSystem::getStaffNumber( const GRStaff * staff ) const
{
	if (mStaffs)
	{
		for (int i = mStaffs->GetMinimum(); i <= mStaffs->GetMaximum(); ++i)
		{
			if (mStaffs->Get(i) == staff)
				return i;
		}
	}
	else
	{
		GuidoPos pos = mSystemSlices.GetHeadPosition();
		int ret = -1;
		while (pos)
		{
			GRSystemSlice * slice = mSystemSlices.GetNext(pos);
			if (slice)
			{
				ret = slice->getStaffNumber(staff);
				if (ret != -1)
					return ret;
			}
		}
	}
	return -1;
}

// ----------------------------------------------------------------------------
const GRSpring * GRSystem::getGRSpring( int id ) const
{
	return mSpringVector ? mSpringVector->Get(id) : 0;
}

// ----------------------------------------------------------------------------
/** \brief Sets the system-format
*/
void GRSystem::setSystemFormat(ARSystemFormat * sysfrm)
{
	traceMethod("setSystemFormat");
	if( sysfrm == 0 ) return;
	// what do we do with systemFormat ....

	// the staves-string has to be evaluated ...
	// (not yet)
	// and the dx has to be done ....
	if (sysfrm->getDX() && sysfrm->getDX()->TagIsSet())
	{
		const float dx = sysfrm->getDX()->getValue();
		mMarginLeft = dx;
		setPosition( mPosition );
	}
}

// ----------------------------------------------------------------------------
void GRSystem::setPosition( const NVPoint & pnt )
{
	traceMethod("setPosition");
	mPosition = pnt;
	mPosition.x += mMarginLeft;
}

// ----------------------------------------------------------------------------
void GRSystem::addBar(GRBar * mybar, GRSystem::BARTYPE btype, GRStaff * inStaff )
{
	traceMethod("addBar");
	// We have to build a new Barline ...
	// the barline needs to know that it belongs to a system.
	// when the distance of staffs is being set, these tags must be updated (for length)
	ARBar * arbar = mybar->getARBar();
	assert(arbar);
	GRBar * newbar = new GRBar( arbar, this, inStaff, mybar->getRelativeTimePosition());

	// I must attach the newbar with the other spring (or at least be told when something happens ...)
	mybar->addAssociation(newbar);

	// this needs to be rethought later .
	// Just for testing :
	// newbar->setPosFrom(2.5*inStaff->getStaffLSPACE());
	// newbar->setPosTo(20*inStaff->getStaffLSPACE());
	GetCompositeElements().push_back(newbar);
}

// ----------------------------------------------------------------------------
/** \brief Adds a Tag to the system it is called by GRBeam to make sure that
	 the beam-position is updated correctly for system-beams
*/
void GRSystem::addSystemTag( GRNotationElement * mytag )
{
	traceMethod("addSystemTag");
	GetCompositeElements().push_back(mytag);
}

// ----------------------------------------------------------------------------
GRSpring * GRSystem::getSpring(int id) const
{
	return mSpringVector ? mSpringVector->Get(id) : 0;
}

// ----------------------------------------------------------------------------
GRSystemSlice * GRSystem::getFirstGRSystemSlice()
{
	if (mSystemSlices.size() > 0)
		return mSystemSlices.front(); // GetHead()

	return 0;
}

// ----------------------------------------------------------------------------
// (JB) Because a system can have several accolades of different types, we
// should copy and store all received accolade tags.
//
// For this pre-implementation we only keep one accolade type.
//
// In the future, several accolades will be displayed, taking account of
// their respective types and ranges.
//
// -> (CL) done in 2013 :  several accolades can be displayed, 
// GRAccolade mAccolade is now replaced by a std::vector<GRAccolade *>, 
// as well as mCurAccolade Tag in GRStaffManager (std::vector<ARAccol *>)

void GRSystem::notifyAccoladeTag( ARAccol * inAccoladeTag )
{
	traceMethod("notifyAccoladeTag");
	int accolType = GRAccolade::kAccoladeCurly;

	// - Get the type
	const TagParameterString * paramType = inAccoladeTag->getType();
	if( paramType && paramType->TagIsSet())
	{
		if(( *paramType == "straightBrace" ) || (*paramType == "standard" ))
		{
			accolType = GRAccolade::kAccoladeStraight;
		}
		else if( *paramType == "curlyBrace" )
		{
			accolType = GRAccolade::kAccoladeCurly;
		}
		else if( *paramType == "thinBrace" )
		{
			accolType = GRAccolade::kAccoladeThin;
		}
		else if( *paramType == "none" )
		{
			accolType = GRAccolade::kAccoladeNone;
		}
	}

	// - Get the x-offest
	const TagParameterFloat * paramDx = inAccoladeTag->getDX();
	float dx = 0;
	if( paramDx && paramDx->TagIsSet())
		dx = paramDx->getValue();

	int beginRange = 0;
	int endRange = 0;

	const TagParameterString * range = inAccoladeTag->getAccolRange();
	std::string stringRange;
	if( range && range->TagIsSet())
	{
		stringRange = range->getValue();
	

		std::size_t begin = 0;
		std::size_t dashPos = stringRange.find("-", begin);
	
		if(dashPos != -1)
		{
			std::string beginString = stringRange.substr(begin, dashPos);
			std::stringstream stream(beginString);
			stream >> beginRange;

			std::string endString = stringRange.substr(dashPos+1);
			std::stringstream stream2(endString);
			stream2 >> endRange;
		}
		else
		{
			std::string beginString = stringRange.substr(begin);
			std::stringstream stream(beginString);
			stream >> beginRange;

			endRange = beginRange;
		}
	}
	
	int accolID = 0;
	const TagParameterInt * IDint = inAccoladeTag->getIDInt();
	if( IDint && IDint->TagIsSet())
		accolID = IDint->getValue();

	GRAccolade * accolade = new GRAccolade;

	accolade->setAccoladeType( accolType );
	accolade->setDx( dx );
	accolade->setAccoladeID(accolID);
	accolade->setBeginRange(beginRange);
	accolade->setEndRange(endRange);

	mAccolade.push_back(accolade);

}


