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

#include "ARSpace.h"
#include "ARMusicalVoice.h"

#include "GRVoice.h"
#include "GRSystem.h"
#include "GRRod.h"
#include "GRSpace.h"
#include "GRStaffManager.h"
#include "GRSpringForceIndex.h"
#include "GRSpacingMatrix.h"
#include "GRClef.h"
#include "GRStaff.h"
#include "kf_list.h"

// ----------------------------------------------------------------------------
GRVoice::GRVoice(ARMusicalObject * arobj, bool ownsar)
			: GRARCompositeNotationElement(arobj,ownsar)
{
	GetCompositeElements().setOwnership(0);

	mSysNodeList = new SysNodeList(1); // owns elements!

	lastrod = NULL;
	firstrod = NULL;
	lastsprpos = NULL;

	// yes, we are looking for newlines 
	// (the first entry is as if there was one!)
	firstPositionInLine = NULL;
	mIsNewLine = true;
}

GRVoice::~GRVoice()
{
	// there is an owns-elements on the list,
	// so that all the elements get deleted.
	delete mSysNodeList;
	
	mSysNodeList = 0;
}


/** \brief Indicates, that a new system has started.
 	this must be reflected by a new entry into the sysvect.
*/
void GRVoice::startNewSystem(GRSystem * in)
{
	SystemNode * sysnode = new SystemNode();
	sysnode->grsystem = in;
	sysnode->pos = Last();
	mSysNodeList->AddTail(sysnode);
}

/** \brief Called from GRMusic to tell a voice
	to prepare for a NLinePosition at the given Time.
*/
void GRVoice::setPossibleNLinePosition(const TYPE_TIMEPOSITION & tp)
{
	nlinepos = Last();
	nlinetp  = tp;
}

/** \brief Remember a previous nline-position
*/
void GRVoice::rememberNLinePosition(const TYPE_TIMEPOSITION & tp)
{
	assert(tp == nlinetp);

	GuidoPos pos = Last();
	while (pos && pos != nlinepos)
	{
		GetCompositeElements().RemoveTail();
		pos = Last();
	}
}

/** \brief Returns the start-position of the given system in the Element-List
*/
GuidoPos GRVoice::getSystemStartPos(GRSystem * in)
{
	GuidoPos pos = mSysNodeList->GetHeadPosition();
	while (pos)
	{
		SystemNode * sysnode = mSysNodeList->GetNext(pos);
		if (sysnode->grsystem == in)
		{
			// it is the FOLLOWING position!
			GuidoPos syspos = sysnode->pos;

			// if syspos == NULL it was called
			// at the beginning ...
			if (syspos == NULL)
			{
				syspos = First();
			}
			else
				GetNext(syspos);
			return syspos;
		}
	}
	return (GuidoPos) 0;
}

ARMusicalVoice * GRVoice::getARMusicalVoice() 
{
	return static_cast/*dynamic cast*/<ARMusicalVoice *>(mAbstractRepresentation);
}

