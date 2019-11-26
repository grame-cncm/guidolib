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
#include <sstream>
#include <algorithm>

#include "GRBeam.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRStaff.h"
#include "GRPage.h"
#include "GRMusic.h"
#include "GRSystemTag.h"
#include "GRAutoBeam.h"
#include "GREmpty.h"
#include "GRSystemSlice.h"
#include "GRSimpleBeam.h"
#include "VGDevice.h"
#include "secureio.h"
#include "FontManager.h"
#include "GRTremolo.h"

#include "TagParameterFloat.h"

using namespace std;

GRBeamSaveStruct::~GRBeamSaveStruct()
{
	delete simpleBeams;
}

pair<float, float> GRBeam::fLastPositionOfBarDuration = make_pair(0.f, 0.f);

GRBeam::GRBeam(GRStaff * grstaf,const ARBeam * arbeam) : GRPTagARNotationElement(arbeam)
{
	fHasRestInMiddle = false;
	fIsGraceBeaming = false;
	fIsFeathered = arbeam->isFeatheredBeam();
	if(fIsFeathered) {
        const ARFeatheredBeam * ar = static_cast<const ARFeatheredBeam *>(arbeam->isARFeatheredBeam());
		fDrawDur = ar->drawDuration();
	}
	else fDrawDur = false;

	fLastPositionOfBarDuration.first = 0;
	fLastPositionOfBarDuration.second = 0;
	setGRStaff(grstaf);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct();
	
	GRBeamSaveStruct * st = new GRBeamSaveStruct();
	sse->grsystem = grstaf->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) st;

	addSystemStartEndStruct (sse);
	fLevel = 0;
}

GRBeam::~GRBeam()
{
	// associations are removed within
	// the GRPositionTag-Removal ...
	FreeAssociatedList();
}

// -----------------------------------------------------------------------------
void GRBeam::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

void GRBeam::OnDraw( VGDevice & hdc) const
{
	if (getError()) return;
	if(!mDraw || !mShow) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if (sse == 0) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	assert(st);

	if (mColRef) {
		VGColor color ( mColRef ); 	// custom or black
		hdc.PushFillColor( color );
		hdc.PushPen( color, 1);
	}
		
	float ax [4] = { st->p[0].x, st->p[1].x, st->p[3].x, st->p[2].x };
	float ay [4] = { st->p[0].y, st->p[1].y, st->p[3].y, st->p[2].y };
	
	// This does the drawing!
	hdc.Polygon(ax, ay, 4);
	
	if (st->simpleBeams)
	{
		GuidoPos smplpos = st->simpleBeams->GetHeadPosition();
		while (smplpos)
		{
			GRSimpleBeam * smplbeam = st->simpleBeams->GetNext(smplpos);
			smplbeam->OnDraw(hdc);
		}
	}

	if(fDrawDur)
	{
		const char * fraction = st->duration.c_str();
		size_t n = st->duration.length();

        hdc.PushPenWidth(4);

		if(sse->startflag != GRSystemStartEndStruct::OPENLEFT)
		{	
			hdc.Line(st->DurationLine[0].x, st->DurationLine[0].y, st->DurationLine[1].x, st->DurationLine[1].y);
			hdc.Line(st->DurationLine[1].x, st->DurationLine[1].y, st->DurationLine[2].x, st->DurationLine[2].y);
			hdc.Line(st->DurationLine[3].x, st->DurationLine[3].y, st->DurationLine[4].x, st->DurationLine[4].y);
		}
		else
			hdc.Line(st->DurationLine[1].x, st->DurationLine[1].y, st->DurationLine[4].x, st->DurationLine[4].y);
		if(sse->endflag != GRSystemStartEndStruct::OPENRIGHT)
			hdc.Line(st->DurationLine[4].x, st->DurationLine[4].y, st->DurationLine[5].x, st->DurationLine[5].y);
		
		const VGFont* hmyfont;
		hmyfont = FontManager::gFontText;
		hdc.SetTextFont( hmyfont );

		if (sse->startflag != GRSystemStartEndStruct::OPENLEFT)
			hdc.DrawString(st->DurationLine[2].x + LSPACE/4, st->DurationLine[2].y + LSPACE / 2, fraction, int(n));

        hdc.PopPenWidth();
	}

	if (mColRef)
    {
		hdc.PopPen();
		hdc.PopFillColor();
	}

}

void GRBeam::addAssociation(GRNotationElement * grnot)
{
	if (getError() || !grnot) return ;

	const GRStaff * staff = grnot->getGRStaff();
	if (staff == 0) return;

    GRNote * grnote = dynamic_cast<GRNote*>(grnot);
    bool isGrace = grnote ? grnote->isGraceNote() : false;
	if (isGrace) {
		if (!mAssociated || (mAssociated->size() == 0))
			fIsGraceBeaming = true;		// beam start on a grace note
		if (!fIsGraceBeaming) return;	// otherwise skip grace notes
	}
	else if (fIsGraceBeaming) return;	// skip regular notes when the beaming starts on grace notes
 
	if (grnot->getDuration() == DURATION_0  && !isGrace) {
		const GREvent * grn = grnot->isGREvent();
		if (!grn || grn->getGlobalStem() == NULL)
			return;
	}

	// if we are an autobeam we do not beam rests ....
	const bool isautobeam = isAutoBeam(); // (dynamic cast<GRAutoBeam *>(this) != 0);

	GREvent * grn = GREvent::cast(grnot);
	if (grn == 0) {
		setError(staff,1);
		return ;
	}

	const bool isRest = (grnot->isRest() != 0);
	if (isautobeam && isRest) {
		if (mAssociated && mAssociated->GetCount() > 0)
			fHasRestInMiddle = true;
	}

	if (fHasRestInMiddle && !isRest  && !getARBeam()->isFullBeaming()) {
		// then we have a real Error
		setError(staff,1);
		return;
	}

	// ignore all elements with duration 0 but only, if I already have the stem
	if ((grnot->getDuration() == DURATION_0) && !isGrace)
	{
		if (mAssociated) {
			GRNotationElement * el = mAssociated->GetTail();
			if (el) {
				const GREvent * ev = el->isGREvent();
				if (ev->getGlobalStem() == NULL ||
					ev->getGlobalStem() == grn->getGlobalStem())
					return;
			}
		}
	}

	if (dynamic_cast<const GRSingleRest *>(grn))
		return;

	const ARMusicalObject* o = grn->getAbstractRepresentation();
	bool allowBeam = isautobeam ? false : o && o->isARNote() && ((o->getDuration() == DURATION_2) || o->isEmptyNote());
	if ((grn->getNumFaehnchen() == 0) && !allowBeam) {
		setError(staff,1);
		return ;
	} 

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if (!sse) return ;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (grn->getStemDirSet())
		st->dirset = 1;
	else {
		if (grn->getStemDirection() == dirUP)			st->direction++;
		else if (grn->getStemDirection() == dirDOWN)	st->direction--;
	}
	GRPTagARNotationElement::addAssociation(grnot);
		
	// this is a test ... and cannot be done that way.
	// otherwise we do not get the flags on again ...
	grn->setFlagOnOff(false);
}

