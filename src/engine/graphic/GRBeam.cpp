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

std::ostream & operator << ( std::ostream & os, const NVPoint p[4] )
{
	os << p[0] << " " << p[1] << " " << p[2] << " " << p[3];
	return os;
}


GRBeamSaveStruct::~GRBeamSaveStruct()
{
	for (GRSimpleBeam* b: simpleBeams) {
		delete b;
	}
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
		
	float ax [4], ay[4];
	st->fRect.xList(ax);
	st->fRect.yList(ay);

//float offset = 0; // LSPACE
//cerr << (void*)getGRStaff() << " " << getGRStaff()->getPosition() << " p: " << st->p << endl;
//if (st->fRect.topLeft.y > 380) { ay[0]-=400+offset; ay[1]-=400+offset; ay[2]-=400; ay[3]-=400; };
	// This does the drawing!
	hdc.Polygon(ax, ay, 4);
	
	for (GRSimpleBeam* b: st->simpleBeams)
		b->OnDraw (hdc);

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

string GRBeam::beamed() const
{
	stringstream list;
	const NEPointerList * p = getAssociations();
	if (p) list << p->reduce_print();
	return list.str();
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
void GRBeam::initTopLeft (GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos)
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
	st->fRect.topLeft = refEvt ? refEvt->getStemStartPos() : refStaff->getPosition();

	if (arBeam && arBeam->isGuidoSpecBeam() && refEvt)
		st->fRect.topLeft.y = refEvt->getPosition().y;

	if (setref) {
		st->fRect.topLeft.x += infos.startStaff.x;
		float yoffset = 0;
		if (infos.stavesStartEnd && !infos.stemsReverse && !startEl->getStemLengthSet()) {
			if (infos.stemdir == dirUP) {
				yoffset = std::min(infos.endStaff.y, infos.startStaff.y);
			}
			else yoffset = std::max(infos.startStaff.y, infos.endStaff.y);
		}
		else yoffset = infos.startStaff.y;
		st->fRect.topLeft.y += yoffset;
	}
	st->fRect.bottomLeft = st->fRect.topLeft;
	
	// -- Adjust point 0
	const TagParameterFloat * p = arBeam->getDx1();
	if (p && p->TagIsSet())
		st->fRect.topLeft.x += p->getValue(infos.currentLSPACE);
	else {
		// This depends on the direction, we do not know this yet (do we?)
		if (infos.oneNote) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(2.0f, result, "hs", infos.currentLSPACE);

            if (conversionOk)
                st->fRect.topLeft.x -= (float)result * infos.currentSize;
		}
	}

	p = arBeam->getDy1();
	if (p && p->TagIsSet()) st->fRect.topLeft.y -= p->getValue(infos.currentLSPACE);
}

//--------------------------------------------------------------------
void GRBeam::initBottomLeft (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 1
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const TagParameterFloat * p = arBeam->getDx2();
	if (p && p->TagIsSet())
		st->fRect.bottomLeft.x += p->getValue(infos.currentLSPACE);
	else {
		if (infos.oneNote) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(2.0f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->fRect.bottomLeft.x -= (float)result * infos.currentSize;
        }
	}

	p = arBeam->getDy2();
	if (p && p->TagIsSet()) st->fRect.bottomLeft.y -= p->getValue(infos.currentLSPACE);
	else {
		if (infos.stemdir == dirUP) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->fRect.bottomLeft.y = st->fRect.topLeft.y + (float)result * infos.currentSize;
        }
		else if (infos.stemdir == dirDOWN) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->fRect.bottomLeft.y = st->fRect.topLeft.y - (float)result * infos.currentSize;
        }
	}
}