#ifdef OLDSPFACTIVE
/** \brief cthis routine is called just as createNewRods is called
	It updates the QP-Matrix for minimizing the Spring-
	energy. This is a first prototype ...
*/
int GRVoice::updateQPMatrix(GRSpaceForceFunction * spf,
							ISpringVector * sprvect,
							int startspr, int endspr)
{
	double * mat = spf->getMatrix();
	int n = spf->getMatrixMemSize();

	int firstrun = 1;

	// this setting just remembers, if it is the first run ....
	// for the first run, there must
	// be a special handling (remembering the first rod)
//	int beginsAtHead = 0;
	GuidoPos pos;
	if (lastsprpos == NULL)
	{
		// there has been no previous rod, that
		// is there has been no element in this
		// voice yet, that is attached to a string.
		if (mIsNewLine)
		{
			// then, there has been no element 
			// added yet!
			return -1;
		}
		pos = firstPositionInLine;
//		beginsAtHead = 1;
	}
	else
	{
		// iterate to the next element....
		pos = lastsprpos;
		elements->GetNext(pos);
	}

	GRNotationElement * el = NULL; 
	float spacedistance = 0;
	bool spaceactive = false;

	while (pos)
	{

		el = elements->GetNext(pos);


		// is this element a Space-Tag?
		GRSpace * spc = dynamic_cast<GRSpace *>(el);
		if (spc)
		{
			spacedistance += spc->getARSpace()->getValue();
			spaceactive = true;
		}

		// does this element have a spring;
		// if not, than there can't be a rod attached to it.
		if (!el->getNeedsSpring())
		{
			continue;
		}

		// if (beginsAtHead)
		// {

		int start; // the spring where the current element starts
		int end; // the spring where the current element ends


		// this is done, as long as the NEXT
		// element needs a spring ...
		while (pos)
		{
			GRNotationElement * next = elements->GetAt(pos);

			GRSpace * spc = dynamic_cast<GRSpace *>(next);
			if (spc)
			{
				// then this element is a space-tag ...
				// all between is added ...
				spacedistance += spc->getARSpace()->getValue();
				spaceactive = true;
			}
			if (!next->getNeedsSpring())
			{
				elements->GetNext(pos);
				continue;
			}

			start = el->getSpringID();
			end = next->getSpringID();

			// then we need the "real" constant:

			float mysconst = GRSpring::onlycalcconst(el);

				// complex-rod ...
				int i,j;
				for (i=start;i<end;i++)
				{	
//					*(mat+i + n*i) += 0.5*mysconst;

					/* This version is one, which works
					   only with added constraints
					   (equality of subgroups in one voice
					   not yet clear, how to to this automatically
					   */
					for (j=i;j<end;j++)
					{
						*(mat + i + n*j) += 0.5*mysconst;

					}
				} 

			// it needs a spring. Now we add a Rod ...
			if (spaceactive)
			{

				spf->addConstraint(start,end,spacedistance);

				//rod = new GRRod(el,next,0,spacedistance);
				//rod->setIsSpaceRod(true);
				spaceactive = false;
				spacedistance = 0;
				// spaceactive!
				// stfmgr->addRod(rod,1);
			}
			else
			{
				double length = el->getRightSpace() + next->getLeftSpace();
				spf->addConstraint(start,end,length);
				// rod = new GRRod(el,next,0);
				// no space active
				// stfmgr->addRod(rod,0);
			}

			// finish the inner cycle
			break;

		} // while (pos);

		// if we are moved out 

	} // while (pos);

	// there is an element ...
	// then we need to add a Rod from this 
	// to the end. This rod is going to
	// be the lastrod ...
	if (el && el->getNeedsSpring())
	{
		int start;
		int end;

		start = el->getSpringID();
		end = endspr;

			float mysconst = GRSpring::onlycalcconst(el);

				// complex-rod ...
				int i,j;
				for (i=start;i<end;i++)
				{	
//					*(mat + i + n*i) += mysconst;
					for (j=start; j<end; j++)
					{
						*(mat+ i + n*j)+= 0.5 * mysconst;
					}
				}
			// }

		if (spaceactive)
		{
			spf->addConstraint(start,end,spacedistance);
			// the rod is of the length of 
			// the following space tag...
			//rod = new GRRod(spacedistance,
			//	el->getSpringID(),endspr);
			//rod->setIsSpaceRod(true);

			// space active!
			//stfmgr->addRod(rod,true);
		}
		else
		{
				double length = el->getRightSpace();
				spf->addConstraint(start,end,length);
			//rod = new GRRod(el->getRightSpace(),
			//	el->getSpringID(),endspr);
			// no space active
			//stfmgr->addRod(rod,0);
		}

	}
	return 0;
}