void GRBeam::RangeEnd(GRStaff * grstaff)
{
	assert(grstaff);

	GRPositionTag::RangeEnd(grstaff);

	if (getError()) return;
	if (!mAssociated) return;
	
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);

	if (mAssociated && (mAssociated->GetCount() == 1) && isAutoBeam())
	{
		GREvent * ev = GREvent::cast(mAssociated->GetHead());
		if (ev)
		{
			ev->setFlagOnOff(true);
			ev->decBeamCount();
			if (sse->startElement)	// something did not work, if the starElement is a glue ... and not an event
				ev->removeAssociation(this);
			if (sse->endElement)
				ev->removeAssociation(this);
		}
		return;
	}

	GREvent * grn;
	GuidoPos syststpos = sse->startpos;
	if (syststpos)
	{
		// this is all done so that I really get a correct first staff to test my stuff ...
		while (syststpos && !(mAssociated->GetAt(syststpos)))
		{
			mAssociated->GetNext(syststpos);
		}
		int tststaffnum = mAssociated->GetNext(syststpos)->getStaffNumber(); 
		while (syststpos)
		{
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el)
			{
				if (el->getStaffNumber() != tststaffnum)
				{
					setTagType (GRTag::SYSTEMTAG);
					GRSystemTag * mysystag = new GRSystemTag(this);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	GuidoPos pos = sse->startpos;
	GRBeamSaveStruct * st = (GRBeamSaveStruct *) sse->p;
	bool first = true;

	GDirection mytmpdir = dirOFF;
	while (pos)
	{
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
		{
			if (!st->dirset)
			{
				if (first)
				{
					// get the stemdir of the first !? maybe it now has been set?
					mytmpdir = grn->getStemDirection();
				}
				if (mytmpdir != dirOFF)
					grn->setStemDirection(mytmpdir);
				else if (st->direction>=0)
					grn->setStemDirection(dirUP);
				else
					grn->setStemDirection(dirDOWN);
			}
			grn->setFlagOnOff(false);
			first = false;
		}
	}
}

void GRBeam::StaffFinished(GRStaff * grstaff)
{
	assert(false);
	assert(grstaff);

	// first, all the BASIC stuff is handled ...
	GRPositionTag::StaffFinished(grstaff);
	if (getError()) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);

	GuidoPos syststpos = sse->startpos;
	if (syststpos) {
		// this is all done so that I really get a correct first staff to test my stuff ...
		while (syststpos && !(mAssociated->GetAt(syststpos)))
			mAssociated->GetNext(syststpos);

		const GRStaff *tststaff = mAssociated->GetNext(syststpos)->getGRStaff();
		while (syststpos) {
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el) {
				if (el->getGRStaff() != tststaff) {
					setTagType (GRTag::SYSTEMTAG);
					GRSystemTag * mysystag = new GRSystemTag(this);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (!checkNotes(grstaff)) return;
//	if (error) return;

	GuidoPos pos = sse->startpos;
	while (pos) {
		GREvent * grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn) {
			if (!st->dirset) {
				if (st->direction > 0)	grn->setStemDirection(dirUP);
				else					grn->setStemDirection(dirDOWN);
			}
			grn->setFlagOnOff(false);
		}
	}
}

void GRBeam::StaffBegin(GRStaff * grstaff)
{
	// now we need to build a new staff 
	assert(grstaff);
	GRPositionTag::StaffBegin(grstaff);
}

//--------------------------------------------------------------------
void GRBeam::initp0 (GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos)
{
	// -- Init point 0 (top left)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const ARBeam * arBeam = getARBeam();
	const GREvent * refEvt = startEl;
	bool setref = (getTagType() == SYSTEMTAG);

	if (setref && !infos.stemsReverse && infos.stavesStartEnd && !startEl->getStemLengthSet())
		refEvt = (infos.stemdir == dirUP) ? infos.highNote : infos.lowNote;

	GRStaff * refStaff = refEvt ? refEvt->getGRStaff() : getGRStaff();
	infos.stemdir = refEvt ? refEvt->getStemDirection() : GDirection(st->direction);
	infos.currentSize = refEvt ? refEvt->getSize() : 1;
	infos.currentLSPACE = refStaff->getStaffLSPACE();
	st->p[0] = refEvt ? refEvt->getStemStartPos() : refStaff->getPosition();
//cerr << "GRBeam::initp0 " << refEvt << " " << st->p[0] << " " << refEvt->getStemLength() << endl;
	if (arBeam && arBeam->isGuidoSpecBeam() && refEvt)
		st->p[0].y = refEvt->getPosition().y;

	if (setref) {
		st->p[0].x += infos.startStaff.x;
		float yoffset = 0;
		if (infos.stavesStartEnd && !infos.stemsReverse && !startEl->getStemLengthSet()) {
			if (infos.stemdir == dirUP) {
				yoffset = std::min(infos.endStaff.y, infos.startStaff.y);
			}
			else yoffset = std::max(infos.startStaff.y, infos.endStaff.y);
		}
		else yoffset = infos.startStaff.y;
		st->p[0].y += yoffset;
	}
	st->p[1] = st->p[0];
	
	// -- Adjust point 0
	const TagParameterFloat * p = arBeam->getDx1();
	if (p && p->TagIsSet())
		st->p[0].x += p->getValue(infos.currentLSPACE);
	else {
		// This depends on the direction, we do not know this yet (do we?)
		if (infos.oneNote) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(2.0f, result, "hs", infos.currentLSPACE);

            if (conversionOk)
                st->p[0].x -= (float)result * infos.currentSize;
		}
	}

	p = arBeam->getDy1();
	if (p && p->TagIsSet()) st->p[0].y -= p->getValue(infos.currentLSPACE);
}

//--------------------------------------------------------------------
void GRBeam::initp1 (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 1
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const TagParameterFloat * p = arBeam->getDx2();
	if (p && p->TagIsSet())
		st->p[1].x += p->getValue(infos.currentLSPACE);
	else {
		if (infos.oneNote) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(2.0f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->p[1].x -= (float)result * infos.currentSize;
        }
	}

	p = arBeam->getDy2();
	if (p && p->TagIsSet()) st->p[1].y -= p->getValue(infos.currentLSPACE);
	else {
		if (infos.stemdir == dirUP) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->p[1].y = st->p[0].y + (float)result * infos.currentSize;
        }
		else if (infos.stemdir == dirDOWN) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->p[1].y = st->p[0].y - (float)result * infos.currentSize;
        }
	}
}

