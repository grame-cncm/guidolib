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


#include "GRPossibleBreakState.h"
#include "GRVoiceManager.h"
#include "GRStaffManager.h"
#include "GRVoice.h"
#include "GRPositionTag.h"
#include "GRSystem.h"
#include "kf_ivect.h"


// ============================================================================
GRPossibleBreakState::GRStaffAndState::~GRStaffAndState()
{
	delete lastrod;
	delete firstrod;
}

void 
GRPossibleBreakState::GRStaffAndState::setLastRod(GRRod * plastrod)
{
	if (plastrod)
		lastrod = new GRRod(*plastrod);
}

void 
GRPossibleBreakState::GRStaffAndState::setFirstRod(GRRod * pfirstrod)
{
	if (pfirstrod)
		firstrod = new GRRod(*pfirstrod);
}


// ============================================================================
GRPossibleBreakState::GRVoiceTagsAndStaff::~GRVoiceTagsAndStaff()
{
	delete grtags;
	delete lastrod;
	delete firstrod;
}

void 
GRPossibleBreakState::GRVoiceTagsAndStaff::setLastRod(GRRod * plastrod)
{
	delete lastrod;
	lastrod = 0;
	if (plastrod)
		lastrod = new GRRod(*plastrod);
}
void 
GRPossibleBreakState::GRVoiceTagsAndStaff::setFirstRod(GRRod * pfirstrod)
{
	
	delete firstrod;
	firstrod = 0;
	if (pfirstrod)
		firstrod = new GRRod(*pfirstrod);

}

// ============================================================================

GRPossibleBreakState::GRPossibleBreakState()
{
	// owns elements!
	ssvect = new KF_IVector<GRStaffAndState>(1);
	vtsvect = new KF_IVector<GRVoiceTagsAndStaff>(1);
#ifdef OLDSPFACTIVE
	spf = NULL;
	copyofcompletespf = NULL;
#endif
	sff = NULL;
	copyofcompletesff = NULL;
	lastrod = NULL;
	firstrod = NULL;
}

GRPossibleBreakState::~GRPossibleBreakState()
{
	delete ssvect;
	delete vtsvect;

#ifdef OLDSPFACTIVE
	delete spf;
	delete copyofcompletespf;
#endif

	delete sff;
	delete copyofcompletesff;
}

void GRPossibleBreakState::SaveState(KF_IVector<GRStaff> * vstaffs,
		KF_IVector<GRVoiceManager> * vvcemgrs,
		GRStaffManager * staffmgr,
		const TYPE_TIMEPOSITION & curtp,
		float parforce,
		float breakval)
{
	// this saves the staff-stuff
	int i;
	for (i=vstaffs->GetMinimum();i<=vstaffs->GetMaximum();i++)
	{
		GRStaff * staff = vstaffs->Get(i);
		if (staff)
		{
			GRStaffAndState * sas = new GRStaffAndState;
			sas->pstaff = staff;
			sas->setLastRod(staff->lastrod);
			sas->setFirstRod(staff->firstrod);
			sas->tpos = staff->mCompElements.GetTailPosition();
			sas->staffstate = staff->getGRStaffState();
			ssvect->Set(i,sas);
		}
	}

	// now the voice-mgr-stuff ...
	// (grtags ...)
	for (i = vvcemgrs->GetMinimum();i<=vvcemgrs->GetMaximum();i++)
	{
		GRVoiceManager * vcmgr = vvcemgrs->Get(i);
		if (vcmgr)
		{
			GRVoiceTagsAndStaff * vts = new GRVoiceTagsAndStaff(vcmgr);
			GRVoice * v = vcmgr->getGRVoice();
			vts->SaveGRTags(vcmgr->grtags);
			vts->setLastRod(v ? v->getLastRod() : 0);
			vts->setFirstRod(v ? v->getFirstRod() : 0);
			vts->pstaff = vcmgr->mCurGrStaff;
			vts->staffnum = vcmgr->staffnum;
			vtsvect->Set(i,vts);
		}
	}

	// now for the staffmgr (springID, positions
	// for rod-lists ...)
	springID = staffmgr->mSpringID;
	simplerodspos = staffmgr->mSimpleRods->GetTailPosition();
	complexrodspos = staffmgr->mComplexRods->GetTailPosition();


	// and now for the system ... the position of 
	// the last system-element ....
	
	// systemelementendpos = staffmgr->grsystem->GetTailPosition();

	// the timeposition
	tp = curtp;

	force = parforce;

	pbreakval = breakval;

	lastrod = staffmgr->lastrod;
	firstrod = staffmgr->firstrod;
}