/** \brief Called just as createNewRods is called.

	It updates the QP-Matrix for minimizing the Spring-
	 energy. This is a first prototype.
*/
int GRVoice::updateMSCMatrix( GRSpaceForceFunction * spf,
							int startspr,int endspr)
{
	double * mat = spf->getMSCMatrix();
	int n = spf->getMSCMatrixMemSize();
	
	// this setting just remembers,
	// if it is the first run ....
	// for the first run, there must
	// be a special handling (remembering
	// the first rod)
	//	int beginsAtHead = 0;
	GuidoPos pos;
	if (lastsprpos == NULL)
	{
		// there has been no previous rod, that
		// is there has been no element in this
		// voice yet, that is attached to a
		// string.
		if (mIsNewLine)
		{
			// then, there has been no element 
			// added yet!
			return -1;
		}
		pos = firstPositionInLine;
		//		beginsAtHead = 1;
	}
	else
	{
		// iterate to the next element....
		pos = lastsprpos;
		elements->GetNext(pos);
	}
	
	GRNotationElement * el = NULL; 

	while (pos)
	{
		el = elements->GetNext(pos);

		// does this element have a spring;
		// if not, than there can't be a rod attached to it.
		if (!el->getNeedsSpring())
		{
			continue;
		}
		
		// if (beginsAtHead)
		// {
		
		int start; // the spring where the current element starts
		int end; // the spring where the current element ends
		
		// this is done, as long as the NEXT element needs a spring ...
		while (pos)
		{
			GRNotationElement * next = elements->GetAt(pos);
			if (!next->getNeedsSpring())
			{
				elements->GetNext(pos);
				continue;
			}
			
			start = el->getSpringID();
			end = next->getSpringID();
			
			// then we need the "real" constant:
			
			//float mysconst = GRSpring::onlycalcconst(el);
			
			double value = 0; // mysconst;
			
			const TYPE_DURATION & durel (el->getDuration());
			
			if (durel > DURATION_0)
			{
				value = 1.0 / (float) durel;
			}

			int i;
			for (i=start;i<end;i++)
			{
				if (value > *(mat + i + n*0))
				{
					*(mat + i + n*0) = value;
					*(mat + i + n*1) = durel.getNumerator();
					*(mat + i + n*2) = durel.getDenominator();
				}
			}
			
			// finish the inner cycle
			break;
			
		} // while (pos);
		
		// if we are moved out 
		
		
	} // while (pos);
	
	// there is an element ...
	// then we need to add a Rod from this 
	// to the end. This rod is going to
	// be the lastrod ...
	if (el && el->getNeedsSpring())
	{
		int start;
		int end;
		
		start = el->getSpringID();
		end = endspr;
		
		float mysconst = GRSpring::onlycalcconst(el);
		
		double value = mysconst;
		
		const TYPE_DURATION & durel (el->getDuration());
		if (durel > DURATION_0)
		{
			value = (1.0 / value) / durel;
		}
		int i;
		for (i=start;i<end;i++)
		{	
			if (value > *(mat + i + n*0))
				{
					*(mat + i + n*0) = value;
					*(mat + i + n*1) = durel.getNumerator();
					*(mat + i + n*2) = durel.getDenominator();
				}
			
		}
	}

	return 0;
}

#endif // ifdef OLDSPFACTIVE