//--------------------------------------------------------------------
void GRBeam::initp2 (GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos)
{
	// -- Init point 2 (top right)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const ARBeam * arBeam = getARBeam();

	st->p[2] = endEl ? endEl->getStemEndPos() : st->p[0];
	// beam length adjustment - DF sept 15 2009
	st->p[2].x += infos.currentLSPACE/10;
	if (arBeam && arBeam->isGuidoSpecBeam() && endEl)
		st->p[2].y = endEl->getPosition().y;
	GRStaff * refStaff = endEl ? endEl->getGRStaff() : getGRStaff();
	infos.currentLSPACE = refStaff->getStaffLSPACE();

//cerr << "GRBeam::initp2 " << endEl << " " << st->p[2] << endl;
//cerr << "GRBeam::initp2 " << endEl->getGlobalStem()->getPosition() << " " << endEl->getGlobalStem()->getStemLength() << endl;

	if (getTagType() == SYSTEMTAG) {
		st->p[2] += refStaff->getPosition();
		if (infos.stavesStartEnd && !infos.stemsReverse &&  !endEl->getStemLengthSet()) {
			st->p[2].y = st->p[0].y;
			float slopeOffset = infos.currentLSPACE/2;
			if (infos.endStaff.y < infos.startStaff.y) st->p[2].y -= slopeOffset;
			else st->p[2].y += slopeOffset;
		}
	}

	st->p[3] = st->p[2];

	// -- Adjust point 2
	const TagParameterFloat * p = arBeam->getDx3();
	if (p)					st->p[2].x += p->getValue(infos.currentLSPACE);

	p = arBeam->getDy3();
	if (p && p->TagIsSet()) st->p[2].y -= p->getValue(infos.currentLSPACE);
	else {
		GCoord val = 0;
		const TagParameterFloat * p = arBeam->getDy1();
		if (p && p->TagIsSet())	val = p->getValue(infos.currentLSPACE);
		st->p[2].y -= val;
	}
}

//--------------------------------------------------------------------
void GRBeam::initp3 (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 3 (bottom right)
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	const TagParameterFloat * p = arBeam->getDx4();
	if (p)					st->p[3].x += (p->getValue(infos.currentLSPACE));

	p = arBeam->getDy4();
	if (p && p->TagIsSet())	st->p[3].y -= (p->getValue(infos.currentLSPACE));
	else {
		if (infos.stemdir == dirUP)
        {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->p[3].y = st->p[2].y + (GCoord)((float)result * infos.currentSize);
        }
		else if (infos.stemdir == dirDOWN)
        {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->p[3].y = st->p[2].y - (GCoord)((float)result * infos.currentSize);
        }
	}
}

//--------------------------------------------------------------------
void GRBeam::slopeAdjust (GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl, float slope, PosInfos& infos)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (startEl && endEl && infos.stemdir != endEl->getStemDirection())
	{
		// then we should try an optimizing strategy which is not implemented now ....
	}
	else if (startEl && endEl && slope > 0.1f )
	{
		// adjust the length of the stem for second st->p ...			
		if( infos.stemdir == dirDOWN) {
			// change the first one ...
			const float newy = st->p[2].y - 0.1f * (float)(st->p[2].x - st->p[0].x);
			const float diff = newy - (float)st->p[0].y;
			st->p[0].y = (GCoord) newy;
			st->p[1].y += (GCoord) diff;
		}
		else {
			// change the second one ...
			const float newy = st->p[0].y + 0.1f * (float)(st->p[2].x - st->p[0].x);	
			const float diff = newy - (float)st->p[2].y;
			st->p[2].y = (GCoord) newy;
			st->p[3].y += (GCoord) diff;				
		}
	}
	else if (startEl && endEl && slope < -0.1f)
	{
		if (infos.stemdir == dirDOWN)
		{
			// change the last one 
			const float newy = -0.1f * (float)(st->p[2].x - st->p[0].x) + st->p[0].y;
			const float diff =  newy - (float)st->p[2].y;
			st->p[2].y = (GCoord) newy;
			st->p[3].y += (GCoord) diff;
		}
		else
		{
			// change the first
			const float newy = st->p[2].y + 0.1f * (float) (st->p[2].x - st->p[0].x);
			const float diff =  newy - (float)st->p[0].y;
			st->p[0].y = (GCoord) newy;
			st->p[1].y += (GCoord) diff;
		}
	}
	else if (endEl == 0)
	{
		// then we have an empty-event make the slope even .... 
		// or make it dependant on generell direction of beam (how ...)
		st->p[2].y = st->p[0].y;
		st->p[3].y = st->p[1].y;
	}		
	else if (startEl == 0)
	{
		st->p[0].y = st->p[2].y;
		st->p[1].y = st->p[3].y;
	}
}

//--------------------------------------------------------------------
void GRBeam::refreshBeams (const GRSystemStartEndStruct * sse, float currentLSPACE, int dir)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	NVPoint p[4];
	p[0] = st->p[0];
	p[1] = st->p[1];
	p[2] = st->p[2];
	p[3] = st->p[3];
	const float yFact1 = 0.75f * currentLSPACE * dir;
