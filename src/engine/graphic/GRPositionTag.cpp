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

#include <iostream>

#include "ARPositionTag.h"
#include "ARMusicalTag.h"

#include "GRPositionTag.h"
#include "GRStaff.h"
#include "GRSystemSlice.h"
#include "GRGlue.h"

GRPositionTag::GRPositionTag(const ARMusicalTag * armt) : GRTag(armt), mStartEndList(1)
{
	if (armt) {
		const ARPositionTag * arpt = dynamic_cast<const ARPositionTag *>(armt);
		if (arpt) fEndPos = arpt->getEndPosition();
	}
	lastendflag = GRSystemStartEndStruct::NOTKNOWN;
	lastendElement = NULL;
	lastendpos = NULL;
}

GRPositionTag::GRPositionTag(GuidoPos endpos, const ARMusicalTag * armt)
	: GRTag(armt), fEndPos(endpos), mStartEndList(1)
{
	lastendflag = GRSystemStartEndStruct::NOTKNOWN;
	lastendElement = NULL;
	lastendpos = NULL;
}

GRSystemStartEndStruct * GRPositionTag::getSystemStartEndStruct(const GRSystem * grsystem) const
{
	if (mStartEndList.GetCount() == 1)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetHead();
		if (sse && sse->grsystem == NULL)
			return sse;

		if (sse && sse->grsystem == grsystem)
			return sse;
		return NULL;
	}
	GuidoPos pos = mStartEndList.GetHeadPosition();
	while (pos)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetNext(pos);
		if (sse->grsystem == grsystem || sse->grsystem == NULL)
			return sse;
	}
	return NULL;
}

/** \brief Deletes the Staff from the sse (if it is found) and returns 1, if no more
	staffs remain, that is sse is empty. Then the positiontag can be deleted.
*/
bool GRPositionTag::DeleteStaff(GRStaff * grstaff)
{
	// if (error) return 1;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	if (!sse)
	{
//		assert(false);
		return (mStartEndList.GetCount() == 0);
	}
	// also deletes it ...
	// remove the associations ...
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
	{
		if (sse->startElement)	sse->startElement->removeAssociation(el);
// here we have a real problem: there are some case where the endElement has been freed before this call and of course it crashes :-( 
// since memory management is quite complex (and a bit messy), the only workaround I've found is to look at the associated fields
// with the hope that the memory is not reused in between (that's currently the case)
// a real solution would be to have automatic memory management using smart pointers... 
// it'll have to be done sooner or later [D.F. 2012 march 6]
		if (sse->endElement && sse->endElement->associated())	sse->endElement->removeAssociation(el);
	}
	// mStartEndList.setOwnership(0);
	mStartEndList.RemoveElement(sse);
	// mStartEndList.setOwnership(1);
	// this really deletes the sse ...
	// deleteSSE(sse);
	
	return (mStartEndList.GetCount() == 0);
}

void GRPositionTag::changeCurrentSystem(GRSystem * psystem)
{
	// this replaces the system-entry of the last sse

	assert(false);

}

/** \brief Called after the new Staff has been created by the
	Staffmanager after a pbreak-Break.

	(It is ALWAYS called after a call to BreakTag)
	It is somewhat analogous to StaffBegin but uses the assocpos.`
*/
void GRPositionTag::ResumeTag(GRStaff * grstaff,GuidoPos assocpos)
{
	if (!grstaff) return;

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::OPENLEFT;
	// here, we use the assocpos
	sse->startpos = assocpos;

	// no need to call addAssociation (did that already ...)!
	sse->endflag = lastendflag;
	sse->endElement = lastendElement;
	sse->endpos = lastendpos;
	
	GRSaveStruct * st = getNewGRSaveStruct();
	sse->p = (void *) st;

	addSystemStartEndStruct (sse);

	if (getError())
	{
		setStartElement(grstaff,NULL);
		setEndElement(grstaff,NULL);
		return;
	}

	setStartElement(grstaff, /*ynamic_cast<GRNotationElement *>*/(grstaff->getSecondGlue()));
}

/** \brief Called to break a tag at a previously saved PBreak-Location.
	It sets the sse-Positions ...

	(It is somewhat equivalent to StaffFinished,
	with the difference, that assocpos is used). assocpos is the tail-position
	of the associated list at the time of the pbreak-situation.
	Now, this routine is called when a tag needs to be broken
	because the optimum line break algorithm has determined the breaklocation.
*/
void GRPositionTag::BreakTag(GRStaff * grstaff, GuidoPos & assocpos)
{
	if (grstaff == 0) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);

	if (getError())
	{
		setStartElement(grstaff,NULL);
		setEndElement(grstaff,NULL);
		lastendElement = NULL;
		return;
	}

	// now we do something .....
	NEPointerList * associated = 0;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
		associated = el->getAssociations();

	if (!associated) return;

	// only, if the startElement is not set and
	// we do not have an OPENLEFT, the very first
	// element from the association is made the
	// startElement.
	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST 
		&& sse->startElement == NULL)
	{
		setStartElement(grstaff,associated->GetHead());
		sse->startpos = associated->GetHeadPosition();
	}

	// now, we remeber the last end-setting ...
	lastendElement = sse->endElement;
	lastendflag = sse->endflag;
	lastendpos = sse->endpos;

	// now, we set the new OPENRIGHT and get the
	// endglue ...

	// the old association of the end-element MUST
	// be maintained! -> 
	sse->endElement = NULL;

	setEndElement(grstaff,grstaff->getEndGlue());
	sse->endflag = GRSystemStartEndStruct::OPENRIGHT;
	// this uses assocpos!
	sse->endpos = assocpos;

	sse->grsystem = grstaff->getGRSystem();

	// now, we increment the assocpos, so that
	// we can save this position for
	// the NEW Segment build with ResumeTag
	if (associated && assocpos)
		associated->GetNext(assocpos);

	// I now have to add a new sse-that handles the next system ?

}