//--------------------------------------------------------------------
void GRBeam::initTopRight (GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos)
{
	// -- Init point 2 (top right)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const ARBeam * arBeam = getARBeam();

	st->fRect.topRight = endEl ? endEl->getStemEndPos() : st->fRect.topLeft;
	// beam length adjustment - DF sept 15 2009
	st->fRect.topRight.x += infos.currentLSPACE/10;
	if (arBeam && arBeam->isGuidoSpecBeam() && endEl)
		st->fRect.topRight.y = endEl->getPosition().y;
	GRStaff * refStaff = endEl ? endEl->getGRStaff() : getGRStaff();
	infos.currentLSPACE = refStaff->getStaffLSPACE();

	if (getTagType() == SYSTEMTAG) {
		st->fRect.topRight += refStaff->getPosition();
		if (infos.stavesStartEnd && !infos.stemsReverse &&  !endEl->getStemLengthSet()) {
			st->fRect.topRight.y = st->fRect.topLeft.y;
			float slopeOffset = infos.currentLSPACE/2;
			if (infos.endStaff.y < infos.startStaff.y) st->fRect.topRight.y -= slopeOffset;
			else st->fRect.topRight.y += slopeOffset;
		}
	}

	st->fRect.bottomRight = st->fRect.topRight;

	// -- Adjust point 2
	const TagParameterFloat * p = arBeam->getDx3();
	if (p)					st->fRect.topRight.x += p->getValue(infos.currentLSPACE);

	p = arBeam->getDy3();
	if (p && p->TagIsSet()) st->fRect.topRight.y -= p->getValue(infos.currentLSPACE);
	else {
		GCoord val = 0;
		const TagParameterFloat * p = arBeam->getDy1();
		if (p && p->TagIsSet())	val = p->getValue(infos.currentLSPACE);
		st->fRect.topRight.y -= val;
	}
}

//--------------------------------------------------------------------
void GRBeam::initBottomRight (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 3 (bottom right)
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	const TagParameterFloat * p = arBeam->getDx4();
	if (p)					st->fRect.bottomRight.x += (p->getValue(infos.currentLSPACE));

	p = arBeam->getDy4();
	if (p && p->TagIsSet())	st->fRect.bottomRight.y -= (p->getValue(infos.currentLSPACE));
	else {
		if (infos.stemdir == dirUP)
        {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->fRect.bottomRight.y = st->fRect.topRight.y + ((float)result * infos.currentSize);
        }
		else if (infos.stemdir == dirDOWN)
        {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE);
			
            if (conversionOk)
                st->fRect.bottomRight.y = st->fRect.topRight.y - ((float)result * infos.currentSize);
        }
	}
}

//--------------------------------------------------------------------
void GRBeam::slopeAdjust (GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl, float slope, PosInfos& infos)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	if (endEl == 0)
	{
		// then we have an empty-event make the slope even ....
		// or make it dependant on generell direction of beam (how ...)
		st->fRect.topRight.y = st->fRect.topLeft.y;
		st->fRect.bottomRight.y = st->fRect.bottomLeft.y;
	}
	else if (startEl == 0)
	{
		st->fRect.topLeft.y = st->fRect.topRight.y;
		st->fRect.bottomLeft.y = st->fRect.bottomRight.y;
	}