//	const float yFact2 = 0.4f * currentLSPACE;
	if (st->simpleBeams)
	{
		GuidoPos smplpos = st->simpleBeams->GetHeadPosition();
		while (smplpos)
		{
			GRSimpleBeam * smplbeam = st->simpleBeams->GetNext(smplpos);
			smplbeam->setPoints(p);
			p[0].y += yFact1;
			p[1].y += yFact1;
			p[2].y += yFact1;
			p[3].y += yFact1;
		}
	}
}

//--------------------------------------------------------------------
void GRBeam::setBeams (GRSystemStartEndStruct * sse, PosInfos& infos, float yFact1, float yFact2, int dir)
{
	if (fIsFeathered) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	// for beam length adjustment - DF sept 15 2009
	const float xadjust = infos.currentLSPACE/10;
	GDirection lastLocalDir = dirOFF;
	int previousBeamsCount = 0;
	
	GuidoPos pos = sse->startpos;
	while (pos) {

		GuidoPos oldpos = pos;
		GREvent * stemNote = GREvent::cast(mAssociated->GetNext(pos));
		if (stemNote) {

			GDirection localDir = stemNote->getStemDirection();
			float yLocalFact1 = yFact1 * localDir * infos.currentSize;
			float yLocalFact2 = yFact2 * localDir * infos.currentSize;

			// now we check the number of beams ...
			if (stemNote->getBeamCount() < stemNote->getNumFaehnchen())
			{
				float beamCount = (float)(stemNote->getBeamCount());
				stemNote->incBeamCount();
			
				NVPoint p[4];
				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
					// the additional beam starts at the startElement (glue), we have more beams to draw
					p[0] = sse->startElement->getPosition();					
					if (getTagType() == SYSTEMTAG)
						p[0] += stemNote->getGRStaff()->getPosition();
					p[1] = p[0];
				}
				else {
					// the additional beam starts at sn. We have more beams to draw
					p[0] = stemNote->getStemStartPos();
					if (getTagType() == SYSTEMTAG)
						p[0] += stemNote->getGRStaff()->getPosition();
					p[0].y += beamCount * yLocalFact1;
					if (localDir != dir)
						p[0].y -= yLocalFact2;
					p[1].x = p[0].x;
					p[1].y = p[0].y + yLocalFact2;
				}
				// now we look for the endposition
				GREvent * sn2 = NULL;
				GuidoPos tmppos = pos;
				// partialbeam is set, if the new SimpleBeam only covers part of the masterBeam.
				bool partialbeam = false;
				while (tmppos)
				{
					GuidoPos oldpos2 = tmppos;
					GRNotationElement * elt = mAssociated->GetNext(tmppos);
					GREvent * tmpsn = GREvent::cast(elt);
					if (tmpsn) {
						if (tmpsn->getBeamCount() < tmpsn->getNumFaehnchen()) {
							sn2 = tmpsn;
							sn2->incBeamCount();
							continue;
						}
						else {
							partialbeam = true;
							break;
						}
					}
					if (oldpos2 == sse->endpos) break;
				}
				
				if (sn2) {
					if (!partialbeam && sse->endflag == GRSystemStartEndStruct::OPENRIGHT) {
						// then the position is different ...
						p[2] = sse->endElement->getPosition();
						p[2].x += xadjust;
						if (getTagType() == SYSTEMTAG)
							p[2] += sn2->getGRStaff()->getPosition();
						p[2].y = p[0].y;						
					}
					else {
						// we have an End-Position ...
						p[2] = sn2->getStemEndPos();
						p[2].x += xadjust;
						if (getTagType() == SYSTEMTAG)
							p[2] += sn2->getGRStaff()->getPosition();
						p[2].y += beamCount * yLocalFact1;
					}
					if (localDir != dir)
						p[2].y -= yLocalFact2;
					p[3].x = p[2].x;
					p[3].y = p[2].y + yLocalFact2;
				}
				else {
					// we do not have an End-Positon single beam ... (meaning a single straight flag)
					// but only, if it is not open on the left or the right hand side.
					const float slope = (float)(st->p[2].y - st->p[0].y ) / (float)(st->p[2].x - st->p[0].x);

					if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
						// then we have to deal with the startposition of the glue-element ....
						// BUT, you can only set this, if the previous beam had this beamcount at the end ....
						// how do I know that? not now.

						// sn is the only element .... and we are open on the left ...
						if (getTagType() == SYSTEMTAG)
							p[0] += stemNote->getGRStaff()->getPosition();
						
						p[1] = p[0];
						p[2] = stemNote->getStemEndPos();
						p[2].x += xadjust;
						if (getTagType() == SYSTEMTAG)
							p[2] += stemNote->getGRStaff()->getPosition();
						p[2].y += beamCount * yLocalFact1;
						if (localDir != dir)
							p[2].y -= yLocalFact2;
						p[3] = p[2];
						p[3].y += yLocalFact2;
					}
					else if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT) {						
						p[0] = stemNote->getStemEndPos();
						if (getTagType() == SYSTEMTAG)
							p[0] += stemNote->getGRStaff()->getPosition();
						p[0].y += beamCount * yLocalFact1;
						if (localDir != dir)
							p[0].y -= yLocalFact2;
						p[1] = p[0];
						p[1].y += yLocalFact2;

						p[2] = sse->endElement->getPosition();
						p[2].x += xadjust;
						if (getTagType() == SYSTEMTAG)
							p[2] += sn2->getGRStaff()->getPosition();
						p[2].y = p[0].y;
						
						p[3].x = p[2].x;
						p[3].y = p[1].y;
					}
					/* 26/11/03 
					 Beaming bug: wrong direction for partial beam (beam-bug.gmn)
					 can be tested but changing this test. 				 

						startpos check added to correct problem with partial beam
						going outside a group like in [ _/16 c d/8 ]
					*/
					else if( oldpos == sse->endpos || pos == NULL || ((!stemNote->isSyncopated()) && (oldpos != sse->startpos)))
					{
						// Partial beams leftward ( using slope)
						p[2] = stemNote->getStemEndPos();
						p[2].x += xadjust;
						if (getTagType() == SYSTEMTAG)
							p[2] += stemNote->getGRStaff()->getPosition();
						
						if (localDir != dir)
							p[2].y -= yLocalFact2;

						p[2].y += beamCount * yLocalFact1;
						p[3] = p[2];
						p[3].y += yLocalFact2;

						p[0] = p[2];
						p[0].x -= infos.currentLSPACE;
						p[0].y -= slope * infos.currentLSPACE;
						p[1] = p[0];
						p[1].y += yLocalFact2;
					}
					else
					{
						// Partial beams rightward ( using slope)
						p[2] = p[0];
						p[2].x += infos.currentLSPACE;
						p[2].y += slope * infos.currentLSPACE;
						p[3] = p[2];
						p[3].y += yLocalFact2;
					}
				}
				
				// now we construct a SimpleBeam, we now have to "undo" the systemTag-stuff
				if (getTagType() == SYSTEMTAG)
				{
					const GRStaff * beamstaff = sse->startElement->getGRStaff();
					const NVPoint & offset = beamstaff->getPosition();
					p[0] -= offset;
					p[1] -= offset;
					p[2] -= offset;
					p[3] -= offset;
				}

				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
				{
					p[0].y = p[2].y;
					p[1].y = p[3].y;
				}

				GRSimpleBeam * tmpbeam = new GRSimpleBeam(this,p);
				if( st->simpleBeams == 0 )
					st->simpleBeams = new SimpleBeamList(1);

				st->simpleBeams->AddTail(tmpbeam);
				
				pos  = sse->startpos;
				oldpos = pos;
				lastLocalDir = localDir;
				previousBeamsCount = stemNote->getBeamCount() - 1;
			}
			// a new hack, again to catch stems directions change - DF sept 15 2009
			else if (localDir != dir) {
				// check for stems length
				NVPoint stemloc = stemNote->getStemStartPos();
				if (getTagType() == SYSTEMTAG)
					stemloc += stemNote->getGRStaff()->getPosition();
				int beamscount = stemNote->getBeamCount() - 1;
				if ((beamscount > 0) && (previousBeamsCount > beamscount) && (lastLocalDir != localDir)) {
					if (localDir == dirUP)
						stemNote->changeStemLength(stemNote->getStemLength() + (yLocalFact1 * beamscount));
					else if (localDir == dirDOWN)
						stemNote->changeStemLength(stemNote->getStemLength() - (yLocalFact1 * beamscount));
				}			
			}
		}
		if (oldpos == sse->endpos) break;
	}
}