/** \brief Called just as createNewRods is called
	It updates the QP-Matrix for minimizing the Spring-
	energy. This is a first prototype.
*/
int GRVoice::updateMSCMatrix(GRSpaceForceFunction2 * sff,
							 GRSpacingMatrix * spm,
							 GRStaffManager * stfmgr,
							 ISpringVector * sprvect,
							int startspr, int endspr )
{
	double * mat = spm->getMSCMatrix();
	int n = spm->getMSCMatrixMemSize();

	// now even newer stuff; we distinguish between different
	// phases:
	// first voice just sets the maximum value for each spring
	// (this is the regular gourlay model)
	// we also take care of "neighbourhoods", telling the
	// SpacingMatrix, where same-notedurations occur.

	// this setting just remembers if it is the first run ....
	// for the first run, there must be a special handling (remembering
	// the first rod)
	//	int beginsAtHead = 0;
	GuidoPos pos;
	if (lastsprpos == NULL)
	{
		// there has been no previous rod, that
		// is there has been no element in this
		// voice yet, that is attached to a
		// string.
		if (mIsNewLine)
		{
			// then, there has been no element 
			// added yet!
			return -1;
		}
		pos = firstPositionInLine;
		//		beginsAtHead = 1;
	}
	else
	{
		// iterate to the next element....
		pos = lastsprpos;
		GetNext(pos);
	}
	
	GRNotationElement * el = NULL; 

	double prevvalue = 0;
	int   prevstartid = -1;
//	int   numneighbours = 0;

	KF_List<int> * neighbourlist = 0;
	while (pos)
	{
		el = GetNext(pos);
			
		// does this element have a spring;
		// if not, than there can't be a rod attached to it.
		if (!el->getNeedsSpring())
			continue;
		
		int start; // the spring where the current element starts
		int end; // the spring where the current element ends
		
		// this is done, as long as the NEXT element needs a spring ...
		while (pos)
		{
			GRNotationElement * next = GetCompositeElements().GetAt( pos );
			
			if (!next->getNeedsSpring())
			{
				GetNext(pos);
				continue;
			}
			
			start = el->getSpringID();

			end = next->getSpringID();

			// now we just determine the necessary value, depending
			// on duration alone ...
			float value = 0; 
			
			const TYPE_DURATION durel (el->getDuration());
//			const TYPE_DURATION &durnext = next->getDuration();
			
			if (durel > DURATION_0)
			{
				value = 1.0f / (float) durel;
				
				// now we fill the second entry in the msc-matrix
				// this is needed, so that we can make sure, that
				// spacing is somewhat controlled, when NO groups
				// have been found.
				
				// this is equivalent to the stuff that was calculated
				// before (see below)
				int cnt;
				for (cnt=start;cnt<end;cnt++)
				{	
					if (value > *(mat + cnt + n*3))
					{
						*(mat + cnt + n*3) = value;
					} 
				}
				
				if (prevvalue == value)
				{
					if (!neighbourlist)
					{
						neighbourlist = new KF_List<int>();
						neighbourlist->AddTail(prevstartid);
					}
					// then we have a neighboor 
					neighbourlist->AddTail(start);
				}
				else
				{
					if (neighbourlist)
					{
						neighbourlist->AddTail(start);
						spm->AddNeighbourList(neighbourlist);
						neighbourlist = NULL;
					}
					// then we just start a new group ...
				}
			}
			else
			{
				if (neighbourlist)
				{
					neighbourlist->AddTail(start);
					spm->AddNeighbourList(neighbourlist);
					neighbourlist = NULL;
				}
			}				
			
			// this means we are finished with finding the next event
			el = NULL;
			if (durel <= DURATION_4)
				prevvalue = value;
			else 
				prevvalue = -1.0f;

			prevstartid = start;
			
			break;
		}
	}

	if (el && el->getNeedsSpring())
	{
		int start;
		int end;
		
		start = el->getSpringID();
		end = endspr;
		
		//float mysconst = GRSpring::onlycalcconst(el);
		
		float value = 0; // mysconst;
		
		const TYPE_DURATION durel (el->getDuration());
		if (durel > DURATION_0)
		{
			value = 1.0f / (float) durel;
			
			int i;
			for (i=start;i<end;i++)
			{	
				if (value > *(mat + i + n*3))
				{
					*(mat + i + n*3) = value;
				}
			}
			// check for neighbours ....

			// maybe this is a hack, but one that makes sense:
			// neighbourhood is only important for durations of up to
			// a quarter note.

				if (prevvalue == value)
				{
					if (!neighbourlist)
					{
						neighbourlist = new KF_List<int>();
						neighbourlist->AddTail(prevstartid);
					}
					// then we have a neighboor 
					neighbourlist->AddTail(start);
					neighbourlist->AddTail(endspr);
					
					spm->AddNeighbourList(neighbourlist);
					neighbourlist = NULL;
				}
		}
		if (neighbourlist)
		{
			// the last element was not a neighbour 
			neighbourlist->AddTail(start);

			spm->AddNeighbourList(neighbourlist);
			neighbourlist = NULL;
		}
	}

	// if i have a neighbourlist that is still open, something 
	// went a little wrong?
	if (neighbourlist)
	{
		assert(false);
		neighbourlist->AddTail(endspr-1);
		spm->AddNeighbourList(neighbourlist);
		neighbourlist = NULL;
	}
	return 0;
}