//	if (startEl && endEl && infos.stemdir != endEl->getStemDirection())
	else if (infos.stemdir != endEl->getStemDirection())
	{
		// then we should try an optimizing strategy which is not implemented now ....
	}
	else if (slope > 0.1f )
	{
		// adjust the length of the stem for second st->p ...			
		if( infos.stemdir == dirDOWN) {
			// change the first one ...
			const float newy = st->fRect.topRight.y - 0.1f * (st->fRect.topRight.x - st->fRect.topLeft.x);
			const float diff = newy - st->fRect.topLeft.y;
			st->fRect.topLeft.y  	= newy;
			st->fRect.bottomLeft.y += diff;
		}
		else {
			// change the second one ...
			const float newy = st->fRect.topLeft.y + 0.1f * (st->fRect.topRight.x - st->fRect.topLeft.x);
			const float diff = newy - st->fRect.topRight.y;
			st->fRect.topRight.y 	= newy;
			st->fRect.bottomRight.y += diff;
		}
	}
	else if (slope < -0.1f)
	{
		if (infos.stemdir == dirDOWN)
		{
			// change the last one 
			const float newy = -0.1f * (st->fRect.topRight.x - st->fRect.topLeft.x) + st->fRect.topLeft.y;
			const float diff =  newy - st->fRect.topRight.y;
			st->fRect.topRight.y 	 = newy;
			st->fRect.bottomRight.y += diff;
		}
		else
		{
			// change the first
			const float newy = st->fRect.topRight.y + 0.1f * (st->fRect.topRight.x - st->fRect.topLeft.x);
			const float diff =  newy - st->fRect.topLeft.y;
			st->fRect.topLeft.y 	= newy;
			st->fRect.bottomLeft.y += diff;
		}
	}
}

//--------------------------------------------------------------------
void GRBeam::refreshBeams (const GRSystemStartEndStruct * sse, float currentLSPACE, int dir)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	BeamRect r = st->fRect;
	const float space = getBeamSpace (currentLSPACE) * dir;
	for (GRSimpleBeam* b: st->simpleBeams) {
		b->setPoints(r);
		r.yOffset(space);
	}
}

//--------------------------------------------------------------------
bool GRBeam::checkPartialBeaming (GuidoPos pos, GuidoPos endpos, GREvent*& next)
{
	while (pos) {
		GuidoPos prev = pos;
		GREvent * elt = GREvent::cast(mAssociated->GetNext(pos));
		if (elt) {
			if (elt->getBeamCount() < elt->getNumFaehnchen()) {
				next = elt;
				next->incBeamCount();
				continue;
			}
			else return true;
		}
		if (prev == endpos) break;
	}
	return false;
}

//--------------------------------------------------------------------
void GRBeam::setBeams (GRSystemStartEndStruct * sse, PosInfos& infos, float beamSpace, float beamSize, int dir)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	// for beam length adjustment - DF sept 15 2009
	const float xadjust = infos.currentLSPACE/10;