//--------------------------------------------------------------------
float GRBeam::setStemEndPos (GRSystemStartEndStruct * sse, PosInfos& infos, bool needsadjust, float offsetbeam)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const GREvent * startEl = sse->startElement->isGREvent();

	GuidoPos pos = sse->startpos;
	while (pos)
	{
		// now we calculate the stem-end-positions ...
		GuidoPos oldpos = pos;
		GREvent * sn = GREvent::cast(mAssociated->GetNext(pos));
		if (sn)
		{
			float rx = sn->getStemStartPos().x - st->p[0].x;
			if (getTagType() == SYSTEMTAG)
				rx += sn->getGRStaff()->getPosition().x;
			
			float disty = st->p[2].y - st->p[0].y;
			float distx = st->p[2].x - st->p[0].x;
			float ly = disty / distx * rx;
			
			ly += st->p[0].y;
			
			float diffy = (float)sn->getStemStartPos().y;
			GDirection adjustdir=dirAUTO;
			if (sn->getStemDirection() == dirUP) {
				diffy += (float)sn->getStemLength();
				adjustdir = dirUP;
			}
			else if (sn->getStemDirection() == dirDOWN) { 
				adjustdir = dirDOWN;
				diffy -= (float)sn->getStemLength();
			}
			
			ly -= diffy;
			if (getTagType() == SYSTEMTAG) ly -= (float)sn->getGRStaff()->getPosition().y;
			
			// if we have a beam between grace notes, we don't want an offbase that whould make the stems too long
			const GRNote * gnote = startEl ? startEl->isGRNote() : 0;
			bool isGrace = gnote ? gnote->isGraceNote() : false;
			
			float offbase = isGrace ? 0 : 3.5f * infos.currentLSPACE;			
			if (ly < 0)
			{
				if (needsadjust) {
					if (adjustdir == dirDOWN) {
						const float newoffs = ly - offbase;
						if (newoffs < offsetbeam) {
							if (offsetbeam > 0) {
								GuidoTrace("WARNING: different beam adjustments!");
								offsetbeam = 0;
							}
							else offsetbeam = newoffs;
						}
						ly = -offbase;
					}
					else if (ly > -offbase) {
						const float newoffs = ly + offbase;
						if (newoffs > offsetbeam) {
							if (offsetbeam < 0) {
								GuidoTrace("WARNING: different beam adjustments!");
								offsetbeam = 0;
							}
							else offsetbeam = newoffs;
						}
					}
				}
				ly = -ly;
			}
			else if (needsadjust) {
				if (adjustdir == dirUP) {
					const float newoffs = ly + offbase;
					if (newoffs > offsetbeam) {
						if (offsetbeam < 0) {
							GuidoTrace("WARNING: different beam adjustments!");
							offsetbeam = 0;
						}
						else offsetbeam = newoffs;
					}
					ly = offbase;
				}
				else if (ly < offbase) {
					const float newoffs = ly - offbase;
					if (newoffs < offsetbeam) {
						if (offsetbeam > 0) {
							GuidoTrace("WARNING: different beam adjustments!");
							offsetbeam = 0;
						}
						else offsetbeam = newoffs;
					}
				}
			}
			// adjusted - DF sept 15 2009
			sn->changeStemLength( ly - infos.currentLSPACE/20 );
			// so that the possible next featherd beam knows that he is chained (and musn't change its slope)
			sn->setStemChanged();
		}
		if (oldpos == sse->endpos) break;
	}
	return offsetbeam;
}