/** \brief Creates all rods from the startspr until the endspr. 

It is called by the StaffManager (BuildSPF)
we have to make sure that firstrod and
lastrod is created correctly.
We do not need to take care of rods with
starting positions that are smaller than startspr.
optForce is the guido layout settings
*/
int GRVoice::createNewRods(GRStaffManager * stfmgr, int & startspr, int & endspr, float optForce)
{
	if (lastrod) {
		// so the calling function knows where the restretching must start.
		assert(lastrod->mSpr2 == startspr);
		// this removes the rod from the rod-list.
		// it is done, because the lastrod was a connection to the end, which is now moved further....
		lastrod = NULL;
	}
	firstrod = NULL;

	// this setting just remembers, if it is the first run ....
	// for the first run, there must be a special handling (remembering the first rod position)
	GuidoPos pos;
	if (lastsprpos == NULL) {
		// there has been no previous rod, that is there has been no element in this
		// voice yet, that is attached to a string.
		if (mIsNewLine)
		{
			// then, there has been no element added yet!
			return -1;
		}
		pos = firstPositionInLine;
	}
	else {
		// iterate to the next element....
		pos = lastsprpos;
		GetNext(pos);
	}

	GRNotationElement * el = 0; 
	float spacedistance = 0;
	bool spaceactive = false;
	while (pos)
	{
		lastsprpos = pos;
		el = GetNext( pos );

		// is this element a Space-Tag?
		GRSpace * spc = dynamic_cast<GRSpace *>(el);
		if (spc) {
			// this can only happen, if we are at the very start ..
			assert(!firstrod);
			spacedistance += spc->getARSpace()->getValue();
			spaceactive = true;
		}

		// does this element have a spring;
		// if not, than there can't be a rod attached to it.
		if (!el->getNeedsSpring()) continue;

		if (!firstrod) {
			GRRod * rod;
			// this may need to change, so that glue- elements get a chance as  well ...
			// maybe we have to create another rod somewhere ?
			// not only the glue, but this MUST be the distance of the last element ....
			// otherwise, we do not get enough space ...
			float dist = (LSPACE * 0.5f);
			if (dynamic_cast<GRClef*>(el))
				dist = 50; // the same as the begin glue, this is a wild hack ...

			rod = new GRRod (dist + el->getLeftSpace(), el->getSpringID()-1, el->getSpringID(), optForce);
			if (spaceactive) {
				rod->setLength (spacedistance);
				rod->setIsSpaceRod( true );		// space-active....
				spaceactive = 0;
				spacedistance = 0;
				stfmgr->addRod(rod,1);
			}
			else stfmgr->addRod(rod,0);

			if (rod->getSpr1() < startspr) startspr = rod->getSpr1();

			// the very first rod is a copy of the one added to the stfmgr.
			firstrod = rod;
		}


		// this is done, as long as the NEXT element needs a spring ...
		while (pos) {
			GRNotationElement * next = GetCompositeElements().GetAt(pos);
			GRSpace * spc = dynamic_cast<GRSpace *>(next);
			if (spc) {
				// then this element is a space-tag ... all between is added ...
				spacedistance += spc->getARSpace()->getValue();
				spaceactive = true;
			}
			if (!next->getNeedsSpring()) {
				GetNext(pos);
				continue;
			}
			// it needs a spring. Now we add a Rod ...
			GRRod * rod = 0;
			if (spaceactive) {
				rod = new GRRod( el, next, 0, spacedistance, optForce);
				rod->setIsSpaceRod( true );
				spaceactive = false;
				spacedistance = 0;
				stfmgr->addRod( rod,1 );	// spaceactive!
			}
			else {
				rod = new GRRod( el, next, 0, optForce);
				stfmgr->addRod(rod,0); 		// no space active
			}

			if( rod->getSpr1() < startspr )
				startspr = rod->getSpr1();

			lastrod = rod;
			// finish the inner cycle
			break;
		} // while (pos);
	} // while (pos);

	// there is an element ...
	// then we need to add a Rod from this to the end. This rod is going to be the lastrod ...
	if (el && el->getNeedsSpring()) {
		GRRod * rod;
		if (spaceactive) {
			// the rod is of the length of the following space tag...
			rod = new GRRod( spacedistance, el->getSpringID(), endspr, optForce);
			rod->setIsSpaceRod( true );
			stfmgr->addRod(rod,1);		// space active!
		}
		else {
			rod = new GRRod(el->getRightSpace(), el->getSpringID(), endspr, optForce);
			stfmgr->addRod(rod,0);		// no space active
		}
		if (rod->getSpr1() < startspr)
			startspr = rod->getSpr1();
		lastrod = rod;
	}
	return 0;
}