//	int previousBeamsCount = 0;
	
	GuidoPos pos = sse->startpos;
	while (pos) {

		GuidoPos oldpos = pos;
		GREvent * stemNote = GREvent::cast(mAssociated->GetNext(pos));
		if (stemNote) {

			GDirection localDir = stemNote->getStemDirection();
			float localSpace = beamSpace * localDir * infos.currentSize;
			float localSize = beamSize * localDir * infos.currentSize;

			// now we check the number of beams ...
			int beamCount = stemNote->getBeamCount();
			if (beamCount < stemNote->getNumFaehnchen())
			{
				stemNote->incBeamCount();
				BeamRect r;
				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
					// the additional beam starts at the startElement (glue), we have more beams to draw
					r.topLeft = sse->startElement->getPosition();
					if (getTagType() == SYSTEMTAG)
						r.topLeft += stemNote->getGRStaff()->getPosition();
					r.bottomLeft = r.topLeft;
				}
				else {
					// the additional beam starts at sn. We have more beams to draw
					r.topLeft = stemNote->getStemStartPos();
					if (getTagType() == SYSTEMTAG)
						r.topLeft += stemNote->getGRStaff()->getPosition();
					r.topLeft.y += beamCount * localSpace;
					if (localDir != dir)
						r.topLeft.y -= localSize;
					r.bottomLeft.Set (r.topLeft.x, r.topLeft.y + localSize);
				}
				// now we look for the endposition
				GREvent * endElt = nullptr;
				// partialbeam is set if the new SimpleBeam only covers part of the masterBeam.
				bool partialbeam = checkPartialBeaming(pos, sse->endpos, endElt);
				
				if (endElt) {
					if (!partialbeam && sse->endflag == GRSystemStartEndStruct::OPENRIGHT) {
						// then the position is different ...
						r.topRight = sse->endElement->getPosition();
						r.topRight.x += xadjust;
						if (getTagType() == SYSTEMTAG)
							r.topRight += endElt->getGRStaff()->getPosition();
						r.topRight.y = r.topLeft.y;
					}
					else {
						// we have an End-Position ...
						r.topRight = endElt->getStemEndPos();
						r.topRight.x += xadjust;
						if (getTagType() == SYSTEMTAG)
							r.topRight += endElt->getGRStaff()->getPosition();
						r.topRight.y += beamCount * localSpace;
					}
					if (localDir != dir)
						r.topRight.y -= localSize;
					r.bottomRight.Set (r.topRight.x, r.topRight.y + localSize);
				}
				else {
					// we do not have an End-Positon single beam ... (meaning a single straight flag)
					// but only, if it is not open on the left or the right hand side.
					const float slope = st->fRect.slope();
					if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
						// then we have to deal with the startposition of the glue-element ....
						// BUT, you can only set this, if the previous beam had this beamcount at the end ....
						// how do I know that? not now.

						// sn is the only element .... and we are open on the left ...
						if (getTagType() == SYSTEMTAG)
							r.topLeft += stemNote->getGRStaff()->getPosition();
						
						r.bottomLeft = r.topLeft;
						r.topRight = stemNote->getStemEndPos();
						r.topRight.x += xadjust;
						if (getTagType() == SYSTEMTAG)
							r.topRight += stemNote->getGRStaff()->getPosition();
						r.topRight.y += beamCount * localSpace;
						if (localDir != dir)
							r.topRight.y -= localSize;
						r.bottomRight = r.topRight;
						r.bottomRight.y += localSize;
					}
					else if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT) {						
						r.topLeft = stemNote->getStemEndPos();
						if (getTagType() == SYSTEMTAG)
							r.topLeft += stemNote->getGRStaff()->getPosition();
						r.topLeft.y += beamCount * localSpace;
						if (localDir != dir)
							r.topLeft.y -= localSize;
						r.bottomLeft = r.topLeft;
						r.bottomLeft.y += localSize;

						r.topRight = sse->endElement->getPosition();
						r.topRight.x += xadjust;
						if (getTagType() == SYSTEMTAG)
							r.topRight += endElt->getGRStaff()->getPosition(); // ???? endElt is supposed to be null
						r.topRight.y = r.topLeft.y;
						
						r.bottomRight.Set (r.topRight.x, r.bottomLeft.y);
					}
					/* 26/11/03 
					 Beaming bug: wrong direction for partial beam (beam-bug.gmn)
					 can be tested but changing this test. 				 

						startpos check added to correct problem with partial beam
						going outside a group like in [ _/16 c d/8 ]
					*/
					else if( oldpos == sse->endpos || pos == NULL || ((!stemNote->isSyncopated()) && (oldpos != sse->startpos)))
						r = getLeftPartialBeam(stemNote, localSpace, localSize, infos.currentLSPACE, slope, (localDir != dir), beamCount);
//					{
//						// Partial beams leftward ( using slope)
//						r.topRight = stemNote->getStemEndPos();
//						r.topRight.x += xadjust;
//						if (getTagType() == SYSTEMTAG)
//							r.topRight += stemNote->getGRStaff()->getPosition();
//
//						if (localDir != dir)
//							r.topRight.y -= localSize;
//
//						r.topRight.y += beamCount * localSpace;
//						r.bottomRight = r.topRight;
//						r.bottomRight.y += localSize;
//
//						r.topLeft = r.topRight;
//						r.topLeft.x -= infos.currentLSPACE;
//						r.topLeft.y -= slope * infos.currentLSPACE;
//						r.bottomLeft = r.topLeft;
//						r.bottomLeft.y += localSize;
//					}
					else
						getRightPartialBeam(r, localSize, infos.currentLSPACE, slope);
//					{
//						// Partial beams rightward ( using slope)
//						r.topRight = r.topLeft;
//						r.topRight.x += infos.currentLSPACE;
//						r.topRight.y += slope * infos.currentLSPACE;
//						r.bottomRight = r.topRight;
//						r.bottomRight.y += localSize;
//					}
				}
				
				// now we construct a SimpleBeam, we now have to "undo" the systemTag-stuff
				if (getTagType() == SYSTEMTAG) {
					const GRStaff * beamstaff = sse->startElement->getGRStaff();
					const NVPoint & offset = beamstaff->getPosition();
					r -= offset;
				}

				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
					r.topLeft.y = r.topRight.y;
					r.bottomLeft.y = r.bottomRight.y;
				}
				st->simpleBeams.push_back(new GRSimpleBeam(this,r));
				
				pos  = sse->startpos;
				oldpos = pos;