//--------------------------------------------------------------------
// if we have a feathered beam, we just have to draw the main beam (already done)
// and the other simple beams will only depend on the begining and ending 
// points, regardless of the durations of the inner notes.
void GRBeam::adjustFeathered (float yFact1, float yFact2, PosInfos& infos, GRSystemStartEndStruct * sse)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	const ARFeatheredBeam * ar = static_cast<const ARFeatheredBeam *>(getARBeam()->isARFeatheredBeam());
	int begin = 0;
	int end = 0;
	const GREvent * stemNoteBegin = mAssociated->GetHead()->isGREvent();
	
	GDirection localDir = stemNoteBegin->getStemDirection();
	float yLocalFact1 = yFact1 * localDir * infos.currentSize;
	float yLocalFact2 = yFact2 * localDir * infos.currentSize;
	
	
	// if the user hasn't set the durations as parameters, 
	// we will take the first and last notes'durations
	end = ar->getLastBeaming();
	begin = ar->getFirstBeaming();

	NVPoint p[4];
	for(int i=1; i <= begin; i++) {
		p[0] = st->p[0];
		p[0].y += (i-1) * yLocalFact1;
		p[1].x = p[0].x;
		p[1].y = p[0].y + yLocalFact2;
			
		p[2] = st->p[2];
		if(end > i || (end == i && i != 1)) 	// no need to draw the main beam again.
			p[2].y += (i-1) * yLocalFact1;
		else
			p[2].y += (end-1) * yLocalFact1;
		p[3].x = p[2].x;
		p[3].y = p[2].y + yLocalFact2;

		GRSimpleBeam * tmpbeam = new GRSimpleBeam(this,p);
		if( st->simpleBeams == 0 )
			st->simpleBeams = new SimpleBeamList(1);

		st->simpleBeams->AddTail(tmpbeam);
	}
	// if end > begin
	for(int i = begin; i < end; i++)
	{
		p[0] = st->p[0];
		p[0].y += (begin-1) * yLocalFact1;
		p[1].x = p[0].x;
		p[1].y = p[0].y + yLocalFact2;

		p[2] = st->p[2];
		p[2].y += i * yLocalFact1;
		p[3].x = p[2].x;
		p[3].y = p[2].y + yLocalFact2;

		GRSimpleBeam * tmpbeam = new GRSimpleBeam(this,p);
		if( st->simpleBeams == 0 )
			st->simpleBeams = new SimpleBeamList(1);

		st->simpleBeams->AddTail(tmpbeam);
	}
	

	// in order to draw the total duration of the beam
	if(fDrawDur)
	{
		TYPE_TIMEPOSITION begin = ar->getBeginTimePosition();
		TYPE_TIMEPOSITION end = ar->getEndTimePosition();
		TYPE_DURATION dur = end - begin;
		int num = dur.getNumerator();
		int den = dur.getDenominator();
		stringstream out;
		out << num << '/' << den;
		st->duration = out.str();
		size_t n = st->duration.length();
				
		GREvent * ev = dynamic_cast<GREvent *>(mAssociated->GetHead());
		const NVPoint p1 = ev->getStemEndPos();
		float xBegin = ev->getPosition().x;
	
		ev = dynamic_cast<GREvent *>(mAssociated->GetTail());
		const NVPoint p2 = ev->getStemEndPos();
		float xEnd = ev->getPosition().x + ev->getBoundingBox().Width()/2;
	
		int dir = ev->getStemDirection();
		float Y1;
		float Y2;
		if(dir>0)
		{
			Y1 = min(p1.y, p2.y) - LSPACE;
			if(Y1>=fLastPositionOfBarDuration.first && Y1<fLastPositionOfBarDuration.second+LSPACE/2)
				Y1 -= LSPACE;
			Y2 = Y1 - LSPACE/2;
			fLastPositionOfBarDuration.first = Y2;
			fLastPositionOfBarDuration.second = Y1;
		}
		else
		{
			Y1 = max(p1.y, p2.y) + LSPACE;
			if(Y1>=fLastPositionOfBarDuration.first-LSPACE/2 && Y1<fLastPositionOfBarDuration.second)
				Y1 += LSPACE;
			Y2 = Y1 + LSPACE/2;
			fLastPositionOfBarDuration.first = Y1;
			fLastPositionOfBarDuration.second = Y2;
		}
		if (xBegin > xEnd)
		{
			if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
				xEnd = sse->endElement->getPosition().x;
			if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
				xBegin = sse->startElement->getPosition().x;
		}
		float x = xBegin + (xEnd - xBegin) / 2;
		float X1 = x - (n - 0.5f) / 2 * LSPACE;
		float X2 = x + (n - 0.5f) / 2 * LSPACE;

		st->DurationLine[0] = NVPoint(xBegin, Y1);
		st->DurationLine[1] = NVPoint(xBegin, Y2);
		st->DurationLine[2] = NVPoint(X1, Y2);
		st->DurationLine[3] = NVPoint(X2, Y2);
		st->DurationLine[4] = NVPoint(xEnd, Y2);
		st->DurationLine[5] = NVPoint(xEnd, Y1);
	}
}