void GRPositionTag::StaffFinished(GRStaff * grstaff)
{
	if (!grstaff) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());

	assert(sse);

	if (getError())
	{
		setStartElement(grstaff,NULL);
		setEndElement(grstaff,NULL);
		lastendElement = NULL;
		return;
	}

	NEPointerList * associated = NULL;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
		associated = el->getAssociations();
	if (associated == 0) return;


	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST
		&& sse->startElement == NULL)
	{
		setStartElement(grstaff,associated->GetHead());
		sse->startpos = associated->GetHeadPosition();
	}

	lastendflag = sse->endflag;
	lastendElement = sse->endElement;
	lastendpos = sse->endpos;

	// there is no old lastendelement in this case
	assert(lastendElement == NULL);

	setEndElement(grstaff, grstaff->getEndGlue());
	sse->endflag = GRSystemStartEndStruct::OPENRIGHT;
	sse->endpos = associated->GetTailPosition();

}

void GRPositionTag::StaffBegin(GRStaff *grstaff)
{
	// now we need to build a new staff 
	if (!grstaff) return;

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRSaveStruct * st = getNewGRSaveStruct();
	sse->p = (void *) st;

	sse->grsystem = grstaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::OPENLEFT;
	sse->startpos = NULL;

	// ths startpos needs to be filled
	// by the next addAssociation call!

	// no need to call addAssociation (did that already ...)!
	sse->endflag = lastendflag;
	sse->endElement = lastendElement;
	sse->endpos = lastendpos;
	
	addSystemStartEndStruct (sse);

	if(getError())
	{
		setStartElement(grstaff,NULL);
		setEndElement(grstaff,NULL);
		return;
	}

	setStartElement(grstaff, /*dynamic cast<GRNotationElement *>*/
		(grstaff->getSecondGlue()));
}

void GRPositionTag::RangeEnd(GRStaff * grstaff)
{
	if (grstaff == 0) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);
	if (getError())
	{
		setStartElement(grstaff,NULL);
		setEndElement(grstaff,NULL);
		return;
	}

	NEPointerList * associated = NULL;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
		associated = el->getAssociations();
	if (associated == 0) return;

	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST
		&& sse->startElement == NULL)
	{
		setStartElement(grstaff,associated->GetHead());
		sse->startpos = associated->GetHeadPosition();
	}

	setEndElement(grstaff,associated->GetTail());
	sse->endflag = GRSystemStartEndStruct::RIGHTMOST;
	sse->endpos = associated->GetTailPosition();
}

/** \brief Here also the position must be retrieved
*/
void GRPositionTag::setStartElement(const GRStaff * grstaff,
				GRNotationElement * n_element)
{
	GRSystemStartEndStruct * sse =
		getSystemStartEndStruct(grstaff->getGRSystem());

	assert(sse);

	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	assert(el);

	if (sse->startElement)
	{
		sse->startElement->removeAssociation(el);
	}

	if (!getError())
	{
		sse->startElement = n_element;
		if (sse->startElement)
		{
			sse->startElement->addAssociation(el);
		}
	}
	else
		sse->startElement = NULL;
}

void GRPositionTag::setEndElement(const GRStaff * grstaff,
				GRNotationElement * n_element)
{
	GRSystemStartEndStruct * sse =
		getSystemStartEndStruct(grstaff->getGRSystem());

	assert(sse);

	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	assert(el);

	if (sse->endElement)
	{
		sse->endElement->removeAssociation(el);
	}

	if (!getError())
	{
		sse->endElement = n_element;
		if (sse->endElement)
		{
			sse->endElement->addAssociation(el);
		}
	}
	else
		sse->endElement = NULL;
}

void GRPositionTag::addAssociation(GRNotationElement * grnot)
{
	if (getError()) return;

	const GRStaff * pstaff = grnot->getGRStaff();
	assert(pstaff);
	GRSystemStartEndStruct * sse= getSystemStartEndStruct(pstaff->getGRSystem());	

	NEPointerList * associated = NULL;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
		associated = el->getAssociations();

	if (!associated) return;
	
	if (sse && sse->startflag == GRSystemStartEndStruct::OPENLEFT 
		&& sse->startpos == NULL)
		sse->startpos = associated->GetTailPosition();
}

/** \brief Called by most error-routines to delete all occurences of the TAG
*/
void GRPositionTag::DeleteAllSSEs()
{
	GuidoPos pos = mStartEndList.GetHeadPosition();
	while (pos)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetNext(pos);
		// also deletes it ...
		// remove the associations ...
		GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
		if (el)
		{
			if (sse->startElement)
				sse->startElement->removeAssociation(el);
			if (sse->endElement)
				sse->endElement->removeAssociation(el);
		}
	}
}

void GRPositionTag::removeAssociation(GRNotationElement * grnot)
{
	// now we test, if the endElement or startElement
	// is effected ...

	const GRStaff * staff = grnot->getGRStaff();
	if( staff )
	{
		GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
		if (sse)
		{
			if (sse->startElement == grnot)
			{
				sse->startElement = NULL;
				sse->startflag = GRSystemStartEndStruct::NOTKNOWN;
				sse->startpos = NULL;
			}
			if (sse->endElement == grnot)
			{
				sse->endElement = NULL;
				sse->endflag = GRSystemStartEndStruct::NOTKNOWN;
				sse->endpos = NULL;
			}
		}
	}
}

GRSystemStartEndStruct::~GRSystemStartEndStruct()
{
	GRPositionTag::GRSaveStruct * st = (GRPositionTag::GRSaveStruct *) p;
	delete st;
	p = 0;
}