//				previousBeamsCount = stemNote->getBeamCount() - 1;
			}
			// a new hack, again to catch stems directions change - DF sept 15 2009
//			else if (localDir != dir) {
//				// check for stems length
//				NVPoint stemloc = stemNote->getStemStartPos();
//				if (getTagType() == SYSTEMTAG)
//					stemloc += stemNote->getGRStaff()->getPosition();
//				int beamscount = stemNote->getBeamCount() - 1;
//				if ((beamscount > 0) && (previousBeamsCount > beamscount) && (lastLocalDir != localDir)) {
//					if (localDir == dirUP)
//						stemNote->changeStemLength(stemNote->getStemLength() + (localSpace * beamscount));
//					else if (localDir == dirDOWN)
//						stemNote->changeStemLength(stemNote->getStemLength() - (localSpace * beamscount));
//				}
//			}
		}
		if (oldpos == sse->endpos) break;
	}
}

//--------------------------------------------------------------------
// Partial beams rightward ( using slope)
void GRBeam::getRightPartialBeam (BeamRect& r, float size, float lspace, float slope) const
{
	r.topRight = r.topLeft;
	r.topRight.x += lspace;
	r.topRight.y += slope * lspace;
	r.bottomRight = r.topRight;
	r.bottomRight.y += size;
}