//--------------------------------------------------------------------
// scan associated elements to detect reverse beaming
bool GRBeam::reverseStems 	(const NEPointerList* assoc) const
{
	GDirection dir = dirOFF;
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GRNotationElement* elt = assoc->GetNext(pos);
		const GREvent* ev = elt->isGREvent();
		if (ev) {
			GDirection nd = ev->getStemDirection();
			if (dir == dirOFF) dir = nd;
			else if ((dir == dirUP) && (nd == dirDOWN)) return true;
			else if ((dir == dirDOWN) && (nd == dirUP)) return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------
void GRBeam::yRange (const NEPointerList* assoc, const GREvent*& high, const GREvent*& low) const
{
	const GREvent* ymin  = 0;
	const GREvent* ymax = 0;
	float min = 10000000.f;
	float max = 0;
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GREvent* ev = assoc->GetNext(pos)->isGREvent();
		if (ev) {
			NVPoint p = ev->getPosition() + ev->getGRStaff()->getPosition();
			if (p.y > max) {
				max = p.y;
				ymax = ev;
			}
			if (p.y < min) {
				min = p.y;
				ymin = ev;
			}
		}
	}
	high = ymin;
	low = ymax;
}

//--------------------------------------------------------------------
void GRBeam::checkEndStemsReverse  	(GREvent* ev, const SimpleBeamList * beams) const
{
	float maxy = 0;
	float miny = 100000000.f;
	GuidoPos pos = beams->GetHeadPosition();
	while (pos) {
		const GRSimpleBeam* b = beams->GetNext(pos);
		if (b->fPoints[3].y > maxy) maxy = b->fPoints[3].y;
		if (b->fPoints[2].y > miny) miny = b->fPoints[2].y;
	}
	float slength = ev->getStemLength();
	float thick = ev->getGRStaff()->getStaffLSPACE()/4; // this is to take account of the beam thickness
	if (ev->getStemDirection() == dirUP) {
		float y = ev->getPosition().y - slength;
		if (y > miny) ev->setStemLength(slength + y - miny - thick);
	}
	else {
		float y = ev->getPosition().y + slength;
		if (y < maxy) ev->setStemLength(slength + maxy - y - thick);
	}
}

//--------------------------------------------------------------------
// this method has been introduced to trig the position computation in case of
// shared stem with chords spanning different staves
// (the global stem layout is changed after tellPosition has been called for the beam)
void GRBeam::refreshPosition()
{
	GRNotationElement * el = mAssociated->GetTail();
	tellPosition (el, el->getPosition());
	const GRStaff* staff = el->getGRStaff();
	const GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	const GREvent * ev = sse->startElement->isGREvent();
	refreshBeams(sse, staff->getStaffLSPACE(), (ev->getStemDirection() == dirUP) ? 1 : -1);
}

//--------------------------------------------------------------------
/** \brief Called from the LAST-Note of the Beam ...
*/
// (JB) \bug There is probably a bug somewhere: with map mode
// different than "ISOTROPIC" (proportionnal), beamings polygons between "UP" stems
// draw at an incorrect x position, while "DOWN" beams are always ok.
//
// Too long, needs some code-factorization, if possible.
void GRBeam::tellPosition( GObject * gobj, const NVPoint & p_pos)
{
	/* Beams are polygons made of four points:
		
		0                  2		
		 ------------------
		 |                |
		 ------------------
		1                  3
	*/

	// a new test is performed: if it is a systemTag and
	// it is not a systemcall (checkpos), than we can just do nothing.
	if (getError() || !mAssociated || ( mAssociated->GetCount() == 0 )
		|| ( getTagType() == GRTag::SYSTEMTAG && !mIsSystemCall ))
		return;
	
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(gobj);
	if (!el || !el->getGRStaff()) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(el->getGRStaff()->getGRSystem());
	assert(sse);
	if (el != sse->endElement) return;
	if(fLevel != 0) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const GREvent * startEl = sse->startElement->isGREvent();
	GREvent * endEl   = sse->endElement->isGREvent();

	if (startEl && startEl->getGlobalStem()) startEl->getGlobalStem()->setBeam (this);
	if (endEl   && endEl->getGlobalStem()) endEl->getGlobalStem()->setBeam (this);
	bool differentStaves = (startEl && endEl) ? (endEl->getGRStaff()!=startEl->getGRStaff()) : false;

	// this is the staff to which the beam belongs and who draws it.
	const GRStaff * beamstaff = sse->startElement->getGRStaff();	
	PosInfos infos = { dirUP, LSPACE, 1.0f, (endEl == startEl), reverseStems(mAssociated), differentStaves, 0, 0 };
	if (getTagType() == SYSTEMTAG) {
		infos.startStaff = startEl->getGRStaff()->getPosition();
		infos.endStaff	 = endEl->getGRStaff()->getPosition();
		yRange(mAssociated, infos.highNote, infos.lowNote);
	}

	const ARBeam * arBeam = getARBeam();
	const bool isSpecBeam = arBeam->isGuidoSpecBeam();

	if (startEl)	infos.stemdir = startEl->getStemDirection();
	else if(endEl)	infos.stemdir = endEl->getStemDirection();

	initp0 (sse, startEl, infos);
	initp1 (sse, infos);
	initp2 (sse, endEl, infos);
	initp3 (sse, infos);

	// -----------------------------------------------------------
	// Now, we adjust the stemlengths, according to beamslope ...
	// we have the start and end-position in st->p
	// We have to determine the slope and adjust the slope to minimum and maximum ...
    // -----------------------------------------------------------
	float stemWidth = st->p[2].x - st->p[0].x;
	if (stemWidth < 0) stemWidth = -stemWidth;
	float slope = 0;
	if (stemWidth > 0) {
		slope = (st->p[2].y - st->p[0].y) / stemWidth;
		// another hack to control the slope when events are on different staves - DF sept 15 2009
		if (startEl && endEl && (startEl->getGRStaff() != endEl->getGRStaff())) {
			while ((slope < -0.20) || (slope > 0.20)) {
				float shift = (slope < 0) ? -LSPACE/4 : LSPACE/4;
				st->p[0].y += shift;
				st->p[1].y += shift;
				st->p[2].y -= shift;
				st->p[3].y -= shift;
				slope = (st->p[2].y - st->p[0].y) / stemWidth;
			}
		}
	}

	bool needsadjust = true;
	// we have to adjust the slope ONLY if the stemlength
	// of the first and last element has not been set automatically!
	// and if we are note in the case of a chained feather beam
	if ( (startEl && startEl->getStemLengthSet() && endEl && endEl->getStemLengthSet())
		|| getTagType() == SYSTEMTAG || (arBeam && isSpecBeam) || (fIsFeathered && startEl && startEl->stemHasBeenChanged()))
	{
		needsadjust = false;
	}
	else slopeAdjust (sse, startEl, endEl, slope, infos);

	if (arBeam && isSpecBeam)
	{
		// then we have to set the length ... if it is not set otherwise ...
		GRSingleNote * nt = dynamic_cast<GRSingleNote *>(sse->startElement);
		if(nt && !nt->getStemLengthSet()) {
			const float myval = arBeam->getDy1()->getValue();
			 nt->setStemLength( myval);
		}

		nt = dynamic_cast<GRSingleNote *>(sse->endElement);
		if(nt && !nt->getStemLengthSet()) {
			float myval;
			const TagParameterFloat* p = arBeam->getDy3();
			if (p && p->TagIsSet())		myval = p->getValue();
			else						myval = arBeam->getDy1()->getValue();
			nt->setStemLength( myval);
		}		
	}
	
	float offsetbeam = 0;
	// nobeamoffset describes, if no beamoffset is valid: if notes ask for different beam-offsets
	// then, there is just no offset .... things must be changed manually then ....
//	bool nobeamoffset = false;
//	
//	if(( startEl && startEl->getStemLengthSet()) || ( endEl && endEl->getStemLengthSet()))
//		  nobeamoffset = true;

	for (int counter = 0; counter < 2; ++counter )
	{
		if (counter == 1) {
			st->p[0].y -= offsetbeam;
			st->p[1].y -= offsetbeam;
			st->p[2].y -= offsetbeam;
			st->p[3].y -= offsetbeam;
			offsetbeam = 0;
		}
		offsetbeam = setStemEndPos(sse, infos, needsadjust, offsetbeam);
	}
	
	if(!fSmallerBeams.empty()) {
		for(auto sb: fSmallerBeams) {
			sb->decLevel();
			sb->tellPosition(sb->getEndElement(), sb->getEndElement()->getPosition());
		}
		return;
	}

	// -- Now we need to add the simplebeams as simplebeamgroups ...
	int dir = st->direction;
	if (st->dirset) {
		// then the direction was set explicitly by the user ...
		const GREvent * sn = mAssociated->GetHead()->isGREvent();
		if (sn) dir = sn->getStemDirection();
	}
	else dir = infos.stemdir;

	// - These constants define the space and the thickness of additionnal beams.
	const float yFact1 = 0.75f * infos.currentLSPACE;	// was 0.7f
	const float yFact2 = 0.4f * infos.currentLSPACE;
	
	// if we have a feathered beam, we just have to draw the main beam (already done) 
	// and the other simple beams will only depend on the begining and ending 
	// points, regardless of the durations of the inner notes.
	if (fIsFeathered) adjustFeathered (yFact1, yFact2, infos, sse);

	// for beam length adjustment - DF sept 15 2009
	setBeams(sse, infos, yFact1, yFact2, dir);

    GuidoPos stemPos = sse->startpos;
    while(stemPos) {
        GREvent * stemNote = GREvent::cast(mAssociated->GetNext(stemPos));
        if(stemNote) {
            GuidoPos tagpos = NULL;
            if (stemNote->getAssociations())
                tagpos = stemNote->getAssociations()->GetHeadPosition();

            while(tagpos) {
                GRNotationElement * tag = stemNote->getAssociations()->GetNext(tagpos);
                GRTremolo * trem = dynamic_cast<GRTremolo*>(tag);
                if(trem) {
                    trem->tellPosition(stemNote,stemNote->getPosition());
                    if(trem->isTwoNotesTremolo()) // in order to force the second pitch (especially the chords) to update
                    {
                        GREvent * secondPitch = dynamic_cast<GREvent*>(mAssociated->GetNext(stemPos));
                        if(secondPitch) trem->tellPosition(secondPitch, secondPitch->getPosition());
                    }
                }
            }
        }
    }

	if (infos.stemsReverse) {
		if (infos.stavesStartEnd) {
			float ratio = startEl->getGRStaff()->getSizeRatio() * endEl->getGRStaff()->getSizeRatio();
			endEl->setStemLength(endEl->getStemLength() + LSPACE * ratio);
		}
		else if (st->simpleBeams) checkEndStemsReverse(endEl, st->simpleBeams);
		else { mDraw = false; } 	// fix issue #86
	}

	// now we have to make sure, that the original positions for the beam are set for the right staff
	if (getTagType() == SYSTEMTAG) {
		const NVPoint &offset = beamstaff->getPosition();
		st->p[0] -= offset;
		st->p[1] -= offset;
		st->p[2] -= offset;
		st->p[3] -= offset;
	}
//cerr << "GRBeam::tellPosition out: \t\t" << st->p[0] << " " << st->p[1] << " " << st->p[2] << " "  << st->p[3] << endl;
}


void GRBeam::setError(const GRStaff * grstaff, int p_error)
{
	GRTag::setError(p_error);
	if (getError() && mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		while (pos) {
			GREvent * grnot = GREvent::cast (mAssociated->GetNext(pos));
			if (grnot)
				grnot->setFlagOnOff(1);
		}
	}

	// this just removes the associations from all active SSE's. The SSE's themselves
	// stay, where they are, so that they are delete properly later ....
	DeleteAllSSEs();

	delete mAssociated;
	mAssociated = NULL;
}

bool GRBeam::checkNotes( GRStaff * grstaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	if (!sse) return false;

	if (mAssociated) {
		GuidoPos pos = sse->startpos;
		bool last = false;
		while (pos) {
			const GREvent * grnot = mAssociated->GetNext(pos)->isGREvent();
			if (!grnot)  {
				setError( grstaff, 1 );
				return false;
			}

			if (last) break;
			if (pos == sse->endpos) last = true;
		}
	}
	return true;
}

/** \brief Called after the new Staff has been created by the Staffmanager after a pbreak-Break.

	(It is ALWAYS called after a call to BreakTag)
	It is somewhat analogous to StaffBegin but uses the assocpos ...
*/
void GRBeam::ResumeTag(GRStaff *grstaff,GuidoPos assocpos)
{
	if (!grstaff) return;
	GRPositionTag::ResumeTag(grstaff,assocpos);
}

/** \brief Break a tag at a previously saved PBreak-Location.

	It sets the sse-Positions ...
	It is somewhat equivalent to StaffFinished with the difference, that assocpos is
	used). assocpos is the tail-position of the mAssociated list at the time of the 
	pbreak situation.
*/
void GRBeam::BreakTag( GRStaff * grstaff, GuidoPos & assocpos)
{
	if (!grstaff) return;

	// do the "basic" stuff.
	GRPositionTag::BreakTag(grstaff,assocpos);

	if (getError()) return;

	GRSystemStartEndStruct *sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);
	GREvent * grn;

	if (!checkNotes(grstaff)) return;
//	if (error) return;

	GuidoPos pos = sse->startpos;
	while (pos)
	{
		GuidoPos prevpos = pos;
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
			grn->setFlagOnOff(0);

		if (prevpos == sse->endpos)
			break;
	}
}

void GRBeam::checkPosition( const GRSystem * grsys)
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grsys);
	assert(sse);

	mIsSystemCall = true;
	tellPosition( sse->endElement, sse->endElement->getPosition());

	mIsSystemCall = false;
}

GRNotationElement * GRBeam::getEndElement()
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	return sse->endElement;
}

void GRBeam::addSmallerBeam(GRBeam * beam)
{
	beam->setLevel(fLevel+1);
	fSmallerBeams.push_back(beam);
}