void GRPossibleBreakState::GRVoiceTagsAndStaff::SaveGRTags(KF_IPointerList<GRTag> *input)
{
	// the list owns the GRTagAndPosition
	grtags = new KF_IPointerList<GRTagAndPosition>(1);

	GuidoPos pos = input->GetHeadPosition();
	while (pos)
	{
		GRTag * grtag = input->GetNext(pos);
		GRTagAndPosition * grtp = new GRTagAndPosition;
		grtp->grtag = grtag;
		GRNotationElement * el = dynamic_cast<GRNotationElement *>(grtag);
		if (el)
		{
			if (el->getAssociations())
				grtp->assocpos = el->getAssociations()->GetTailPosition();
			else
				grtp->assocpos = NULL;
		}
		else
			grtp->assocpos = NULL;
		grtags->AddTail(grtp);
	}

}

/** \brief Called, to signal a break at this saved position.
	It calls the respective functions for all saved grtags (including
 	the assocpos)
*/
void GRPossibleBreakState::GRVoiceTagsAndStaff::EndAtBreak()
{
	GuidoPos pos = grtags->GetHeadPosition();
	while (pos)
	{
		GRTagAndPosition * tp = grtags->GetNext(pos);
		// increments tp->assocpos!
		// (so that it can be used
		//  in BeginAfterBreak to correctly
		// set the assocpos).
		assert(tp);
		GRPositionTag * grpt = dynamic_cast<GRPositionTag *>(tp->grtag);
		if (grpt)
			grpt->BreakTag(pstaff,tp->assocpos);
	}
}

/** \brief Called, when the new staff has
	been created and we need to resume the tags ...
	addpos is the position within the staff, where the
	open-tags should be (can be) added.
*/
void GRPossibleBreakState::GRVoiceTagsAndStaff::BeginAfterBreak(GRStaff * newstaff,
											GuidoPos addpos)
{
	GuidoPos pos = grtags->GetHeadPosition();
	while (pos)
	{
		GRTagAndPosition * tp = grtags->GetNext(pos);
		GRPositionTag * grpt = dynamic_cast<GRPositionTag *>(tp->grtag);
		if (grpt)
			grpt->ResumeTag(newstaff,tp->assocpos);
	
		// add the tag to the new tag to the staff ...
		// this must be at the correct position though! 
		GRNotationElement * el = dynamic_cast<GRNotationElement *>(tp->grtag);
		if (el && newstaff)
		{
			if (addpos)	newstaff->AddElementAt(addpos,el);
			else	newstaff->AddTail(el);
		}
	}
}

/** \brief Changes all inlying staff-pointers
	so that they belong to new staves.
*/
void GRPossibleBreakState::ChangeStaffPointers(KF_IVector<GRStaff> * newstaves,
							GRPossibleBreakState * oldpbs)
{
	int i;
	for (i=ssvect->GetMinimum();i<=ssvect->GetMaximum(); ++i )
	{
		GRStaffAndState * sas = ssvect->Get(i);
		if (!sas) continue;
		assert(sas);
		// this is easy, because the staves
		// are still numbered...
		sas->pstaff = newstaves->Get(i);

		if (oldpbs && oldpbs->ssvect)
		{
			GRStaffAndState * oldsas = oldpbs->ssvect->Get(i);
			if (oldsas)
			{
				if( oldsas->tpos == 0 )
				{
					sas->tpos = sas->pstaff->mCompElements.GetTailPosition();
				}
			}
		}
	}

	for (i=vtsvect->GetMinimum();i<=vtsvect->GetMaximum(); ++i )
	{
		GRVoiceTagsAndStaff *vts = vtsvect->Get(i);
		assert(vts);
		// this relies heavily on STAFFNUM!
		vts->pstaff = newstaves->Get(vts->grvoicemgr->getStaffNum());
	}
}