//--------------------------------------------------------------------
// Partial beams leftward ( using slope)
BeamRect GRBeam::getLeftPartialBeam (GREvent* elt, float space, float size, float lspace, float slope, bool dirchange, int num) const
{
	BeamRect r;
	r.topRight = elt->getStemEndPos();
	r.topRight.x += lspace / 10;
	if (getTagType() == SYSTEMTAG)
		r.topRight += elt->getGRStaff()->getPosition();
	
	if (dirchange)
		r.topRight.y -= size;

	r.topRight.y += num * space;
	r.bottomRight = r.topRight;
	r.bottomRight.y += size;

	r.topLeft = r.topRight;
	r.topLeft.x -= lspace;
	r.topLeft.y -= slope * lspace;
	r.bottomLeft = r.topLeft;
	r.bottomLeft.y += size;
	return r;
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
			bool empty = false;
			if (sn->isEmpty()) {
				const GREmpty* gr = static_cast<const GREmpty*>(sn);
				empty = !gr->isInChord();
			}
			float rx = sn->getStemStartPos().x - st->fRect.topLeft.x;
			if (getTagType() == SYSTEMTAG)
				rx += sn->getGRStaff()->getPosition().x;
			
			float disty = st->fRect.topRight.y - st->fRect.topLeft.y;
			float distx = st->fRect.topRight.x - st->fRect.topLeft.x;
			float ly = disty / distx * rx;
			
			ly += st->fRect.topLeft.y;
			
			float diffy = sn->getStemStartPos().y;
			GDirection adjustdir=dirAUTO;
			if (sn->getStemDirection() == dirUP) {
				diffy += sn->getStemLength();
				adjustdir = dirUP;
			}
			else if (sn->getStemDirection() == dirDOWN) { 
				adjustdir = dirDOWN;
				diffy -= sn->getStemLength();
			}
			
			ly -= diffy;
			if (getTagType() == SYSTEMTAG) ly -= sn->getGRStaff()->getPosition().y;
			
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
				if (empty) {
					offsetbeam = 0;
				}
				else if (adjustdir == dirUP) {
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
			// so that the possible next featherd beam knows that he is chained (and dont change its slope)
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
void GRBeam::adjustFeathered (float beamSpace, float beamSize, PosInfos& infos, GRSystemStartEndStruct * sse)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	const ARFeatheredBeam * ar = static_cast<const ARFeatheredBeam *>(getARBeam()->isARFeatheredBeam());
	int begin = 0;
	int end = 0;
	const GREvent * stemNoteBegin = mAssociated->GetHead()->isGREvent();
	
	GDirection localDir = stemNoteBegin->getStemDirection();
	float yLocalFact1 = beamSpace * localDir * infos.currentSize;
	float yLocalFact2 = beamSize * localDir * infos.currentSize;
	
	
	// if the user hasn't set the durations as parameters, 
	// we will take the first and last notes'durations
	end = ar->getLastBeaming();
	begin = ar->getFirstBeaming();

	BeamRect r;
	for(int i=1; i <= begin; i++) {
		r.topLeft = st->fRect.topLeft;
		r.topLeft.y += (i-1) * yLocalFact1;
		r.bottomLeft.Set (r.topLeft.x, r.topLeft.y + yLocalFact2);
			
		r.topRight = st->fRect.topRight;
		if(end > i || (end == i && i != 1)) 	// no need to draw the main beam again.
			r.topRight.y += (i-1) * yLocalFact1;
		else
			r.topRight.y += (end-1) * yLocalFact1;
		r.bottomRight.Set (r.topRight.x, r.topRight.y + yLocalFact2);
		st->simpleBeams.push_back(new GRSimpleBeam(this,r));
	}
	// if end > begin
	for(int i = begin; i < end; i++)
	{
		r.topLeft = st->fRect.topLeft;
		r.topLeft.y += (begin-1) * yLocalFact1;
		r.bottomLeft.Set (r.topLeft.x, r.topLeft.y + yLocalFact2);

		r.topRight = st->fRect.topRight;
		r.topRight.y += i * yLocalFact1;
		r.bottomRight.Set (r.topRight.x, r.topRight.y + yLocalFact2);
		st->simpleBeams.push_back(new GRSimpleBeam(this,r));
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
void GRBeam::scanStaves (const NEPointerList* assoc, float& highStaff, float& lowStaff) const
{
	float high = 10000.f;
	float low = 0.f;
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GREvent* ev = assoc->GetNext(pos)->isGREvent();
		if (ev) {
			const GRStaff* currentStaff = ev->getGRStaff();
			float ypos = currentStaff->getPosition().y ;
			if (ypos > low) low = ypos;
			if (ypos < high) high = ypos;
		}
	}
	highStaff = high;
	lowStaff = low;
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
void GRBeam::checkEndStemsReverse  	(GREvent* ev, const SimpleBeamList& beams) const
{
	float maxy = 0;
	float miny = 100000000.f;
	for (GRSimpleBeam* b: beams) {
		if (b->fRect.bottomRight.y > maxy) maxy = b->fRect.bottomRight.y;
		if (b->fRect.topRight.y > miny) miny = b->fRect.topRight.y;
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

//	float maxy = 0.f;
//	float miny = 0.f;
////	float miny = 100000000.f;
//
//	float slength = ev->getStemLength();
//	float thick = ev->getGRStaff()->getStaffLSPACE()/4; // this is to take account of the beam thickness
//
//	for (GRSimpleBeam* b: beams) {
////cerr << "GRBeam::checkEndStemsReverse ty/by : " << b->fRect.topRight.y << " / " << b->fRect.bottomRight.y <<  endl;
//		if ((b->fRect.bottomRight.y > maxy) || !maxy) maxy = b->fRect.bottomRight.y;
////		float minoffset = (staff != b->getGRStaff()) ? b->getGRStaff()->getPosition().y : 0.f;
//		if ((b->fRect.topRight.y > miny) || !miny) miny = b->fRect.topRight.y;
//	}
//cerr << "GRBeam::checkEndStemsReverse maxy miny : " << maxy << " / " << miny << endl;
//	if (ev->getStemDirection() == dirUP) {
//		float y = ev->getPosition().y - slength;
//		if (y > miny) ev->setStemLength(slength + y - miny - thick);
//	}
//	else {
//		float y = ev->getPosition().y + slength;
//		if (y < maxy) {
//			ev->setStemLength(slength + maxy - y - thick);
//cerr << "GRBeam::checkEndStemsReverse fixed : " << ev->getStemLength() << endl;
//		}
////		else if (std::abs(miny) > slength) ev->setStemLength(std::abs(miny));
//	}
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

//cerr << (void*)this <<  " GRBeam::tellPosition in type " << getTagType() << " - " << el << " start elt " << sse->startElement << " end elt " << sse->endElement << " level: " << fLevel  << endl;

	if (el != sse->endElement) return;
	if(fLevel != 0) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const GREvent * startEl = sse->startElement->isGREvent();
	GREvent * endEl   = sse->endElement->isGREvent();

//cerr << "GRBeam::tellPosition " << endl;
	if (startEl && startEl->getGlobalStem()) startEl->getGlobalStem()->setBeam (this);
	if (endEl   && endEl->getGlobalStem()) endEl->getGlobalStem()->setBeam (this);
	bool differentStaves = (startEl && endEl) ? (endEl->getGRStaff()!=startEl->getGRStaff()) : false;

	// this is the staff to which the beam belongs and who draws it.
//	const GRStaff * beamstaff = sse->startElement->getGRStaff();
	PosInfos infos = { dirUP, LSPACE, 1.0f, (endEl == startEl), reverseStems(mAssociated), differentStaves, 0, 0 };
	if (getTagType() == SYSTEMTAG) {
		infos.startStaff = startEl->getGRStaff()->getPosition();
		infos.endStaff	 = endEl->getGRStaff()->getPosition();
		scanStaves(mAssociated, infos.highStaff, infos.lowStaff);
		yRange(mAssociated, infos.highNote, infos.lowNote);
	}

	const ARBeam * arBeam = getARBeam();
	const bool isSpecBeam = arBeam->isGuidoSpecBeam();

	if (startEl)	infos.stemdir = startEl->getStemDirection();
	else if(endEl)	infos.stemdir = endEl->getStemDirection();

	initTopLeft 	(sse, startEl, infos);
	initBottomLeft 	(sse, infos);
	initTopRight 	(sse, endEl, infos);
	initBottomRight (sse, infos);

	// -----------------------------------------------------------
	// Now, we adjust the stemlengths, according to beamslope ...
	// we have the start and end-position in st->p
	// We have to determine the slope and adjust the slope to minimum and maximum ...
    // -----------------------------------------------------------
//	float stemWidth = st->fRect.topRight.x - st->fRect.topLeft.x;
	float stemWidth = st->fRect.width();
//	if (stemWidth < 0) stemWidth = -stemWidth;
	float slope = 0;
	if (stemWidth > 0) {
		slope = st->fRect.slope();
		// another hack to control the slope when events are on different staves - DF sept 15 2009
		float shift = LSPACE/4;
		if (startEl && endEl && (startEl->getGRStaff() != endEl->getGRStaff())) {
			while (slope < -0.20) {
				st->fRect.tilt (shift);
				slope = st->fRect.slope();
			}
			while (slope > 0.20) {
				st->fRect.tilt (-shift);
				slope = st->fRect.slope();
			}
		}
//cerr << " slope adjust : " << slope << endl;
	}

	bool needsadjust = true;
	// we have to adjust the slope ONLY if the stemlength
	// of the first and last element has not been set automatically!
	// and if we are note in the case of a chained feather beam
	if ( (startEl && startEl->getStemLengthSet() && endEl && endEl->getStemLengthSet())
		|| getTagType() == SYSTEMTAG || (arBeam && isSpecBeam) || (fIsFeathered && startEl && startEl->stemHasBeenChanged()))
		needsadjust = false;
	if (infos.fixCrossStaffUp()) needsadjust = true;
	if (needsadjust) slopeAdjust (sse, startEl, endEl, slope, infos);

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
	
	// nobeamoffset describes, if no beamoffset is valid: if notes ask for different beam-offsets
	// then, there is just no offset .... things must be changed manually then ....
//	bool nobeamoffset = false;
//	
//	if(( startEl && startEl->getStemLengthSet()) || ( endEl && endEl->getStemLengthSet()))
//		  nobeamoffset = true;

	float offsetbeam = setStemEndPos(sse, infos, needsadjust, 0);
	if (offsetbeam ) {
		st->fRect.yOffset(-offsetbeam);
	}
	setStemEndPos(sse, infos, needsadjust, 0); // has to be called twice! don't know why (?)


	if(!fSmallerBeams.empty()) {
		for(auto sb: fSmallerBeams) {
			sb->decLevel();
			sb->tellPosition(sb->getEndElement(), sb->getEndElement()->getPosition());
		}
		if (fIsFeathered || !mIsSystemCall) return;
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
	const float beamSpace = getBeamSpace (infos.currentLSPACE);
	const float beamSize  = getBeamSize (infos.currentLSPACE);
	
	// if we have a feathered beam, we just have to draw the main beam (already done) 
	// and the other simple beams will only depend on the begining and ending 
	// points, regardless of the durations of the inner notes.
	if (fIsFeathered) adjustFeathered (beamSpace, beamSize, infos, sse);

	// for beam length adjustment - DF sept 15 2009
	else setBeams(sse, infos, beamSpace, beamSize, dir);

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
		else if (st->simpleBeams.size()) checkEndStemsReverse(endEl, st->simpleBeams);
		else { mDraw = false; } 	// fix issue #86
	}

//	if (infos.stemsReverse && (st->simpleBeams.size() > 1)) {
//		checkEndStemsReverse(endEl, st->simpleBeams);
//		if (infos.stavesStartEnd ) {
//			checkEndStemsReverse(endEl, st->simpleBeams);
//		}
//
//
//
////		if (infos.stavesStartEnd) {
////			float ratio = startEl->getGRStaff()->getSizeRatio() * endEl->getGRStaff()->getSizeRatio();
////			endEl->setStemLength(endEl->getStemLength() + LSPACE * ratio);
////		}
////		else
////		if (infos.stavesStartEnd || st->simpleBeams.size()) checkEndStemsReverse(endEl, st->simpleBeams);
////		else { mDraw = false; } 	// fix issue #86
//	}

	// now we have to make sure, that the original positions for the beam are set for the right staff
	if (getTagType() == SYSTEMTAG) {
		const GRStaff * beamstaff = sse->startElement->getGRStaff();
		const NVPoint &offset = beamstaff->getPosition();
//cerr << (void*)this <<  " GRBeam::tellPosition offset \t\t" << offset << endl;
		st->fRect-= offset;
	}
//cerr << (void*)this <<  " GRBeam::tellPosition out: \t\t" << st->fRect.topLeft << " " << st->fRect.bottomLeft << " " << st->fRect.topRight << " "  << st->fRect.bottomRight << endl;
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