GuidoPos GRVoice::AddTail(GRNotationElement * el)
{
	// this waits for elements to add to the 
	// voice. If we are on the lookout for
	// newlines (to remember the position for
	// the first springposition) the
	// position is set.
	GuidoPos pos = GRARCompositeNotationElement::AddTail(el);
	if (mIsNewLine)
	{
		firstPositionInLine = pos;
		mIsNewLine = false;
	}

	return pos;
}

GRPage * GRVoice::getPageNum(int num,int denom)
{
	// I have to travers the voice and find the element
	// at the position num/denom. Then I have to find the 
	// pagenumber of that element.

	const TYPE_TIMEPOSITION tpsearch ( num,denom );
	GuidoPos pos = First();
	while (pos)
	{
		GRNotationElement * el = GetNext(pos);
		GREvent * ev = GREvent::cast(el);
		if (ev)
		{
			if (ev->getRelativeTimePosition() == tpsearch)
			{
				GRStaff * stf = ev->getGRStaff();
				GRSystem * sys = stf->getGRSystem();
				GRPage * page = sys->getGRPage();
				return page;
			}
			else if (ev->getRelativeTimePosition() > tpsearch)
				return NULL;
		}
	}
	return NULL;
}

/** \brief Returns a page corresponding to a time position.
	
	It finds the musical element which time position is immediately before or equal to 
	the input time position. Then it returns the page of that musical element.

	getPageForTimePos() always try to return a page, while getPageNum() only returns
	a page when the input time position match the time position of an existing 
	musical element.

	TODO: Check if the page keeps track of both its start timepos and end timepos,
			therefore we could directly jump from one page to another


	\bug The last system of a page point to the next page instead of its parent page.
	
*/ 
GRPage * GRVoice::getPageForTimePos( int num, int denom ) const
{
	GRPage * outPage = 0;
	TYPE_TIMEPOSITION inTime ( num, denom );
	GuidoPos pos = First();
	
	// - Find the event which best match the input date
	GREvent * bestEv = 0;
	while (pos)
	{
		GRNotationElement * el = GetNext(pos);
		GREvent * ev = GREvent::cast(el);
		if( ev )
		{
			const TYPE_TIMEPOSITION & currTime = ev->getRelativeTimePosition();
			if( currTime <= inTime )
				bestEv = ev;
			else			
				break;	// (JB) I suppose that all next elements have a greater time pos. 
		}
	}
	
	// - Get the page corresponding to the event found

	if( bestEv )
	{				
		GRStaff * staff = bestEv->getGRStaff();
		GRSystem * sys = staff->getGRSystem();
		outPage = sys->getGRPage();
	}
	
	return outPage;
}
